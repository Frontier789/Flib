PROJECT_FOLDERS=$(wildcard */makefile)

FPATH=..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

# for every subfolder that has a makefile call make in it
all:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) --no-print-directory -C $(folder) CXX=$(CXX);)

clean:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) clean --no-print-directory -C $(folder);)