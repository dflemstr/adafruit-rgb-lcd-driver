obj-m += adafruit_rgb_lcd.o

export KROOT=/lib/modules/`uname -r`/build

.DEFAULT: all
.PHONY: all clean

all:
	@$(MAKE) -C $(KROOT) M=$(PWD) modules

clean:
	@$(MAKE) -C $(KROOT) M=$(PWD) clean
