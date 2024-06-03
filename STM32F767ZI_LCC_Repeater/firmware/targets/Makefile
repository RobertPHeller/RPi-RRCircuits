SUBDIRS = \
	freertos.armv7m.st-stm32f767zi-nucleo	  

include $(OPENMRNPATH)/etc/recurse.mk

flash:      all
	+$(MAKE) -C freertos.armv7m.st-stm32f767zi-nucleo flash
	
gdb: flash
	+$(MAKE) -C freertos.armv7m.st-stm32f767zi-nucleo gdb
