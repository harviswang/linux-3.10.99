here include linux mips interface
they are:

void plat_irq_dispatch(void);
const char *get_system_type(void);
void prom_putchar(char c);
void arch_init_irq(void);
void prom_init(void);
int plat_mem_setup(void);
void plat_time_init(void);
void prom_free_prom_memory(void);

