export EXECUTABLE:=$(notdir $(realpath $(CURDIR)/../../..))
APP_PATH ?= $(realpath ../..)
-include $(APP_PATH)/config.mk
-include local_config.mk

OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

# Find STM32CubeF3 libraries
include $(OPENMRNPATH)/etc/stm32cubef7.mk

LDFLAGSEXTRA += 
SYSLIBRARIESEXTRA += -lfreertos_drivers_stm32cubef767xx
SYSLIBRARIESEXTRA += -lfreertos_drivers_spiffs_stm32f767xx
OBJEXTRA += 

CFLAGS += -DSTM32F767xx
CXXFLAGS += -DSTM32F767xx
OPENOCDARGS = -f board/st_nucleo_f7.cfg

GITREPOS += $(realpath $(CURDIR)/../../../..)

ifndef TARGET
export TARGET := freertos.armv7m
endif
include $(OPENMRNPATH)/etc/prog.mk

ifeq ($(call find_missing_deps,OPENOCDPATH OPENOCDSCRIPTSPATH),)
all: $(EXECUTABLE).bin

GDB=/usr/bin/gdb-multiarch


flash: $(EXECUTABLE)$(EXTENTION) $(EXECUTABLE).lst
	@if ps ax -o comm | grep -q openocd ; then echo openocd already running. quit existing first. ; exit 1 ; fi
	$(GDB) $< -ex "target remote | $(OPENOCDPATH)/openocd -c \"gdb_port pipe\" --search $(OPENOCDSCRIPTSPATH) $(OPENOCDARGS)" -ex "monitor reset halt" -ex "load" -ex "monitor reset init" -ex "monitor reset run"  -ex "detach" -ex "quit"

gdb:
	@if ps ax -o comm | grep -q openocd ; then echo openocd already running. quit existing first. ; exit 1 ; fi
	$(GDB) $(EXECUTABLE)$(EXTENTION) -ex "target remote | $(OPENOCDPATH)/openocd -c \"gdb_port pipe\" --search $(OPENOCDSCRIPTSPATH) $(OPENOCDARGS)" -ex "continue" # -ex "monitor reset halt"

else

flash gdb:
	echo OPENOCD not found ; exit 1

endif
