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

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/pwm_backlight.h>
#include <linux/err.h> /* IS_ERR */
#include <linux/pwm.h>

static struct platform_pwm_backlight_data data = {
    .pwm_id = 0,
    .max_brightness = 255,
    .dft_brightness = 102,
    .pwm_period_ns = 1000000,
};
 
static struct platform_device pwm_backlight_device = {
    .name = "pwm-backlight",
    .id = 0,
    .dev = {
        .platform_data = &data,
    },
};

/* TODO: refactor it by Device Tree */
static struct pwm_lookup board_pwm_lookup[] = {
    PWM_LOOKUP("m200_pwm", 0, "pwm-backlight.0", NULL),
};

static int __init m200_pwm_init(void)
{
    int err;
    struct platform_device *pdev;

    /* TODO: refactor it by Device Tree */
    pwm_add_table(board_pwm_lookup, ARRAY_SIZE(board_pwm_lookup));
    pdev = platform_device_register_simple("m200_pwm", -1, NULL, 0);
    if (IS_ERR(pdev)) {
        err = PTR_ERR(pdev);
        dev_err(&pdev->dev, "platform_device_register_simple() failed: %d\n", err);
        return err;
    } 

    printk("%s line:%d\n", __func__, __LINE__);
    err = platform_device_register(&pwm_backlight_device);
    if (err) {
        dev_err(&pwm_backlight_device.dev, "platform_device_register failed\n");
    }
    

    return err;
}

module_init(m200_pwm_init);

MODULE_DESCRIPTION("PWM based Backlight Device");
MODULE_LICENSE("Dual BSD/GPL");

