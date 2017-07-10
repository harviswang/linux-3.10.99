#
# Makefile for the M200 kernel device drivers.
#

obj-$(CONFIG_SERIAL_M200_CONSOLE) += m200_uart.o
obj-y += cp0.o
obj-y += register.o
obj-$(CONFIG_PWM)                 += m200_pwm.o
obj-$(CONFIG_BACKLIGHT_PWM)       += m200_backlight_pwm.o
obj-$(CONFIG_GPIOLIB)             += m200_gpio.o
