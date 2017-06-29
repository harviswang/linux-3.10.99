dtb-$(CONFIG_DTB_W561A) += w561a.dtb

obj-y                   += $(patsubst %.dtb, %.dtb.o, $(dtb-y))

# Force kbuild to make empty built-in.o if necessary
obj-                    += dummy.o

always                  := $(dtb-y)
clean-files             := *.dtb *.dtb.S
