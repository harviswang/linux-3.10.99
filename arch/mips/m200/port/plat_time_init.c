/*
 * invoked in arch/mips/kernel/time.c:time_init()
 */

#include <linux/init.h>
#include "../linux/ost_linux.h"
#include "../linux/tcu_linux.h"

void __init plat_time_init(void)
{
    ost_clocksource_init();
    tcu_timer5_clockevent_init();
}

