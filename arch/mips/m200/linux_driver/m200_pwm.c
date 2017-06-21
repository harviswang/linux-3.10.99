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
#include <linux/pwm.h>
#include "../inc/hw_memmap.h"
#include "../driverlib/pwm.h"

static int m200_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
                           int duty_ns, int period_ns)
{
/*
    // Below are test code for PWMFIFOMode1Configure/PWMFIFOMode2Configure
    unsigned long ulDuty[16];
    unsigned long ulPeriod[16];
    int i;

    for (i = 0; i < 16; i++) {
        ulDuty[i] = 1000 - duty_ns/1000;
        ulPeriod[i] = period_ns/1000;
    }

    PWMFIFOMode2Configure(PWM_BASE, pwm->hwpwm, ulDuty, ulPeriod, sizeof(ulDuty)/sizeof(ulDuty[0]), 6);
    //PWMFIFOMode1Configure(PWM_BASE, pwm->hwpwm, ulDuty, ulPeriod, sizeof(ulDuty)/sizeof(ulDuty[0]));
*/
    printk("%s line:%d hwpwm:%d duty_ns:%d, period_ns:%d\n", __func__, __LINE__, pwm->hwpwm, duty_ns, period_ns);
    PWMConfigure(PWM_BASE, pwm->hwpwm, 1000 - duty_ns/1000, period_ns/1000);

    return 0;
}

static int m200_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    PWMEnable(PWM_BASE, pwm->hwpwm);
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static void m200_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    printk("%s line:%d\n", __func__, __LINE__);
    //PWMDisable(PWM_BASE, pwm->hwpwm);
}

static int m200_pwm_probe(struct platform_device *pdev)
{
    int ret;
    static struct pwm_chip pwmchip;
    static struct pwm_ops m200_pwm_ops;

    printk("%s line:%d\n", __func__, __LINE__);

    m200_pwm_ops.config = m200_pwm_config;
    m200_pwm_ops.enable = m200_pwm_enable;
    m200_pwm_ops.disable= m200_pwm_disable;
    m200_pwm_ops.owner  = THIS_MODULE;

    pwmchip.dev = &pdev->dev;
    pwmchip.ops = &m200_pwm_ops;
    pwmchip.base = -1;
    pwmchip.npwm = 8;

    platform_set_drvdata(pdev, &pwmchip);
    
    ret = pwmchip_add(&pwmchip);
    if (ret < 0) {
        dev_err(&pdev->dev, "pwmchip_add() failed: %d\n", ret);
    }

    return ret;
}

static int m200_pwm_remove(struct platform_device *pdev)
{
    struct pwm_chip *chip = platform_get_drvdata(pdev);

    if (!chip) {
        dev_err(&pdev->dev, "platform_get_drvdata() failed\n");
        return -ENODEV;
    }

    return pwmchip_remove(chip);
}

static struct platform_driver m200_pwm_driver = {
    .driver = {
        .name = "m200_pwm",
    },

    .probe  = m200_pwm_probe,
    .remove = m200_pwm_remove,
};

module_platform_driver(m200_pwm_driver);

MODULE_DESCRIPTION("M200 PWM Driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Harvis Wang");

