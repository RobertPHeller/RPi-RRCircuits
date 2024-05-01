OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

SUBDIRS = targets doc

-include config.mk
include $(OPENMRNPATH)/etc/recurse.mk

refman.pdf: doc/latex/refman.pdf
	cp doc/latex/refman.pdf ./
	

doc/latex/refman.pdf: 
	$(MAKE) -C doc
