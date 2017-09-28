/*
 * Copyright (c) 2017 Harvis Wang.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_device.h> /* module_platform_driver */
#include <linux/dma-mapping.h> /* */
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>

#include <asm/barrier.h>

#include "../driverlib/msc.h"

#define DUMP_TIMES(n) \
do { \
    static int times = n; \
    if (times) { \
        dump_stack(); \
        times--; \
    } \
} while (0)

struct m200_mmc_host {
    unsigned long mmc_mem_base; /* mmc memory access base address get from DT */
    struct clk *clk;
    unsigned int clock;
    struct mmc_host *mmc_host;
    struct mmc_command *mmc_command;
    struct device *dev;
    struct sdma_descriptor *sdma_descriptor;
    unsigned char sdma_descriptor_count;
    unsigned char power_mode;
    unsigned char bus_width;
    int irq;
    spinlock_t spinlock;
};

static int m200_mmc_request_done(struct mmc_host *host, struct mmc_request *request)
{
    struct m200_mmc_host *mmh = mmc_priv(host);

    mmh->mmc_command = NULL;

    mmc_request_done(mmh->mmc_host, request);
    return 0;
}

static int m200_mmc_data_done(struct mmc_host *host, struct mmc_data *data)
{
    struct m200_mmc_host *mmh = mmc_priv(host);
    printk("%s line:%d\n", __func__, __LINE__);

    if (data->error) {
        data->bytes_xfered = 0;
        dev_err(mmh->dev, "MMC Data failed at %s line:%d\n", __func__, __LINE__);
    } else {
        data->bytes_xfered = data->blocks * data->blksz;
    }

    MSCDataTransferDoneFlagClear(mmh->mmc_mem_base);
    MSCDataTransferDoneDisable(mmh->mmc_mem_base);
    if (data->stop) { // TODO
        printk("%s line:%d data->stop:0x%p\n", __func__, __LINE__, data->stop);
    } else {
        m200_mmc_request_done(host, data->mrq);
        printk("%s line:%d\n", __func__, __LINE__);
    }

    return 0;
}

static int m200_mmc_data_start(struct mmc_host *host, struct mmc_data *data)
{
#if 0
    struct m200_mmc_host *mmh = mmc_priv(host);
    MSCDMADataDoneEnable(mmh->mmc_mem_base);
    MSCDMAEndEnable(mmh->mmc_mem_base);

    printk("%s line:%d NDA:0x%08x DA:0x%08x LEN:0x%08x CMD:0x%08x\n", __func__, __LINE__, mmh->sdma_descriptor->NDA, mmh->sdma_descriptor->DA, mmh->sdma_descriptor->LEN, mmh->sdma_descriptor->CMD);
{
    int result;
    result = MSCDMAEnable(mmh->mmc_mem_base);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);
}
#endif
    return 0;
}

static int m200_mmc_command_done(struct mmc_host *host, struct mmc_command *command)
{
    struct m200_mmc_host *mmh = mmc_priv(host);

    MSCEndCommandResponseFlagClear(mmh->mmc_mem_base);
    MSCEndCommandResponseDisable(mmh->mmc_mem_base);

    if (command->flags & MMC_RSP_PRESENT) {
        if (command->flags & MMC_RSP_136) { /* 136 bits command response */
            /* response type 2 */
            int i;
            unsigned long ulResponse = MSCCommandResponseGet(mmh->mmc_mem_base);

            for (i = 0; i < ARRAY_SIZE(command->resp); i++) {
                command->resp[i] = ulResponse << 24;
                ulResponse = MSCCommandResponseGet(mmh->mmc_mem_base);
                command->resp[i] |= ulResponse << 8;
                ulResponse = MSCCommandResponseGet(mmh->mmc_mem_base);
                command->resp[i] |= ulResponse >> 8;
            }
        } else { /* 48 bits command response */
            /* response types 1, 1b, 3, 4, 5, 6 */
            int i;
            unsigned long ulResponse = MSCCommandResponseGet(mmh->mmc_mem_base);

            command->resp[0] = ulResponse << 24;
            ulResponse = MSCCommandResponseGet(mmh->mmc_mem_base);
            command->resp[0] |= ulResponse << 8;
            ulResponse = MSCCommandResponseGet(mmh->mmc_mem_base);
            command->resp[0] |= ulResponse & 0x00FF;

            for (i = 1; i < ARRAY_SIZE(command->resp); i++) {
                command->resp[i] = 0x00000000;
            }
        }
    }

    if (command->error) { /* command with error */
        // TODO error handle
        printk("%s line:%d\n", __func__, __LINE__);
    } else if (command->data) { /* command with data */
        MSCDataTransferDoneEnable(mmh->mmc_mem_base);
    } else { /* command without data */
        m200_mmc_request_done(host, command->mrq);
    }

    return 0;
}

static int m200_mmc_command_start(struct mmc_host *host, struct mmc_command *command)
{
    struct m200_mmc_host *mmh = mmc_priv(host);

    switch(mmc_resp_type(command)) {
    case MMC_RSP_NONE:
        MSCResponseFormatSet(mmh->mmc_mem_base, MSC_RESPONSE_NONE);
        break;
    case MMC_RSP_R1: //case MMC_RSP_R5: //case MMC_RSP_R6: //case MMC_RSP_R7:/* as MMC_RSP_R1 == MMC_RSP_R5 == MMC_RSP_R6 == MMC_RSP_R7 */
        MSCResponseFormatSet(mmh->mmc_mem_base, MSC_RESPONSE_R1R1B);
        break;
    case MMC_RSP_R1B:
        MSCResponseFormatSet(mmh->mmc_mem_base, MSC_RESPONSE_R1R1B);
        break;
    case MMC_RSP_R2:
        MSCResponseFormatSet(mmh->mmc_mem_base, MSC_RESPONSE_R2);
        break;
    case MMC_RSP_R3: //case MMC_RSP_R4: /* as MMC_RSP_R4 == MMC_RSP_R3 */
        MSCResponseFormatSet(mmh->mmc_mem_base, MSC_RESPONSE_R3);
        break;
    default:
        dev_err(mmh->dev, "Unsupport mmc response type(0x%x) at %s line:%d\n", mmc_resp_type(command), __func__, __LINE__);
        break;
    }

    mmh->mmc_command = command;

    if (command->flags & MMC_RSP_BUSY) { /* R1B */
        MSCBusySet(mmh->mmc_mem_base);
    } else {
        MSCBusyClear(mmh->mmc_mem_base);
    }

    //printk("%s line:%d command->mrq:0x%p\n", __func__, __LINE__, command->mrq);
    if (command->mrq->stop) {
        MSCAutoCMD12Enable(mmh->mmc_mem_base);
    } else {
        MSCAutoCMD12Disable(mmh->mmc_mem_base);
    }

    MSCCommandIndexSet(mmh->mmc_mem_base, command->opcode);
    MSCCommandArgumentSet(mmh->mmc_mem_base, command->arg);
    MSCEndCommandResponseEnable(mmh->mmc_mem_base);
    MSCNewOperationStart(mmh->mmc_mem_base);

    return 0;
}

/* f(data->timeout_ns, data->timeout_clks) = total timeout_clks */
static unsigned int m200_mmc_timout_ticks_calculate(struct mmc_host *host, struct mmc_data *data)
{
    struct m200_mmc_host *mmh = mmc_priv(host);
    const unsigned int clock_per_ms = mmh->clock / 1000;
    const unsigned int ms = data->timeout_ns / 1000;
    const unsigned int ticks = ms * clock_per_ms;

    return data->timeout_clks + ticks;
}

/**/
static int m200_mmc_data_setup(struct mmc_host *host, struct mmc_data *data)
{
    struct m200_mmc_host *mmh = mmc_priv(host);
    unsigned int ticks = m200_mmc_timout_ticks_calculate(host, data);
//    int i;
//    struct scatterlist *scatterlist;
//    int dma_direction = data->flags & MMC_DATA_WRITE ? DMA_TO_DEVICE : DMA_FROM_DEVICE;
//    struct sdma_descriptor *sdma_descriptor = mmh->sdma_descriptor;
//    int result;
#if 0
    /* DMA prepare */
    printk("%s line:%d data->sg_len:%d ticks:0x%08X\n", __func__, __LINE__, data->sg_len, ticks);
    dma_map_sg(mmh->dev, data->sg, data->sg_len, dma_direction);


    printk("%s line:%d data->sg_len:%d\n", __func__, __LINE__, data->sg_len);
    for_each_sg(data->sg, scatterlist, data->sg_len, i) {
        sdma_descriptor = &mmh->sdma_descriptor[i];

        sdma_descriptor->NDA = CPHYSADDR((unsigned long)&mmh->sdma_descriptor[i+1]);
        sdma_descriptor->DA  = sg_phys(scatterlist);
        sdma_descriptor->LEN = sg_dma_len(scatterlist);
        sdma_descriptor->CMD = 0;
        sdma_descriptor->CMD &= ~MSC_SDMACMD_ENDI;
        sdma_descriptor->CMD |= MSC_SDMACMD_LINK;

        // TODO no enough dma descriptor, how to deal with
    }
    dma_unmap_sg(mmh->dev, data->sg, data->sg_len, dma_direction);
    /* Last sdma_descriptor enable end interrupt, no link */
    sdma_descriptor->CMD |= MSC_SDMACMD_ENDI;
    sdma_descriptor->CMD &= ~MSC_SDMACMD_LINK;

    result = MSCDMADescriptorAddressSet(mmh->mmc_mem_base, CPHYSADDR((unsigned long)(mmh->sdma_descriptor)));
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);
#endif
    MSCBlockCountSet(mmh->mmc_mem_base, data->blocks);
    MSCBlockSizeSet(mmh->mmc_mem_base, data->blksz);
    MSCReadTimeOutSet(mmh->mmc_mem_base, ticks);
    MSCDataEnable(mmh->mmc_mem_base);
    if (data->flags & MMC_DATA_READ) {
        MSCDataRead(mmh->mmc_mem_base);
    } else {
        MSCDataWrite(mmh->mmc_mem_base);
    }

    if (data->flags & MMC_DATA_STREAM) {
        MSCStreamModeEnable(mmh->mmc_mem_base);
    } else {
        MSCStreamModeDisable(mmh->mmc_mem_base);
    }
    MSCRXFIFOReadRequestEnable(mmh->mmc_mem_base);
    MSCDataFIFOFullEnable(mmh->mmc_mem_base);
    MSCDataFIFOEmptyEnable(mmh->mmc_mem_base);
    MSCCRCResponseErrorEnable(mmh->mmc_mem_base);
    MSCCRCReadErrorEnable(mmh->mmc_mem_base);
    printk("%s line:%d\n", __func__, __LINE__);
    MSCRegisterDump(mmh->mmc_mem_base, printk);

    return 0;
}

/* ops callbacks */
static void m200_mmc_host_ops_post_req(struct mmc_host *host, struct mmc_request *req, int err)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static void m200_mmc_host_ops_pre_req(struct mmc_host *host, struct mmc_request *req, bool is_first_req)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static void m200_mmc_host_ops_request(struct mmc_host *host, struct mmc_request *req)
{
    struct m200_mmc_host *mmh = mmc_priv(host);
    struct mmc_command *mmc_command = req->cmd;
    struct mmc_data *mmc_data = req->data;

//    printk("%s line:%d mmh->clock:%d mmc_command->opcode:%d mmc_command->arg:0x%08X req:0x%p mmc_data:0x%p\n",
//         __func__, __LINE__, mmh->clock, mmc_command->opcode, mmc_command->arg, req, mmc_data);

    if (mmc_data != NULL) {
        m200_mmc_data_setup(host, mmc_data);
    } else {
        MSCDataDisable(mmh->mmc_mem_base);
    }

    m200_mmc_command_start(host, mmc_command);

    if (mmc_data != NULL) {
        m200_mmc_data_start(host, mmc_data);
    }
}

static void m200_mmc_host_ops_set_ios(struct mmc_host *host, struct mmc_ios *ios)
{
    struct m200_mmc_host *mmh = mmc_priv(host);
    //printk("%s line:%d ios->clock:%d\n", __func__, __LINE__, ios->clock);

    /* power mode */
    if (ios->power_mode != mmh->power_mode) {
        switch (ios->power_mode) {
        case MMC_POWER_OFF:
            break;
        case MMC_POWER_UP:
            /* set MMC Vdd */
            break;
        case MMC_POWER_ON:
            break;
        default:
            dev_err(mmh->dev, "Unsupport ios->power_mode: %d at %s line:%d\n", ios->power_mode, __func__, __LINE__);
            break;
        }
        mmh->power_mode = ios->power_mode;
    }

    /* bus width */
    if (ios->bus_width != mmh->bus_width) {
        switch(ios->bus_width) {
        case MMC_BUS_WIDTH_1:
            MSCBusWidthSet(mmh->mmc_mem_base, MSC_BUSWIDTH_1BIT);
            break;
        case MMC_BUS_WIDTH_4:
            MSCBusWidthSet(mmh->mmc_mem_base, MSC_BUSWIDTH_4BIT);
            break;
        case MMC_BUS_WIDTH_8:
            MSCBusWidthSet(mmh->mmc_mem_base, MSC_BUSWIDTH_8BIT);
            break;
        default:
            dev_err(mmh->dev, "Unsupport ios->bus_width: %d at %s line:%d\n", ios->bus_width, __func__, __LINE__);
            break;
        }
        mmh->bus_width = ios->bus_width;
    }

    /* clock rate */
    if (ios->clock != mmh->clock) {
        clk_set_rate(mmh->clk, (unsigned long)ios->clock);
        mmh->clock = ios->clock;
    }

}

/*
 * return 1 for a read-only card
 *        0 for a read/write card
 *        < 0 error
 * TODO
 */
static int m200_mmc_host_ops_get_ro(struct mmc_host *host)
{
    //struct m200_mmc_host *mmh = mmc_priv(host);

    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

/*
 * return 1 for a present card
 *        0 for a absent card
 *        < 0 error
 * TODO
 */
static int m200_mmc_host_ops_get_cd(struct mmc_host *host)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 1;
}

static void m200_mmc_host_ops_enable_sdio_irq(struct mmc_host *host, int enable)
{
    struct m200_mmc_host *mmh = mmc_priv(host);

    printk("%s line:%d\n", __func__, __LINE__);
    if (enable) {
        MSCSDIOInterruptEnable(mmh->mmc_mem_base);
    } else {
        MSCSDIOInterruptDisable(mmh->mmc_mem_base);
    }
}

static void m200_mmc_host_ops_init_card(struct mmc_host *host, struct mmc_card *card)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static int m200_mmc_host_ops_start_signal_voltage_switch(struct mmc_host *host, struct mmc_ios *ios)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_mmc_host_ops_card_busy(struct mmc_host *host)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_mmc_host_ops_execute_tuning(struct mmc_host *host, u32 opcode)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_mmc_host_ops_select_drive_strength(unsigned int max_dtr, int host_drv, int card_drv)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static void m200_mmc_host_ops_hw_reset(struct mmc_host *host)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static void m200_mmc_host_ops_card_event(struct mmc_host *host)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static struct mmc_host_ops m200_mmc_host_ops = {
    .post_req = m200_mmc_host_ops_post_req,
    .pre_req  = m200_mmc_host_ops_pre_req,
    .request  = m200_mmc_host_ops_request,
    .set_ios  = m200_mmc_host_ops_set_ios,
    .get_ro   = m200_mmc_host_ops_get_ro,
    .get_cd   = m200_mmc_host_ops_get_cd,
    .enable_sdio_irq = m200_mmc_host_ops_enable_sdio_irq,
    .init_card       = m200_mmc_host_ops_init_card,
    .start_signal_voltage_switch = m200_mmc_host_ops_start_signal_voltage_switch,
    .card_busy       = m200_mmc_host_ops_card_busy,
    .execute_tuning  = m200_mmc_host_ops_execute_tuning,
    .select_drive_strength = m200_mmc_host_ops_select_drive_strength,
    .hw_reset        = m200_mmc_host_ops_hw_reset,
    .card_event      = m200_mmc_host_ops_card_event,
};

static struct of_device_id m200_mmc_match_table[] = {
    { .compatible = "ingenic,m200-mmc", },
};

static irqreturn_t m200_mmc_irq_handler(int irq, void *dev_id)
{
    struct m200_mmc_host *mmh = (struct m200_mmc_host *)dev_id;
    if (MSCSDIOInterruptFlagGet(mmh->mmc_mem_base)) {
        mmc_signal_sdio_irq(mmh->mmc_host);
        printk("%s line:%d SDIO %s\n", __func__, __LINE__, mmh->mmc_command && mmh->mmc_command->data ? "data" : "");
    } else if (MSCEndCommandResponseFlagGet(mmh->mmc_mem_base)) {
        printk("%s line:%d CMD DONE %s\n", __func__, __LINE__, mmh->mmc_command && mmh->mmc_command->data ? "data" : "");
        //MSCRegisterDump(mmh->mmc_mem_base, printk);
        m200_mmc_command_done(mmh->mmc_host, mmh->mmc_command);
    } else if (MSCDataTransferDoneFlagGet(mmh->mmc_mem_base)) {
        m200_mmc_data_done(mmh->mmc_host, mmh->mmc_command->data);
        printk("%s line:%d DATA TRANSFER DONE %s\n", __func__, __LINE__, mmh->mmc_command && mmh->mmc_command->data ? "data" : "");
#if 0
    } else if (MSCDMADataDoneFlagGet(mmh->mmc_mem_base)) {
        MSCDMADataDoneFlagClear(mmh->mmc_mem_base);
        MSCDMADataDoneDisable(mmh->mmc_mem_base);
        printk("%s line:%d DMA DATA DONE\n", __func__, __LINE__);
    } else if (MSCDMAEndFlagGet(mmh->mmc_mem_base)) {
        MSCDMAEndFlagClear(mmh->mmc_mem_base);
        MSCDMADisable(mmh->mmc_mem_base);
        printk("%s line:%d DMA END\n", __func__, __LINE__);
#endif
    } else if (MSCRXFIFOReadRequestFlagGet(mmh->mmc_mem_base)) {
        /* read */
        struct mmc_data *data = mmh->mmc_command->data;
        unsigned int *address;
        unsigned int count;
        int i = 0;

        printk("%s line:%d\n", __func__, __LINE__);
        address = sg_virt(data->sg);
        count = sg_dma_len(data->sg);
        //MSCRegisterDump(mmh->mmc_mem_base, printk);
        for (i = 0; i < count / 4; i++) {
            *address = MSCRXFIFOGet(mmh->mmc_mem_base);
            address++;
        }
        printk("%s line:%d RXFIFO RD REQ\n", __func__, __LINE__);
        //MSCRXFIFOReadRequestFlagClear(mmh->mmc_mem_base);
        //MSCRXFIFOReadRequestDisable(mmh->mmc_mem_base);
        //MSCRegisterDump(mmh->mmc_mem_base, printk);
    } else if (MSCDataFIFOFullFlagGet(mmh->mmc_mem_base)) {
        printk("%s line:%d DATA FIFO FULL\n", __func__, __LINE__);
    } else if (MSCDataFIFOEmptyFlagGet(mmh->mmc_mem_base)) {
        printk("%s line:%d DATA FIFO EMPTY\n", __func__, __LINE__);
        MSCDataFIFOEmptyDisable(mmh->mmc_mem_base);
        MSCRegisterDump(mmh->mmc_mem_base, printk);
    } else if (MSCCRCResponseErrorFlagGet(mmh->mmc_mem_base)) {
        printk("%s line:%d CRC Response Error\n", __func__, __LINE__);
    } else if (MSCCRCReadErrorFlagGet(mmh->mmc_mem_base)) {
        printk("%s line:%d CRC Read Error\n", __func__, __LINE__);
    } else {
        printk("%s line:%d UNKNOW %s\n", __func__, __LINE__,
            mmh->mmc_command && mmh->mmc_command->data ? "data" : "");
    }

    return IRQ_HANDLED;
}

static int m200_mmc_platform_driver_probe(struct platform_device *pdev)
{
    struct mmc_host *mmc_host;
    struct m200_mmc_host *mmh;
    struct device *dev = &pdev->dev;
    struct device_node *device_node = dev->of_node;
    const struct of_device_id *match;
    struct resource resource;
    int err;
    u32 bus_width;
    u32 sdma_descriptor_count;

    match = of_match_device(m200_mmc_match_table, dev);
    if (match == NULL) {
        dev_err(dev, "Error: No device match found at %s\n", __func__);
        return -ENODEV;
    }

    mmc_host = mmc_alloc_host(sizeof(struct m200_mmc_host), dev);
    if (mmc_host == NULL) {
        dev_err(dev, "mmc_alloc_host failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    mmc_host->ocr_avail = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 |
                          MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;

    err = of_property_read_u32_index(device_node, "min-frequency", 0, &mmc_host->f_min);
    if (err) {
        dev_err(dev, "Error: No min-frequency configuration in MMC DT at %s\n", __func__);
        return -ENODEV;
    }

    err = of_property_read_u32_index(device_node, "max-frequency", 0, &mmc_host->f_max);
    if (err) {
        dev_err(dev, "Error: No max-frequency configuration in MMC DT at %s\n", __func__);
        return -ENODEV;
    }
    printk("%s line:%d mmc_host->f_min:%d mmc_host->f_max:%d\n", __func__, __LINE__, mmc_host->f_min, mmc_host->f_max);

    mmh = mmc_priv(mmc_host);
    mmh->mmc_host = mmc_host;
    printk("%s line:%d mmc_host:0x%p mmh:0x%p\n", __func__, __LINE__, mmc_host, mmh);

    err = of_property_read_u32_index(device_node, "bus-width", 0, &bus_width);
    if (err) {
        dev_err(dev, "Error: No bus-width configuration in MMC DT at %s\n", __func__);
        return -ENODEV;
    }
    mmh->bus_width = bus_width;
    printk("%s line:%d mmh->bus_width:%d\n", __func__, __LINE__, mmh->bus_width);
    switch (mmh->bus_width) {
    case 1:
        //mmh->mmc_host->caps |= MMC_CAP_1_BIT_DATA; /* no MMC_CAP_1_BIT_DATA defined */
        break;
    case 4:
        mmh->mmc_host->caps = MMC_CAP_4_BIT_DATA;
        break;
    case 8:
        mmh->mmc_host->caps = MMC_CAP_8_BIT_DATA;
        mmh->mmc_host->caps |= MMC_CAP_SDIO_IRQ;
        break;
    default:
        dev_err(dev, "Error: wrong bus-width in MMC DT at %s\n", __func__);
        break;
    }

    err = of_property_read_u32_index(device_node, "sdma-descriptor-count", 0, &sdma_descriptor_count);
    if (err) {
        dev_err(dev, "Error: No sdma-descriptor-count configuration in MMC DT at %s\n", __func__);
        return -ENODEV;
    }
    mmh->sdma_descriptor_count = sdma_descriptor_count;

    printk("%s line:%d mmh->sdma_descriptor_count:%d\n", __func__, __LINE__, mmh->sdma_descriptor_count);
    mmh->sdma_descriptor = kzalloc(sizeof(struct sdma_descriptor) * mmh->sdma_descriptor_count, GFP_KERNEL);
    if (mmh->sdma_descriptor == NULL) {
        dev_err(dev, "Error: alloc memory for SDMA descriptor at %s\n", __func__);
        return -ENOMEM;
    } else {
        int i;
        for (i = 0; i < mmh->sdma_descriptor_count; i++) {
            mmh->sdma_descriptor[i].NDA = 0xFFFFFFFF;
            mmh->sdma_descriptor[i].DA  = 0xFFFFFFFF;
            mmh->sdma_descriptor[i].LEN = 0xFFFFFFFF;
            mmh->sdma_descriptor[i].CMD = 0xFFFFFFFF;
        }
    }

    mmh->sdma_descriptor = ioremap_nocache(virt_to_phys(mmh->sdma_descriptor), sizeof(struct sdma_descriptor) * mmh->sdma_descriptor_count);
    if (mmh->sdma_descriptor == NULL) {
        dev_err(dev, "Error: remap SDMA descriptor failed at %s\n", __func__);
        return -ENOMEM;
    }
    printk("%s line:%d mmh->sdma_descriptor:0x%p\n", __func__, __LINE__, mmh->sdma_descriptor);

    mmh->clk = of_clk_get(device_node, 0);
    printk("%s line:%d mmh->clk:0x%p\n", __func__, __LINE__, mmh->clk);
    mmh->clock = clk_get_rate(mmh->clk);
    printk("%s line:%d mmh->clock:%d\n", __func__, __LINE__, mmh->clock);

    mmh->dev = dev;
    mmh->power_mode = MMC_POWER_OFF;
    mmh->irq = platform_get_irq(pdev, 0); /* the same as irq_of_parse_and_map(device_node, 0) */
    printk("%s line:%d mmh->irq:%d\n", __func__, __LINE__, mmh->irq);

    err = of_address_to_resource(device_node, 0, &resource);
    if (err) {
        dev_err(&pdev->dev, "Error: None resource  at %s err:%d\n", __func__, err);
        return -EINVAL;
    }
    mmh->mmc_mem_base = resource.start;

    err = request_irq(mmh->irq, m200_mmc_irq_handler, 0, mmc_hostname(mmc_host), mmh);
    if (err) {
        dev_err(dev, "Unable request mmc irq at %s line:%d\n", __func__, __LINE__);
        return err;
    }

    mmc_host->ops = &m200_mmc_host_ops;
    err = mmc_add_host(mmc_host);

    spin_lock_init(&mmh->spinlock);
    platform_set_drvdata(pdev, mmh);

    return err;
}

static int m200_mmc_platform_driver_remove(struct platform_device *pdev)
{
    struct m200_mmc_host *mmh = platform_get_drvdata(pdev);

    printk("%s line:%d\n", __func__, __LINE__);
    kfree(mmh);
    return 0;
}

static struct of_device_id m200_mmc_platform_driver_of_match_table[] = {
    {.compatible = "ingenic,m200-mmc", },
};
MODULE_DEVICE_TABLE(of, m200_mmc_platform_driver_of_match_table);

static struct platform_driver m200_mmc_platform_driver = {
    .driver = {
        .name  = "m200_mmc",
        .owner = THIS_MODULE,
        .of_match_table = m200_mmc_platform_driver_of_match_table,
    },
    .probe  = m200_mmc_platform_driver_probe,
    .remove = m200_mmc_platform_driver_remove,
};
module_platform_driver(m200_mmc_platform_driver);

MODULE_DESCRIPTION("M200 MMC/SD Driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Harvis Wang");

