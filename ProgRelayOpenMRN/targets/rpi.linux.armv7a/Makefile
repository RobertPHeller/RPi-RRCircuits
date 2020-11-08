export TARGET:=linux.armv7a
export EXECUTABLE:=$(notdir $(realpath $(CURDIR)/../..))_RPI

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
include $(OPENMRNPATH)/etc/prog.mk

SYSLIBRARIES += -lavahi-client -lavahi-common

