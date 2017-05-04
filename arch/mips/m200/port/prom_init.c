#include <linux/printk.h>
#include <linux/string.h>

#include <asm/addrspace.h>
#include <asm/bootinfo.h> /* fw_arg0, fw_arg1, arcs_cmdline */

static void prom_init_cmdline(void)
{
/*
	int argc = fw_arg0;
	char **argv = (char **)fw_arg1;
	int i;
	unsigned int count = COMMAND_LINE_SIZE - 1;
	char *dst = &(arcs_cmdline[0]);


	for (i = 1; i < argc && count; i++) {
		char *p = (char *) argv[i];
		while (*p && count) {
			*dst++ = *p++;
			count--;
		}
		*dst++ = ' ';
	}

	if (i > 1) {
		dst--;
	}

	*dst = '\0';
 */
	int argc = fw_arg0;
	char **argv = (char **) KSEG1ADDR(fw_arg1);
	int i;

	arcs_cmdline[0] = '\0'; /* make sure strlen(arcs_cmdline) == 0 in strlcat */

	for (i = 0; i < argc; i++) {
		char *p = (char *) KSEG1ADDR(argv[i]);

		if (CPHYSADDR(p) && *p) {
			strlcat(arcs_cmdline, p, sizeof(arcs_cmdline));
			strlcat(arcs_cmdline, " ", sizeof(arcs_cmdline));
		}
	}

	pr_notice("fw_arg0 = 0x%lx\n", fw_arg0);
	pr_notice("fw_arg1 = 0x%lx\n", fw_arg1);
	pr_notice("fw_arg2 = 0x%lx\n", fw_arg2);
	pr_notice("fw_arg3 = 0x%lx\n", fw_arg3);

	pr_notice("&fw_arg0 = %p\n", &fw_arg0);
	pr_notice("&fw_arg1 = %p\n", &fw_arg1);
	pr_notice("&fw_arg2 = %p\n", &fw_arg2);
	pr_notice("&fw_arg3 = %p\n", &fw_arg3);
}

void __init prom_init(void)
{
	pr_info("%s line:%d\n", __func__, __LINE__);
	prom_init_cmdline();
}

