#include <asm/cpu-features.h>
#include <asm/idle.h>
#include <asm/mipsregs.h>
#include <asm/cpu.h> /* CPU_JZRISC */
#include <asm/cpu-info.h>

#include <linux/printk.h>
#include <linux/smp.h> /* cpu_has_fpu */
#include <linux/init.h> /* __init */


#include "unittest.h"

int __init cpu_has_test(void)
{
	unittestAssert(cpu_has_vint == 524288);
	unittestAssert(cpu_has_veic == 0);
	unittestAssert(cpu_has_clo_clz == 96);
	unittestAssert(cpu_has_dsp == 0);
	unittestAssert(cpu_has_divec == 512);
	unittestAssert(cpu_has_mips_r2 == 64);
	unittestAssert(cpu_has_ejtag == 16384);
	unittestAssert(cpu_has_watch == 256);
	unittestAssert(cpu_has_fpu == 32);
	unittestAssert(cpu_has_nofpuex == 0);
	unittestAssert(cpu_has_vce == 0);
	unittestAssert(using_rollback_handler() == 0);
	unittestAssert(CPU_JZRISC == 61);
	
	unittestPrintk("cpu_has_mipsmt = %ld\n", cpu_has_mipsmt);
	return 0;
}

module_init(cpu_has_test);
