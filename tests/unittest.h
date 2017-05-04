#ifndef UNITTEST_H
#define UNITTEST_H

#include <linux/printk.h> /* printk/KERN_ERR */
#include <linux/stringify.h> /* __stringify */

#define unittestPrintk(...) do{ printk(KERN_ERR "file: " __FILE__ " line:" __stringify(__LINE__) " "__VA_ARGS__); }while(0)
#define unittestAssert(expr)                                            \
do {                                                                    \
	if(unlikely(!(expr))) {				                \
		printk(KERN_ERR "Assertion failed! %s,%s,%s,line=%d\n",	\
		#expr, __FILE__, __func__, __LINE__);		        \
	}                                                               \
} while(0)
#endif /* UNITTEST_H */
