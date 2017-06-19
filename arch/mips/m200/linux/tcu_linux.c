/*
 * Timer Counter Unit(TCU)
 * TODO: HZ=100, but actually I measure it's 102
 * measure method: cat /proc/interrupts  && sleep 1 &&  cat /proc/interrupts
 */
#include <linux/interrupt.h>
#include <linux/printk.h>
#include <linux/irq.h>
#include <linux/clockchips.h>
#include <linux/init.h>
#include <linux/io.h> /* readl/writel */
#include "../driverlib/tcu.h"
#include "../driverlib/intc.h"
#include "../inc/hw_tcu.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_types.h"

#if 0
static void pwm0_fifomode1_init(void)
{
    /* GPIO PE0 configure gpio PE0 as PWM function pin */
    HWREG(GPIOE_BASE + GPIO_O_INTC) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_MASKC) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_PAT1C) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_PAT0C) |= 0x00000001;

    /* 1. set TCSR, please keep PWM_EN to 0. */
    TCUPWMDisable(TCU_BASE, TCU_TIMER0);

    /* 2. set TCUMOD to 0x0000_0001 */
    TCUModeSet(TCU_BASE, TCU_TIMER0, TCU_MODE_FIFO);

    /* 3. push data into fifo, through write data in TFWD. fifo's depth is 16, do not over flow
          do not push data like 0x0000_XXXX, or 0xXXXX_0000 X: nonzero value
     */
    HWREG(TCU_BASE + TCU_O_FWD0) = 0xF0008000;

    /* 4. set the TCSR.PWM_EN to 1 */
    TCUPWMEnable(TCU_BASE, TCU_TIMER0);

    /* 5. set the TESR to enable the TCNT */
    TCUCounterEnable(TCU_BASE, TCU_TIMER0);

    /* a. set clock prescale and clock input */
    TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKPRESCALE_1024);
    TCUClockInputSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(TCU_BASE, TCU_TIMER0);

    /* 6. push some data into fifo is optional. Please insure the TFIFOSR. fifo_w_flag is 0, before push
          data into fifo. and do not over flow. do not push data like 0x0000_XXXX, or 0xXXXX_0000
     */
    if ((HWREG(TCU_BASE + TCU_O_FIFOSR0) & TCU_FIFOSR0_FIFOWFLAG) != TCU_FIFOSR0_FIFOWFLAG) {
        HWREG(TCU_BASE + TCU_O_FWD0) = 0xF0008000;
    }

    /* 7. when TFR.fifo_empty_flag is 1, symbol the fifo is empty */
    if ((HWREG(TCU_BASE + TCU_O_FR) & TCU_FR_FIFOEFLAG0) == TCU_FR_FIFOEFLAG0) {
        printk("FIFO mode1, the fifo is empty\n");
    }
}

void pwm0_fifomode1_again(void)
{
    /* 8. when fifo match finished ,if you want to do it again, please stop the TCNT count */
    TCUCounterDisable(TCU_BASE, TCU_TIMER0);

    /* 9. set TCSR,please keep PWM_EN to 0 */
    TCUPWMDisable(TCU_BASE, TCU_TIMER0);

    /* 10. set TFCR to clr the flag */
    TCUComparisonMatchFlagClear(TCU_BASE, TCU_TIMER0, TCU_FLAGTYPE_FIFOEMPTY);

    /* 11. set TCUMOD.fifo_clr to 1 */
    HWREG(TCU_BASE + TCU_O_MODE0) |= TCU_MODE0_FIFOCLR;

    /* 12. set TCUMOD.fifo_clr to 0 */
    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_FIFOCLR;

    /* 2. set TCUMOD to 0x0000_0001 */
    TCUModeSet(TCU_BASE, TCU_TIMER0, TCU_MODE_FIFO);

    /* 3. push data into fifo, through write data in TFWD. fifo's depth is 16, do not over flow
          do not push data like 0x0000_XXXX, or 0xXXXX_0000 X: nonzero value
     */
    HWREG(TCU_BASE + TCU_O_FWD0) = 0xF0008000;

    /* 4. set the TCSR.PWM_EN to 1 */
    TCUPWMEnable(TCU_BASE, TCU_TIMER0);

    /* 5. set the TESR to enable the TCNT */
    TCUCounterEnable(TCU_BASE, TCU_TIMER0);

    /* a. set clock prescale and clock input */
    TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKPRESCALE_1024);
    TCUClockInputSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(TCU_BASE, TCU_TIMER0);

    /* 6. push some data into fifo is optional. Please insure the TFIFOSR. fifo_w_flag is 0, before push
          data into fifo. and do not over flow. do not push data like 0x0000_XXXX, or 0xXXXX_0000
     */
    if ((HWREG(TCU_BASE + TCU_O_FIFOSR0) & TCU_FIFOSR0_FIFOWFLAG) != TCU_FIFOSR0_FIFOWFLAG) {
        HWREG(TCU_BASE + TCU_O_FWD0) = 0xF0008000;
    }

    /* 7. when TFR.fifo_empty_flag is 1, symbol the fifo is empty */
    if ((HWREG(TCU_BASE + TCU_O_FR) & TCU_FR_FIFOEFLAG0) == TCU_FR_FIFOEFLAG0) {
        printk("FIFO mode1, the fifo is empty\n");
    }
}
#endif
#if 1
static void pwm0_fifomode2_init(void)
{
    unsigned int cyc_timer, cyc_num;

    /* GPIO PE0 configure gpio PE0 as PWM function pin */
    HWREG(GPIOE_BASE + GPIO_O_INTC) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_MASKC) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_PAT1C) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_PAT0C) |= 0x00000001;

    TCUPWMInitialOutputLevelSet(TCU_BASE, TCU_TIMER0, TCU_PWMINITOUTPUT_LOW);
    TCUPWMShutdown(TCU_BASE, TCU_TIMER0, TCU_SHUTDOWN_GRACEFUL);

    /* 1. set the TCSR, Please keep PWM_EN to 0 */
    TCUPWMDisable(TCU_BASE, TCU_TIMER0);

    /* 2. set TCUMOD to 0x0000_0001. */
    TCUModeSet(TCU_BASE, TCU_TIMER0, TCU_MODE_FIFO);

    /* 3. push data into fifo, through write data in TFWD. fifo`s depth is 16, do not over flow */
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;

    /* 4. set TCUMOD */
    TCUFIFOModeSet(TCU_BASE, TCU_TIMER0, TCU_FIFOMODE_FIFO2);

    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_CYCNUM;
//    HWREG(TCU_BASE + TCU_O_MODE0) |= 0x3F << 10;
    HWREG(TCU_BASE + TCU_O_MODE0) |= 0x2 << 10;
    HWREG(TCU_BASE + TCU_O_MODE0) |= TCU_MODE0_FIFONUMST;
    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_FIFONUM;
    HWREG(TCU_BASE + TCU_O_MODE0) |= 0x10 << 4; // FIFO num = 16

    /* 5. set the TCSR.PWM_EN to 1. */
    TCUPWMEnable(TCU_BASE, TCU_TIMER0);

    /* a. set clock prescale and clock input */
    TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKPRESCALE_256);
    TCUClockInputSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(TCU_BASE, TCU_TIMER0);

    /* 6. set the TESR to enable the TCNT */
    TCUCounterEnable(TCU_BASE, TCU_TIMER0);

    /* 7. if step 4 has been done, do not push data into fifo */
    /* 8. when TFIFOSR. cyc_timer == TCUMOD. cyc_num , It symbol finished. */
    cyc_timer = (HWREG(TCU_BASE + TCU_O_FIFOSR0) & TCU_FIFOSR0_CYCTIMER) >> 6;
    cyc_num = (HWREG(TCU_BASE + TCU_O_MODE0) & TCU_MODE0_CYCNUM) >> 10;
    printk("cyc_timer:%d\n", cyc_timer);
    printk("cyc_num:%d\n", cyc_num);
    TCURegisterDump(TCU_BASE, printk);
}
#endif
void pwm0_fifomode2_again(void)
{
    unsigned int ret;

    /* 9. if you want do again, please stop the TCNT count */
    ret = TCUCounterDisable(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    /* 10. set the TCSR, Please keep PWM_EN to 0 */
    ret = TCUPWMDisable(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    /* 11. set TFCR to clr the flag */
    ret = TCUComparisonMatchFlagClear(TCU_BASE, TCU_TIMER0, TCU_FLAGTYPE_FIFOEMPTY);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    /* 12. set TCUMOD.fifo_clr to 1 */
    HWREG(TCU_BASE + TCU_O_MODE0) |= TCU_MODE0_FIFOCLR;

    /* 13. set TCUMOD.fifo_clr to 0 */
    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_FIFOCLR;

    /* 14. reset TCU_MODE0_CYCST/TCU_MODE0_FIFONUMST, must be done
     *     or the again not works, since TCNT not count any more
     */
    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_CYCST;
    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_FIFONUMST;

    /* 2. set TCUMOD to 0x0000_0001. */
    ret = TCUModeSet(TCU_BASE, TCU_TIMER0, TCU_MODE_FIFO);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    /* 3. push data into fifo, through write data in TFWD. fifo`s depth is 16, do not over flow */
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;
    HWREG(TCU_BASE + TCU_O_FWD0) = 0x20001000;

    /* 4. set TCUMOD */
    ret = TCUFIFOModeSet(TCU_BASE, TCU_TIMER0, TCU_FIFOMODE_FIFO2);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_CYCNUM;
    HWREG(TCU_BASE + TCU_O_MODE0) |= 0x2 << 10;
    HWREG(TCU_BASE + TCU_O_MODE0) |= TCU_MODE0_FIFONUMST;
    HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_FIFONUM;
    HWREG(TCU_BASE + TCU_O_MODE0) |= 0x10 << 4; // FIFO num = 16

    /* 5. set the TCSR.PWM_EN to 1. */
    ret = TCUPWMEnable(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    /* 6. set the TESR to enable the TCNT */
    ret = TCUCounterEnable(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d ret:%d\n", __func__, __LINE__, ret);

    /* 7. if step 4 has been done, do not push data into fifo */
    /* 8. when TFIFOSR. cyc_timer == TCUMOD. cyc_num , It symbol finished. */
    TCURegisterDump(TCU_BASE, printk);
}

#if 0
static void pwm0_init(void)
{
    unsigned int result = 0;
    /* GPIO PE0 configure gpio PE0 as PWM function pin */
    HWREG(GPIOE_BASE + GPIO_O_INTC) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_MASKC) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_PAT1C) |= 0x00000001;
    HWREG(GPIOE_BASE + GPIO_O_PAT0C) |= 0x00000001;

    /* 1. Initial the configuration. */
    /* a. Writing TCSR.INITL to initialize PWM output level. */
    result = TCUPWMInitialOutputLevelSet(TCU_BASE, TCU_TIMER0, TCU_PWMINITOUTPUT_LOW);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);

    /* b. Writing TCSR.SD to setting the shutdown mode(abrupt or graceful mode) */
    result = TCUPWMShutdown(TCU_BASE, TCU_TIMER0, TCU_SHUTDOWN_GRACEFUL);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);

    /* c. Writing TCSR.PRESCALE to set TCNT count clock frequency */
    result = TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKPRESCALE_1024);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);

    /* d. Setting TCNT, TDHR and TDFR */
    /* if TDHR = 0, TDFR = 1, duty = 50% */
    TCUCounterSet(TCU_BASE, TCU_TIMER0, 0x0000);
    TCUDataHalfSet(TCU_BASE, TCU_TIMER0, 0x000E);
    TCUDataFullSet(TCU_BASE, TCU_TIMER0, 0x000F);

    /* 2. Enable the clock */
    /* a. Writing TCSR.PWM_EN to set whether enable PWM */
    result = TCUPWMEnable(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);

    /* b. Writing TCSR.EXT_EN, TCSR.RTC_EN or TCSR.PCK_EN to 1 to select the input
          clock and enable the input clock. */
    result = TCUClockInputSet(TCU_BASE, TCU_TIMER0, TCU_CLOCKINPUT_EXTAL);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);
    result = TCUClockSupply(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);

    /* 3. Enable the counter */
    result = TCUCounterEnable(TCU_BASE, TCU_TIMER0);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);
}
#endif

static void tcu_timer5_init(void)
{
    unsigned int timer5_irq = 0;
    printk("%s line:%d\n", __func__, __LINE__);

    /* 1. Initial the configuration */
    /* a. Writing TCSR.INITL to initialize PWM output level. */
    TCUPWMInitialOutputLevelSet(TCU_BASE, TCU_TIMER5, TCU_PWMINITOUTPUT_LOW);

    /* b. Set shutdown mode */
    TCUPWMShutdown(TCU_BASE, TCU_TIMER5, TCU_SHUTDOWN_GRACEFUL);

    /* c. Set prescale */
    TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER5, TCU_CLOCKPRESCALE_4);

    /* Setting TCNT, TDHR and TDFR */
    TCUCounterSet(TCU_BASE, TCU_TIMER5, 0x0000);
    TCUDataHalfSet(TCU_BASE, TCU_TIMER5, 0xFFFF); // Half > Full, we not use half comparison here
    TCUDataFullSet(TCU_BASE, TCU_TIMER5, 60000 - 1); // 24000000/4/HZ = 60000

    /* 2. Enable the clock */
    /* a. Writing TCSR.PWM_EN to set whether enable PWM or disable PWM */
    TCUPWMDisable(TCU_BASE, TCU_TIMER5);

    /* b. Set input clock */
    TCUClockInputSet(TCU_BASE, TCU_TIMER5, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(TCU_BASE, TCU_TIMER5);

    /* 3. Enable the counter. */
    TCUCounterEnable(TCU_BASE, TCU_TIMER5);
    printk("file:%s line:%d\n", __FILE__, __LINE__);

    /* 4. Mask non-full interrupt of timer 5, unmask full interrupt of timer 5 */
    TCUInterruptMask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_HALF);
    TCUInterruptMask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_FIFOEMPTY);
    TCUInterruptMask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_FIFO);
    TCUInterruptUnmask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_FULL);
    timer5_irq = TCUIntNumberGet(TCU_TIMER5);
    if (timer5_irq == -1) {
        printk("TCUIntNumberGet() failed\n");
    } else {
        INTCInterruptEnable(timer5_irq);
    }

}

static void tcu_timer5_start(void)
{
//    TCUCounterEnable(TCU_BASE, TCU_TIMER5);
    TCUClockSupply(TCU_BASE, TCU_TIMER5);
}

static void tcu_timer5_stop(void)
{
//    TCUCounterDisable(TCU_BASE, TCU_TIMER5);
    TCUClockNotSupply(TCU_BASE, TCU_TIMER5);
}

static void tcu_timer5_count_reset(void)
{
    tcu_timer5_stop();
    HWREGH(TCU_BASE + TCU_O_CNT5) = 0x0000;
    tcu_timer5_start();
}

static irqreturn_t
tcu_timer5_interrupt_handle(int irq, void *dev_id)
{
    struct clock_event_device *ced = dev_id;

    if (HWREG(TCU_BASE + TCU_O_FR) & TCU_FR_FFLAG5) {
        HWREG(TCU_BASE + TCU_O_FCR) |= TCU_FCR_FFCL5;
// TCNT will become zero, after match full data register
//        TCUCounterSet(TCU_BASE, TCU_TIMER5, 0x0000);
    }

    //printk("%s line:%d\n", __func__, __LINE__);

    ced->event_handler(ced);

    return IRQ_HANDLED;
}

static void
tcu_timer5_set_mode(enum clock_event_mode mode, struct clock_event_device *ced)
{
    switch(mode) {
    case CLOCK_EVT_MODE_UNUSED:
    case CLOCK_EVT_MODE_SHUTDOWN:
        break;
    case CLOCK_EVT_MODE_PERIODIC:
        tcu_timer5_count_reset();
        break;
    case CLOCK_EVT_MODE_ONESHOT:
    case CLOCK_EVT_MODE_RESUME:
        break;
    default:
        printk("%s line:%d\n", __func__, __LINE__);
        break;
    }
    (void)ced;
}

static int
tcu_timer5_set_next_event(unsigned long delta, struct clock_event_device *ced)
{
    (void)delta;
    (void)ced;

    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

void __init tcu_timer5_clockevent_init(void)
{
    static struct irqaction action;
    static struct clock_event_device ced;

//    pwm0_fifomode1_init();
    tcu_timer5_init();
    pwm0_fifomode2_init();

    action.name = "'TCU Timer5 IRQ'";
    action.handler = tcu_timer5_interrupt_handle;
    action.flags = IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL;
    action.dev_id = (void *)&ced;
    setup_irq(IRQ_NO_TCU1, &action);

    ced.name = "'TCU Timer5 Clock Event'";
    ced.features = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC;
    ced.irq = IRQ_NO_TCU1;
    ced.rating = 400,
    ced.set_mode = tcu_timer5_set_mode;
    ced.set_next_event = tcu_timer5_set_next_event;
    clockevents_config_and_register(&ced, 1, 4, 65536);
}
