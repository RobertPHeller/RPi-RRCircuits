export TARGET:=linux.armv7a
export EXECUTABLE:=$(notdir $(realpath $(CURDIR)/../..))_PB

OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

NODEID.hxx : NODEID.txt
	../../GenerateNODEID.tcl
	
main.o: NODEID.hxx

NodeIDBin: $(EXECUTABLE) NODEID.txt
	mv $(EXECUTABLE) $(EXECUTABLE)_`cat NODEID.txt|tr ' ' '_'`
	rm -f NODEID.hxx 
	mv $(EXECUTABLE).lst $(EXECUTABLE)_`cat NODEID.txt|tr ' ' '_'`.lst
	mv $(EXECUTABLE).map $(EXECUTABLE)_`cat NODEID.txt|tr ' ' '_'`.map

APP_PATH ?= $(realpath ../..)
-include ../../config.mk
include $(OPENMRNPATH)/etc/prog.mk
