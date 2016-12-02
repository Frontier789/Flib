PROJECT_FOLDERS=$(wildcard */makefile)

all:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) --no-print-directory -C $(folder))

clean:
	$(foreach folder,$(PROJECT_FOLDERS:/makefile=),$(MAKE) clean --no-print-directory -C $(folder))