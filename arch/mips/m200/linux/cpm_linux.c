/*
 * CPM module
 */
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/printk.h>
#include <linux/slab.h>

#include "../inc/hw_memmap.h"
#include "../driverlib/cpm.h"

enum m200_pll_type {
    PLL_TYPE_APLL,
    PLL_TYPE_MPLL,
};

struct m200_clk_pll {
    struct clk_hw pll_hw; /* must be in first placement since easy type convert */
    enum m200_pll_type pll_type;
    unsigned long ulBase;
};

static int pll_clk_enable(struct clk_hw *hw)
{
    struct m200_clk_pll *pll_clk = (struct m200_clk_pll *)hw;
    int result = CPMApllEnable(pll_clk->ulBase);
    printk("%s line:%d result:%d\n", __func__, __LINE__, result);
    return 0;
}

static unsigned long pll_clk_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
    struct m200_clk_pll *pll_clk = (struct m200_clk_pll *)hw;

    switch (pll_clk->pll_type) {
    case PLL_TYPE_APLL:
        if (CPMApllIsBypassed(pll_clk->ulBase)) {
            return parent_rate;
        } else {
            return (parent_rate * CPMApllGetM(pll_clk->ulBase)) / CPMApllGetN(pll_clk->ulBase);
        }
    case PLL_TYPE_MPLL:
        return (parent_rate * CPMMpllGetM(pll_clk->ulBase)) / CPMMpllGetN(pll_clk->ulBase);
    default:
        printk("error at %s line:%d\n", __func__, __LINE__);
        return parent_rate;
    }
}

static long pll_clk_round_rate(struct clk_hw *hw, unsigned long rate, unsigned long *parent_rate)
{
    printk("%s line:%d rate:%ld parent_rate:%ld\n", __func__, __LINE__, rate, *parent_rate);
    return rate;
}

static int pll_clk_set_rate(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate)
{
    struct m200_clk_pll *pll_clk = (struct m200_clk_pll *)hw;
    unsigned long ulM, ulMFraction, ulN, ulNFraction;
    int result = CPMPLLCalculateMNFraction(rate, parent_rate, &ulM, &ulMFraction, &ulN, &ulNFraction);
    printk("%s line:%d rate:%ld parent_rate:%ld ulM:%ld ulMFraction:0x%lx ulN:%ld ulNFraction:0x%lx\n",
        __func__, __LINE__, rate, parent_rate, ulM, ulMFraction, ulN, ulNFraction);

    switch (pll_clk->pll_type) {
    case PLL_TYPE_APLL:
        CPMApllSetM(pll_clk->ulBase, ulM);
        CPMApllSetN(pll_clk->ulBase, ulN);
        CPMApllSetNFraction(pll_clk->ulBase, ulNFraction);
    case PLL_TYPE_MPLL:
        CPMMpllSetM(pll_clk->ulBase, ulM);
        CPMMpllSetN(pll_clk->ulBase, ulN);
        CPMMpllSetNFraction(pll_clk->ulBase, ulNFraction);
    default:
        break;
    }
    return result ? 0 : -EINVAL;
}

static const struct clk_ops pll_clk_ops = {
    .enable      = pll_clk_enable,
    .recalc_rate = pll_clk_recalc_rate,
    .round_rate  = pll_clk_round_rate,
    .set_rate    = pll_clk_set_rate,
};

static void m200_pll_clk_setup(struct device_node *node)
{
    struct clk *clk;
    struct clk *parent_clk;
    const char *clk_name = node->name;
    struct clk_init_data init;
    int err;
    struct m200_clk_pll *pll_clk;
    const char *parent_name;

    parent_clk = of_clk_get(node, 0);
    parent_name = __clk_get_name(parent_clk);
    printk("%s line:%d parent_clk:0x%p parent_name:%s clk_name:%s\n", __func__, __LINE__, parent_clk, parent_name, clk_name);

    init.name = clk_name;
    init.ops  = &pll_clk_ops;
    init.parent_names = &parent_name;
    init.num_parents = 1;

    pll_clk = kmalloc(sizeof(struct m200_clk_pll), GFP_KERNEL);
    if (pll_clk == NULL) {
        printk("kmalloc() failed at %s line:%d\n", __func__, __LINE__);
        return;
    }

    if (strcmp(clk_name, "mpll") == 0) {
        pll_clk->pll_type = PLL_TYPE_MPLL;
    } else if (strcmp(clk_name, "apll") == 0) {
        pll_clk->pll_type = PLL_TYPE_APLL;
    } else {
        printk("unsupport pll tye in DT, please check %s line:%d\n", __func__, __LINE__);
        return;
    }

    pll_clk->ulBase = CPM_BASE;
    pll_clk->pll_hw.init = &init;
    clk = clk_register(NULL, &pll_clk->pll_hw);
    if (IS_ERR(clk)) {
        printk("%s line:%d clock register failed:0x%p\n", __func__, __LINE__, clk);
        kfree(pll_clk);
        return;
    }
    printk("%s line:%d num_parents:%d\n", __func__, __LINE__, __clk_get_num_parents(clk));
    pll_clk->pll_hw.clk = clk;

    err = of_clk_add_provider(node, of_clk_src_simple_get, clk);
    if (err) {
        printk("of_clk_add_provider() failed at %s line:%d err:%d\n", __func__, __LINE__, err);
        clk_unregister(clk);
        kfree(pll_clk);
    }
}
CLK_OF_DECLARE(pll_clk, "pll-clock", m200_pll_clk_setup);

struct m200_clk_mux {
    struct clk_hw mux_hw;
    unsigned int clk_no; /* clock number defined in driverlib/clock.h
                          * used to enable/disable clock gate
                          */
};

static int mux_clk_enable(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static void mux_clk_disable(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static u8 mux_clk_get_parent(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int mux_clk_set_parent(struct clk_hw *hw, u8 index)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static const struct clk_ops mux_clk_ops = {
    .enable  = mux_clk_enable,
    .disable = mux_clk_disable,
    .get_parent = mux_clk_get_parent,
    .set_parent = mux_clk_set_parent,
};

static void m200_mux_clk_setup(struct device_node *node)
{
    struct clk *clk;
    const char *clk_name = node->name;
    struct clk_init_data init;
    const char *parent_name;
    u8 num_parents;
    struct m200_clk_mux *mux_clk;

    init.name = clk_name;
    init.ops  = &mux_clk_ops;

    num_parents = -1; /* If no parent num_parents will be zero */
    do {
        num_parents++;
        parent_name = of_clk_get_parent_name(node, num_parents);
    } while (parent_name != NULL);

    if (num_parents == 0) {
        init.parent_names = NULL;
        init.num_parents  = 0;
    } else {
        init.parent_names = kmalloc(sizeof(const char *) * num_parents, GFP_KERNEL);
        if (init.parent_names == NULL) {
            printk("kmalloc() failed at %s line:%d\n", __func__, __LINE__);
            return;
        } else {
            u8 i;
            for (i = 0; i < num_parents; i++) {
                *(init.parent_names + i) = of_clk_get_parent_name(node, i);
            }
            init.num_parents = num_parents;
        }
    }

    mux_clk = kmalloc(sizeof(struct m200_clk_mux), GFP_KERNEL);
    if (mux_clk == NULL) {
        printk("kmalloc() failed at %s line:%d\n", __func__, __LINE__);
    }
    mux_clk->mux_hw.init = &init;

    clk = clk_register(NULL, &mux_clk->mux_hw);
    of_clk_add_provider(node, of_clk_src_simple_get, clk);
    printk("%s line:%d name:%s num_parents:%d\n", __func__, __LINE__, node->name, num_parents);
}
CLK_OF_DECLARE(mux_clk, "mux-clock", m200_mux_clk_setup);

static int __init m200_clock_tree_init(void)
{
    printk("%s line:%d\n", __func__, __LINE__);
    of_clk_init(NULL);

    return 0;
}

arch_initcall(m200_clock_tree_init);
