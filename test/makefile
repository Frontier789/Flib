PROJECT_MAKEFILES=$(wildcard */makefile)
PROJECT_FOLDERS=$(PROJECT_MAKEFILES:/makefile=)

FPATH=..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

all clean: $(PROJECT_FOLDERS)

.PHONY: force

$(PROJECT_FOLDERS):: force
	-$(MAKE) --no-print-directory -C $@ $(MAKECMDGOALS) CXX=$(CXX)