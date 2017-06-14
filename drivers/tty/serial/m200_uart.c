/*-
 * Copyright (c) 2016 Harvis Wang.
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

#if defined(CONFIG_SERIAL_M200_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/console.h>
#include <linux/tty_flip.h> /* tty_flip_buffer_flush */
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h> /* devm_kzalloc */
#include <linux/compiler.h> /* unlikely */
#include <linux/interrupt.h> /* request_irq */
#include <linux/spinlock.h> /* spin_lock_irqsave */
#include "../../../arch/mips/m200/driverlib/uart.h" /* UARTTxEmpty */
#include "../../../arch/mips/m200/driverlib/intc.h" /* IntEnable */

#define DEBUG() printk("%s %s line:%d\n", __FILE__, __func__, __LINE__)

/*
 * We wrap our port structure around the generic uart_port.
 */
struct m200_uart_port {
	struct uart_port	uart; /* must be first, for type conversion */
	char                    name[16];
};

#define UART_TO_M200(uart_port) ((struct m200_uart_port *) uart_port)

static unsigned int m200_uart_tx_empty(struct uart_port *port)
{
	return UARTTxEmpty(port->iobase);
}

/*
 * modem control
 */
static void m200_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	if (mctrl & TIOCM_DTR) {
		/* M200 not support DTR, only support RTS/CTS */
	}

	/* Modem RTS */
	if (mctrl & TIOCM_RTS) {
		UARTModemControlSet(port->iobase, UART_MODEM_RTS);
	}

	/* Modem loop back */
	if (mctrl & TIOCM_LOOP) {
		UARTModemControlSet(port->iobase, UART_MODEM_LOOPBACK);
	}
}

static unsigned int m200_uart_get_mctrl(struct uart_port *port)
{
	unsigned int mctrl = UARTModemControlGet(port->iobase);
	 
	switch (mctrl) {
		case UART_MODEM_RTS:
			return TIOCM_RTS;
		case UART_MODEM_LOOPBACK:
			return TIOCM_RTS;
		default:
			return 0;
	}
}

static void m200_uart_stop_tx(struct uart_port *port)
{
	UARTTxStop(port->iobase);
}

static void m200_uart_start_tx(struct uart_port *port)
{
	UARTTxStart(port->iobase);
}

static void m200_uart_stop_rx(struct uart_port *port)
{
	UARTRxStop(port->iobase);
}

/* TODO */
static void m200_uart_flush_buffer(struct uart_port *port)
{
	/*
	static int cnt = 0;
	if (!cnt) {
		dump_stack();
		cnt++;
	}
	*/
}

/* Enable modem status interrupt */
static void m200_uart_enable_ms(struct uart_port *port)
{
	DEBUG();
	UARTModemStatusInterruptEnable(port->iobase);
}

static void m200_uart_break_ctl(struct uart_port *port, int ctl)
{
	DEBUG();
}

static irqreturn_t m200_uart_irq_handler(int irq_no, void *dev)
{
	
	struct uart_port *port = dev;
	unsigned long flags;
	unsigned int status;

	spin_lock_irqsave(&port->lock, flags);

	while (1) {
		status = UARTLsrGet(port->iobase);

		if (UARTIsTxInterrupt(port->iobase, status)) {
			struct circ_buf *xmit = &(port->state->xmit);
			static int nonblock_cnt = 0, block_cnt = 0;
			//DEBUG();
			while (!uart_circ_empty(xmit)) {
				if (UARTCharPutNonBlocking(port->iobase, xmit->buf[xmit->tail])) {
					xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
					port->icount.tx++;
					nonblock_cnt++;
				} else {
					/* TODO */
					block_cnt++;
					continue;
				}
			}
	
	       		if (uart_circ_empty(xmit)) {
				port->ops->stop_tx(port);
				//printk("nonblock_cnt:%d\n", nonblock_cnt);
				//printk("block_cnt:%d\n", block_cnt);
				//DEBUG();
			}
		}
		
		if (UARTIsRxInterrupt(port->iobase, status)) {
			struct tty_port *tport = &(port->state->port);
			//DEBUG();
			do {
				/* After read all, UARTIsRxInterrupt() return 0 */
				unsigned char ch;
				unsigned int flag;

				ch = UARTCharGet(port->iobase);
				flag = TTY_NORMAL;
				port->icount.rx++;
				if (UARTIsRxBreakInterrupt(port->iobase, status)) {
					port->icount.brk++;
					uart_handle_break(port);
				} else if (status & UART_RXERROR_PARITY) {
					port->icount.parity++;
				} else if (status & UART_RXERROR_OVERRUN) {
					port->icount.overrun++;
				} else if (status & UART_RXERROR_FRAMIMG) {
					port->icount.frame++;
				}

				status &= port->read_status_mask;
				if (status & UART_RXERROR_PARITY) {
					flag = TTY_PARITY;
				} else if (status & UART_RXERROR_OVERRUN) {
					flag = TTY_OVERRUN;
				} else if (status & UART_RXERROR_FRAMIMG) {
					flag = TTY_FRAME;
				}
				
				uart_insert_char(port, status, UART_RXERROR_OVERRUN, ch, flag);
				//printk("%c", ch);
				status = UARTLsrGet(port->iobase);
			} while (UARTIsRxInterrupt(port->iobase, status));
			tty_flip_buffer_push(tport);
			//UARTRegisterDump(port->iobase, printk);
		}
		break;
	}
	spin_unlock_irqrestore(&port->lock, flags);
	
	return IRQ_HANDLED;
}

/*
 * uart startup
 * open uart clock gate
 * enable uart sdram gate
 */
static int m200_uart_startup(struct uart_port *port)
{
	int err = 0;
	struct m200_uart_port *m200_port = UART_TO_M200(port);
	
	snprintf(m200_port->name, sizeof(m200_port->name),
		"'UART %d'", port->line);
	
	port->irq = UARTIntNumberGet(port->iobase);
	err = request_irq(port->irq, m200_uart_irq_handler, port->irqflags, 
	                  m200_port->name, port);
	if (err) {
		dev_err(port->dev, "%s:%d request_irq() failed!\n", __FILE__, __LINE__);
	}


	return err;
}

static void m200_uart_shutdown(struct uart_port *port)
{
	DEBUG();
}

static void 
m200_uart_set_termios(struct uart_port *port, struct ktermios *termios,
		      struct ktermios *old)
{
	unsigned long flags;
	unsigned long config = 0;
	unsigned int baud;

	spin_lock_irqsave(&port->lock, flags);
	
	/* calculate and set baud rate */
	baud = uart_get_baud_rate(port, termios, old, 0, 4000000); /* [0, 4M] */

	/* set baud rate */
	if (tty_termios_baud_rate(termios)) {
		tty_termios_encode_baud_rate(termios, baud, baud);
	}
	
	/* calculate parity */
	if (termios->c_cflag & PARENB) {
		/* ODD parity */
		if (termios->c_cflag & PARODD) {
			config |= UART_CONFIG_PAR_ODD;
		} else {
		/* EVEN prity */
			config |= UART_CONFIG_PAR_EVEN;
		}

		/* Sticky prity */
		if (termios->c_cflag & CMSPAR) {
			config |= UART_CONFIG_PAR_STICKY;
		}
	}
	
	/* calculate bits per char */
	switch(termios->c_cflag & CSIZE) {
	case CS5:
		config |= UART_CONFIG_WLEN_5;
		break;
	case CS6:
		config |= UART_CONFIG_WLEN_6;
		break;
	case CS7:
		config |= UART_CONFIG_WLEN_7;
		break;
	case CS8:
	default:
		config |= UART_CONFIG_WLEN_8;
		break;
	}
	
	/* calculate stop bits */
	if (termios->c_cflag & CSTOPB) {
		/* two stop bits */
		config |= UART_CONFIG_STOP_TWO;
	} else {
		/* one stop bit */
		config |= UART_CONFIG_STOP_ONE;
	}
	
	/* calculate and set hardware flow control */
	if (termios->c_cflag & CRTSCTS) {
		/* RTS(request to send) CTS(clear to send) */
	}
	
	/* Configure status bits to ignore based on termios flags */
	if (termios->c_iflag & INPCK) {
		
	}
	if (termios->c_iflag & (BRKINT | PARMRK)) {
		
	}
	
	/* Disable UART */
	UARTDisable(port->iobase);

	/* Config baudrate TODO */
	UARTConfigSetExpClk(port->iobase, 24000000, baud, config);

	/* Interrupt set */
	UARTRxStart(port->iobase);

	/* Enable UART */
	UARTEnable(port->iobase);

	/* Enablue UART module interrupt  */ 
	INTCInterruptEnable(port->irq);

	//DEBUG();
	//UARTRegisterDump(port->iobase, printk);

	uart_update_timeout(port, termios->c_cflag, baud);
	spin_unlock_irqrestore(&port->lock, flags);
}

static const char* m200_uart_type(struct uart_port *port)
{
	return "M200_UART";
}

static int m200_uart_request_port(struct uart_port *port)
{
	int err = 0;
	DEBUG();
	return err;	
}

static void m200_uart_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_16550;
	}
	DEBUG();
}

static void m200_uart_release_port(struct uart_port *port)
{
	DEBUG();
}

static struct uart_ops m200_uart_ops = {
	.tx_empty  = m200_uart_tx_empty,
	.set_mctrl = m200_uart_set_mctrl,
	.get_mctrl = m200_uart_get_mctrl,
	.stop_tx   = m200_uart_stop_tx,
	.start_tx  = m200_uart_start_tx,
	.stop_rx   = m200_uart_stop_rx,
	.flush_buffer = m200_uart_flush_buffer,
	.enable_ms = m200_uart_enable_ms,
	.break_ctl = m200_uart_break_ctl,
	.startup   = m200_uart_startup,
	.shutdown  = m200_uart_shutdown,
	.set_termios  = m200_uart_set_termios,
	.type         = m200_uart_type,
	.request_port = m200_uart_request_port,
	.config_port  = m200_uart_config_port,
	.release_port = m200_uart_release_port,
};

static struct m200_uart_port m200_uart_ports[] = {
	{     /* uart[0] */
		.uart = {
			.iotype = UPIO_MEM,
			.iobase = -1,
			.ops    = &m200_uart_ops,
			.flags  = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line   = 0,
			.type   = PORT_16550,
		},
	},
	{     /* uart[1] */
		.uart = {
			.iotype = UPIO_MEM,
			.iobase = -1,
			.ops    = &m200_uart_ops,
			.flags  = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line   = 1,
			.type   = PORT_16550,
		},
	},
	{     /* uart[2] */
		.uart = {
			.iotype = UPIO_MEM,
			.iobase = -1,
			.ops    = &m200_uart_ops,
			.flags  = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line   = 2,
			.type   = PORT_16550,
		},
	},
	{     /* uart[3] */
		.uart = {
			.iotype = UPIO_MEM,
			.iobase = 0xB0033000, /* TODO */
			.ops    = &m200_uart_ops,
			.flags  = UPF_BOOT_AUTOCONF | UPF_FIXED_TYPE | UPF_SPD_VHI,
			.fifosize = 64,
			.line   = 3,
			.type   = PORT_16550,
		},
	},
	{     /* uart[4] */
		.uart = {
			.iotype = UPIO_MEM,
			.iobase = -1,
			.ops    = &m200_uart_ops,
			.flags  = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line   = 4,
			.type   = PORT_16550,
		},
	},
};

#define M200_UART_NR ARRAY_SIZE(m200_uart_ports)

static inline struct uart_port *get_port_from_line(unsigned int line)
{
	return &m200_uart_ports[line].uart;
}

#ifdef CONFIG_SERIAL_M200_CONSOLE
static void m200_console_putchar(struct uart_port *port, int ch)
{
	while(UARTCharPutNonBlocking(port->iobase, ch) == false) {
		(void)ch;
	}
}

static void
m200_console_write(struct console *co, const char *s, unsigned count)
{
	struct uart_port *port = get_port_from_line(co->index);

	BUG_ON(co->index < 0 || co->index > M200_UART_NR);

	spin_lock(&port->lock);
	uart_console_write(port, s, count, m200_console_putchar);
	spin_unlock(&port->lock);
}

static int m200_console_setup(struct console *co, char *options)
{
	struct uart_port *port = get_port_from_line(co->index);
	/* default setting: 115200 8N1 */
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';
	
	if (unlikely(co->index >= M200_UART_NR || co->index < 0)) {
		return -ENXIO;
	}
	
	if (options) {
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	}

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct uart_driver m200_uart_driver;

static struct console m200_console = {
	.name = "ttyS",
	.write  = m200_console_write,
	.device = uart_console_device,
	.setup = m200_console_setup,
	.flags = CON_PRINTBUFFER,
	.index = -1,
	.data  = &m200_uart_driver,
};

#define M200_CONSOLE (&m200_console)
#else
#define M200_CONSOLE NULL
#endif

static struct uart_driver m200_uart_driver = {
	.owner = THIS_MODULE,
	.driver_name = "m200_uart",
	.dev_name = "ttyS",
	.major = TTY_MAJOR,
	.minor = 64,
	.nr    = M200_UART_NR,
	.cons  = M200_CONSOLE,
};

static struct platform_device m200_uart_platform_device = {
	.id = 3,
	.name = "m200_uart",
	.num_resources = 0,
};

static int m200_uart_platform_driver_probe(struct platform_device *pdev)
{
	int err = 0;
	struct uart_port *port;
	
	printk("%s %s %d pdev->id:%d\n", __FILE__, __func__, __LINE__, pdev->id);
	if (unlikely(pdev->id >= M200_UART_NR || pdev->id < 0)) {
		return -ENXIO;
	}
	
	port = get_port_from_line(pdev->id);
	DEBUG();
	port->dev = &pdev->dev; /* parent device */

	platform_set_drvdata(pdev, port);
	
	err = uart_add_one_port(&m200_uart_driver, port);
	if (err) {
		dev_err(&pdev->dev, "Failed to add uart port, error:%d\n", err);
		return err;
	}

	DEBUG();
	return err;
}

static int m200_uart_platform_driver_remove(struct platform_device *pdev)
{
	int err = 0;
	struct m200_uart_port *mup = platform_get_drvdata(pdev);
	DEBUG();

	(void)mup;
	return err;
}

//static struct of_device_id m200_uart_match_table[] = {
//	{ .compatible = "m200_uart" },
//	{}
//};

static struct platform_driver m200_uart_platform_driver = {
	.probe  = m200_uart_platform_driver_probe,
	.remove = m200_uart_platform_driver_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "m200_uart",
//		.of_match_table = m200_uart_match_table,
	},
};

static int __init m200_uart_init(void)
{
	int err = 0;

	printk(KERN_INFO "Serial: M200 UART driver\n");

	err = uart_register_driver(&m200_uart_driver);
	if (err) {
		pr_err("Could not register %s driver\n", 
			m200_uart_driver.driver_name);
		return err;
	}

// 	TODO
//	err = platform_driver_probe(&m200_uart_platform_driver, m200_uart_platform_driver_probe);
//	if (err) {
//		pr_err("Uart platform driver register failed, error:%d\n", err);
//		uart_unregister_driver(&m200_uart_driver);
//		return err;
//	}

	err = platform_driver_register(&m200_uart_platform_driver);
	if (err) {
		pr_err("Uart platform driver register failed, error:%d\n", err);
		uart_unregister_driver(&m200_uart_driver);
		return err;
	}
	DEBUG();

	err = platform_device_register(&m200_uart_platform_device);
	if (err) {
		pr_err("Uart platform device register failed, error:%d\n", err);
	}

	return err;
}

static void __exit m200_uart_exit(void)
{
	DEBUG();
#ifdef CONFIG_SERIAL_M200_CONSOLE
	unregister_console(&m200_console);
#endif
	platform_driver_unregister(&m200_uart_platform_driver);
	uart_unregister_driver(&m200_uart_driver);
}

/*
 * While this can be a module, if builtin it's most likely the console
 * So let's leave module_exit but move module_init to an earlier place
 */
arch_initcall(m200_uart_init);
module_exit(m200_uart_exit);

MODULE_AUTHOR("Ingenic Semiconductor Ltd");
MODULE_DESCRIPTION("Ingenic M200 serial port driver");
MODULE_LICENSE("GPL");
