MODULE_NAME := adafruit_rgb_lcd
KROOT := /lib/modules/`uname -r`/build

obj-m += adafruit_rgb_lcd.o

.DEFAULT: all
.PHONY: all clean

all:
	@$(MAKE) -C $(KROOT) M=$(PWD) modules

clean:
	@$(MAKE) -C $(KROOT) M=$(PWD) clean
