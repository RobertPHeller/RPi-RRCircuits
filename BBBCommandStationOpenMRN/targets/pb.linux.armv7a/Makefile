export TARGET:=linux.armv7a
export EXECUTABLE:=$(notdir $(realpath $(CURDIR)/../..))_PB

OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

include ../../../CommonOpenMRNExtras/NodeIDBin.mk

APP_PATH ?= $(realpath ../..)
-include ../../config.mk
INCLUDES += -I$(realpath ../../LCCTrainSearchProtocol/include)
INCLUDES += -I/usr/include/nlohmann
SUBDIRS = LCCTrainSearchProtocol
include $(OPENMRNPATH)/etc/prog.mk

