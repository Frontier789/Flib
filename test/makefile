PROJECT_FOLDERS=$(wildcard */makefile)

FPATH=..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

all:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) --no-print-directory -C $(folder) CXX=$(CXX) CXXFLAGS=$(CXXFAGS) LDFLAGS=$(LDFLAGS);)

clean:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) clean --no-print-directory -C $(folder) CXX=$(CXX) CXXFLAGS=$(CXXFAGS) LDFLAGS=$(LDFLAGS);)