/*
 * invoked in arch/mips/kernel/time.c:time_init()
 */

#include "../inc/hw_ost.h"
#include <linux/init.h>
void __init plat_time_init(void)
{
	ost_time_init();
}

