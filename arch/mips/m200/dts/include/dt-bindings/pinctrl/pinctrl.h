/*
 * This header provides constants for most PINCTRL bindings.
 */

#ifndef _DT_BINDINGS_PINCTRL_PINCTRL_H
#define _DT_BINDINGS_PINCTRL_PINCTRL_H

/* Pin pull (default/sleep) */
#define PIN_PULL_ENABLE  0
#define PIN_PULL_DISABLE 1
#define PIN_SLEEP_PULL_ENABLE  PIN_PULL_ENABLE
#define PIN_SLEEP_PULL_DISABLE PIN_PULL_DISABLE

/* Pin function (default/sleep) */
#define PIN_FUNCTION_0 0
#define PIN_FUNCTION_1 1
#define PIN_FUNCTION_2 2
#define PIN_FUNCTION_3 3
#define PIN_SLEEP_FUNCTION_0 PIN_FUNCTION_0
#define PIN_SLEEP_FUNCTION_1 PIN_FUNCTION_1
#define PIN_SLEEP_FUNCTION_2 PIN_FUNCTION_2
#define PIN_SLEEP_FUNCTION_3 PIN_FUNCTION_3

#endif
