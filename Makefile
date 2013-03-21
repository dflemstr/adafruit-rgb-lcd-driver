MODULE_NAME := adafruit_rgb_lcd
KROOT := /lib/modules/`uname -r`/build

obj-m += adafruit_rgb_lcd.o

.DEFAULT: modules
.PHONY: modules clean

modules:
	@$(MAKE) -C $(KROOT) M=$(PWD) modules

modules_install:
	@$(MAKE) -C $(KROOT) M=$(PWD) modules_install

clean:
	@$(MAKE) -C $(KROOT) M=$(PWD) clean
