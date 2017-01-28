PROJECT_FOLDERS=$(wildcard */makefile)

FPATH=..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

define EOL


endef

# for every subfolder that has a makefile call make in it
all:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) --no-print-directory -C $(folder) CXX=$(CXX) ${EOL})

clean:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) clean --no-print-directory -C $(folder) ${EOL})