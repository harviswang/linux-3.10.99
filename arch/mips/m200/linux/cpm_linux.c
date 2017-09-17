/*
 * CPM module
 */
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/printk.h>
#include <linux/slab.h>

#include "../inc/hw_memmap.h"
#include "../inc/hw_cpm.h"
#include "../inc/hw_types.h"
#include "../driverlib/cpm.h"
#include "../driverlib/clock.h"

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

static int pll_clk_set_rate(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate);
static unsigned long pll_clk_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
    struct m200_clk_pll *pll_clk = (struct m200_clk_pll *)hw;
    unsigned long ulM, ulN, ulNFraction;

    printk("%s line:%d parent_rate:%ld\n", __func__, __LINE__, parent_rate);
/*
 * test code
    if (pll_clk->pll_type == PLL_TYPE_APLL) {
        pll_clk_set_rate(hw, 1280000000, 24000000);
    }
*/

    switch (pll_clk->pll_type) {
    case PLL_TYPE_APLL:
        if (CPMApllIsBypassed(pll_clk->ulBase)) {
            return parent_rate;
        } else {
            ulM = CPMApllGetM(pll_clk->ulBase);
            ulN = CPMApllGetN(pll_clk->ulBase);
            ulNFraction = CPMApllGetNFraction(pll_clk->ulBase);
            if (ulNFraction == 0) {
                unsigned long ulGcd = CPMGreatestCommonDividor(parent_rate, ulN);/* in order to avoid overflow */
                parent_rate /= ulGcd;
                ulN /= ulGcd;
                ulGcd = CPMGreatestCommonDividor(ulM, ulN);
                ulM /= ulGcd;
                ulN /= ulGcd;
                return (parent_rate * ulM) / ulN;
            } else {
                unsigned long ulGcd = CPMGreatestCommonDividor(ulNFraction, 1 << 24);
                unsigned long ulFenZi = ulNFraction / ulGcd;
                unsigned long ulFenMu = (1 << 24) / ulGcd;
                unsigned long ulFenMu2;
                printk("%s line:%d rate:%ld\n", __func__, __LINE__, ((parent_rate * ulM) * ulFenMu) / (ulN * ulFenMu + ulFenZi));
                /* ((parent_rate * ulM) * ulFenMu) / (ulN * ulFenMu + ulFenZi) may be overflow, not tested */
                ulFenMu2 = (ulN * ulFenMu + ulFenZi);
                ulGcd = CPMGreatestCommonDividor(parent_rate, ulFenMu2);
                parent_rate /= ulGcd;
                ulFenMu2 /= ulGcd;

                ulGcd = CPMGreatestCommonDividor(ulM, ulFenMu2);
                ulM /= ulGcd;
                ulFenMu2 /= ulGcd;

                ulGcd = CPMGreatestCommonDividor(ulFenMu, ulFenMu2);
                ulFenMu /= ulGcd;
                ulFenMu2 /= ulGcd;

                return ((parent_rate * ulM) * ulFenMu) / ulFenMu2;
            }
        }
    case PLL_TYPE_MPLL:
        ulM = CPMMpllGetM(pll_clk->ulBase);
        ulN = CPMMpllGetN(pll_clk->ulBase);
        ulNFraction = CPMMpllGetNFraction(pll_clk->ulBase);
        if (ulNFraction == 0) {
            return (parent_rate * ulM) / ulN;
        } else {
            unsigned long ulGcd = CPMGreatestCommonDividor(ulNFraction, 1 << 24);
            unsigned long ulFenZi = ulNFraction / ulGcd;
            unsigned long ulFenMu = (1 << 24) / ulGcd;
            return ((parent_rate * ulM) * ulFenMu) / (ulN * ulFenMu + ulFenZi);
        }
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
    unsigned long ulTmp;
    printk("%s line:%d rate:%ld parent_rate:%ld ulM:%ld ulMFraction:0x%lx ulN:%ld ulNFraction:0x%lx\n",
        __func__, __LINE__, rate, parent_rate, ulM, ulMFraction, ulN, ulNFraction);

    switch (pll_clk->pll_type) {
    case PLL_TYPE_APLL:
        ulTmp = HWREG(CPM_BASE + CPM_O_APCR);
/*
        ulTmp &= ~CPM_APCR_APLLEN;
        HWREG(CPM_BASE + CPM_O_APCR) = ulTmp;
*/
        ulTmp &= (~(CPM_APCR_APLLM | CPM_APCR_APLLN | CPM_APCR_APLLOD1 | CPM_APCR_APLLOD0));
        ulTmp |= (ulM << 20) | (ulN << 14) | (1 << 11) | (1 << 8) | CPM_APCR_APLLEN;
        HWREG(CPM_BASE + CPM_O_APCR) = ulTmp;

        while ((HWREG(CPM_BASE + CPM_O_APCR) & CPM_APCR_APLLON) != CPM_APCR_APLLON) {
            printk("%s line:%d\n", __func__, __LINE__);
        }
        break;
    case PLL_TYPE_MPLL:
        ulTmp = HWREG(CPM_BASE + CPM_O_MPCR);
        ulTmp &= (~(CPM_MPCR_MPLLM | CPM_MPCR_MPLLN | CPM_MPCR_MPLLOD1 | CPM_MPCR_MPLLOD0));
        ulTmp |= (ulM << 20) | (ulN << 14) | (1 << 11) | (1 << 8) | CPM_MPCR_MPLLEN;
        HWREG(CPM_BASE + CPM_O_MPCR) = ulTmp;
        while ((HWREG(CPM_BASE + CPM_O_MPCR) & CPM_MPCR_MPLLON) != CPM_MPCR_MPLLON) {
            printk("%s line:%d\n", __func__, __LINE__);
        }
        break;
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

/*
 * @clk_nos, clk_no array to store few clk_no, clock number defined in driverlib/clock.h
 *           used to enable/disable clock gate
 * @clk_no_num, clk_no array size
 * @current_parent_index, indicate which parent is current used.
 * clk_nos[current_parent_index] is the parent's clock number,
 * mux_hw.init->parent_names[current_parent_index] is the parent's clock name
 * @max_divider, clock divider range [1 or 2, max_divider], such MSC clock divider
 * range from 2, other from 1
 */
struct m200_clk_mux {
    struct clk_hw mux_hw;
    u8 *clk_nos;
    u8  clk_no_num;
    u8  current_parent_index;
    u32 max_divider;
};

static int mux_clk_prepare(struct clk_hw *hw)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    printk("%s line:%d max_divider:%d\n", __func__, __LINE__, mux_clk->max_divider);
    return 0;
}

static void mux_clk_unprepare(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static int mux_clk_enable(struct clk_hw *hw)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    int clk_no = *(mux_clk->clk_nos + mux_clk->current_parent_index);
    int result = CPMClockEnable(CPM_BASE, clk_no);

    printk("%s line:%d clk_no:%d result:%d\n", __func__, __LINE__, clk_no, result);
    return 0;
}

static void mux_clk_disable(struct clk_hw *hw)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    int clk_no = *(mux_clk->clk_nos + mux_clk->current_parent_index);
    int result = 0;

    printk("%s line:%d clk_no:%d\n", __func__, __LINE__, clk_no);
    result = CPMClockDisable(CPM_BASE, clk_no);
    printk("%s line:%d clk_no:%d result:%d\n", __func__, __LINE__, clk_no, result);
}

static u8 mux_clk_get_parent(struct clk_hw *hw)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    struct clk *clk = mux_clk->mux_hw.clk;
    printk("%s line:%d mux_clk->current_parent_index:%d name:%s\n", __func__, __LINE__, mux_clk->current_parent_index, __clk_get_name(clk));

    return mux_clk->current_parent_index;
}

static int mux_clk_set_parent(struct clk_hw *hw, u8 index)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    printk("%s line:%d\n", __func__, __LINE__);

    if (index < mux_clk->clk_no_num) {
        mux_clk->current_parent_index = index;
        return 0;
    } else {
        printk("Error: Invalid parent index at %s line:%d\n", __func__, __LINE__);
        return -EINVAL;
    }
}

#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

static long mux_clk_round_rate(struct clk_hw *hw, unsigned long rate, unsigned long *parent_rate)
{
    /*
     * if rate=0, *parent_rate % rate will very very slow(Floating point exception)
     */
    if (((*parent_rate % rate) == 0) || (rate == 0)) {
        /* need do nothing */
    } else { /* get the nearest divider */
        unsigned long tmp = *parent_rate / rate;
        unsigned long i = 0, left = tmp, right = tmp;
        unsigned long ulAbsLeft, ulAbsTmp, ulAbsRight, ulAbsMin;

        ulAbsTmp = ABS(*parent_rate / tmp - rate);

        for (i = tmp + 1; (*parent_rate % i) != 0; i++) {
        }
        right = i;
        ulAbsLeft = ABS(rate - (*parent_rate / right)); // *parent_rate/right in left of ulRate

        for (i = tmp - 1; (i != 0) && ((*parent_rate % i) != 0); i--) {
        }
        left = i;
        ulAbsRight = ABS(rate - (*parent_rate / left)); // *parent_rate/left in right of ulRate

        ulAbsMin = MIN(MIN(ulAbsLeft, ulAbsTmp), ulAbsRight);
        if (ulAbsMin == ulAbsLeft) {         /* ulParentRate/right is the most nearest to ulRate */
            tmp = right;
        } else if (ulAbsMin == ulAbsRight) { /* ulParentRate/left is the most nearest to ulRate */
            tmp = left;
        } else { /* ulAbsMin == ulAbsTmp */  /* ulParentRate/tmp is the most nearest to ulRate */
        }

        rate = *parent_rate / tmp;
    }
    return rate;
}

/*
 *
 */
static int mux_clk_set_rate(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    int clk_no = *(mux_clk->clk_nos + mux_clk->current_parent_index);
    unsigned long result;

    /* rate is zero, we should gate the clock */
    if (rate == 0) {
        result = CPMClockDisable(CPM_BASE, clk_no);
    } else {
        unsigned long divider;
        /* if last clock rate is zero, we should ungate the clock first */
        unsigned long old_rate = clk_get_rate(hw->clk);
        if (old_rate == 0) {
            CPMClockEnable(CPM_BASE, clk_no);
        }

        rate = mux_clk_round_rate(hw, rate, &parent_rate);
        printk("%s line:%d rate:%ld\n", __func__, __LINE__, rate);
        divider = parent_rate / rate;
        if (divider > mux_clk->max_divider) {
            printk("Error: invalid divider %ld at %s line:%d\n", divider, __func__, __LINE__);
            return -EINVAL;
        }

        result = CPMClockDividerSet(CPM_BASE, clk_no, divider);
    }

    return result == 1 ? 0 : -EINVAL;
}

static unsigned long mux_clk_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
    struct m200_clk_mux *mux_clk = (struct m200_clk_mux *)hw;
    int clk_no = *(mux_clk->clk_nos + mux_clk->current_parent_index);

    if (CPMClockIsEnabled(CPM_BASE, clk_no)) {
        unsigned long divider = CPMClockDividerGet(CPM_BASE, clk_no);

        printk("clk:%s %s line:%d divider:%ld\n", __clk_get_name(hw->clk), __func__, __LINE__, divider);
/*
        if (clk_no == CLOCK_NO_AHB2) {
            mux_clk_set_rate(hw, 100000000, parent_rate);
            divider = CPMClockDividerGet(CPM_BASE, clk_no);
        }
*/
        if (divider == 0) { /* the clock not support frequency divider */
            return parent_rate;
        } else {
            return parent_rate / divider;
        }
    } else { // clock is disable, it's frequency need to set zero
             // since when set clock's frequency 0Hz, we will disable it.
        return 0;
    }
}

static const struct clk_ops mux_clk_ops = {
    .prepare = mux_clk_prepare,
    .unprepare = mux_clk_unprepare,
    .enable  = mux_clk_enable,
    .disable = mux_clk_disable,
    .get_parent = mux_clk_get_parent,
    .set_parent = mux_clk_set_parent,
    .round_rate = mux_clk_round_rate,
    .set_rate   = mux_clk_set_rate,
    .recalc_rate= mux_clk_recalc_rate,
};

static struct clk *mux_clk_src_onecell_get(struct of_phandle_args *clkspec, void *data)
{
    struct m200_clk_mux *mux_clk = ( struct m200_clk_mux *)data;
    struct clk *clk = mux_clk->mux_hw.clk;

    printk("%s line:%d args_count:%d clk:0x%p clk->name:%s clk_no:%d\n", __func__, __LINE__, clkspec->args_count, clk, __clk_get_name(clk), *(mux_clk->clk_nos + mux_clk->current_parent_index));
    return clk;
}

static int mux_clk_onecell_get_clkno(struct device_node *node, int index)
{
    if (index < 0) {
        return -EINVAL;
    } else {
        struct of_phandle_args clkspec;
        int err = of_parse_phandle_with_args(node, "clocks", "#clock-cells", index,
                    &clkspec);
        if (err) {
            return -EINVAL;
        } else {
            return clkspec.args[0];
        }
    }
}

static void m200_mux_clk_setup(struct device_node *node)
{
    struct clk *clk;
    const char *clk_name = node->name;
    struct clk_init_data init;
    const char *parent_name;
    u8 num_parents;
    struct m200_clk_mux *mux_clk;
    int err = 0;
    u32 current_parent_index;

    init.name = clk_name;
    init.ops  = &mux_clk_ops;

    num_parents = -1; /* If no parent num_parents will be zero */
    do {
        num_parents++;
        parent_name = of_clk_get_parent_name(node, num_parents);
    } while (parent_name != NULL);

    mux_clk = kmalloc(sizeof(struct m200_clk_mux), GFP_KERNEL);
    if (mux_clk == NULL) {
        printk("kmalloc() failed at %s line:%d\n", __func__, __LINE__);
    }

    err = of_property_read_u32_index(node, "current_parent_index", 0, &current_parent_index);
    if (err) {
        printk("Error: no current_parent_index property at %s line:%d\n", __func__, __LINE__);
        mux_clk->current_parent_index = 0; /* default parent is the first */
    } else {
        mux_clk->current_parent_index = current_parent_index;
    }

    if (num_parents == 0) {
        init.parent_names = NULL;
        init.num_parents  = 0;
        mux_clk->clk_nos = NULL;
        mux_clk->clk_no_num = 0;
    } else {
        mux_clk->clk_nos = kzalloc(sizeof(*mux_clk->clk_nos) * num_parents, GFP_KERNEL);
        if (mux_clk->clk_nos == NULL) {
            printk("kzalloc() failed at %s line:%d\n", __func__, __LINE__);
            return;
        }

        init.parent_names = kmalloc(sizeof(const char *) * num_parents, GFP_KERNEL);
        if (init.parent_names == NULL) {
            printk("kmalloc() failed at %s line:%d\n", __func__, __LINE__);
            return;
        } else {
            u8 i;
            for (i = 0; i < num_parents; i++) {
                *(init.parent_names + i) = of_clk_get_parent_name(node, i);
                *(mux_clk->clk_nos + i) = mux_clk_onecell_get_clkno(node, i);
            }
            init.num_parents = num_parents;
            mux_clk->clk_no_num = num_parents; 
        }
    }

    err = of_property_read_u32_index(node, "max_divider", 0, &mux_clk->max_divider);
    if (err) {
        printk("of_property_read_u32_index() failed at %s line:%d err:0x%x\n", __func__, __LINE__, err);
    }
    mux_clk->mux_hw.init = &init;

    clk = clk_register(NULL, &mux_clk->mux_hw);
    mux_clk->mux_hw.clk = clk;
    of_clk_add_provider(node, mux_clk_src_onecell_get, mux_clk);
    printk("%s line:%d name:%s num_parents:%d\n", __func__, __LINE__, node->name, num_parents);
    mux_clk_set_parent(&mux_clk->mux_hw, mux_clk->current_parent_index);
}
CLK_OF_DECLARE(mux_clk, "mux-clock", m200_mux_clk_setup);

static int device_clk_prepare(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static void device_clk_unprepare(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static int device_clk_enable(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static void device_clk_disable(struct clk_hw *hw)
{
    printk("%s line:%d\n", __func__, __LINE__);
}

static struct clk_ops device_clk_ops = {
    .prepare   = device_clk_prepare,
    .unprepare = device_clk_unprepare,
    .enable  = device_clk_enable,
    .disable = device_clk_disable,
};

static void m200_device_clk_setup(struct device_node *node)
{
    struct clk *clk;
    struct clk *parent_clk;
    const char *clk_name = node->name;
    struct clk_init_data init;
    const char *parent_name;
    struct clk_hw *hw;

    parent_clk = of_clk_get(node, 0);
    parent_name = __clk_get_name(parent_clk);

    init.name = clk_name;
    init.ops  = &device_clk_ops;
    init.parent_names = &parent_name;
    init.num_parents = 1;

    hw = kzalloc(sizeof(struct clk_hw), GFP_KERNEL);
    if (hw == NULL) {
        printk("kzalloc() failed at %s line:%d\n", __func__, __LINE__);
        return;
    }
    hw->init = &init;

    clk = clk_register(NULL, hw);
    of_clk_add_provider(node, of_clk_src_simple_get, clk);
    printk("%s line:%d name:%s parent_name:%s\n", __func__, __LINE__, node->name, parent_name);
}
CLK_OF_DECLARE(device_clk, "device-clock", m200_device_clk_setup);

static int __init m200_clock_tree_init(void)
{
    printk("%s line:%d\n", __func__, __LINE__);
    of_clk_init(NULL);

    return 0;
}

arch_initcall(m200_clock_tree_init);
