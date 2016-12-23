#assume windws if Windows_NT is set
ifeq ($(OS),Windows_NT)
 F_LIB_DIR_NAME=libWin
 F_PLATFORM=Windows
 F_EXEC_EXT=.exe
 F_RM=cmd /C del /Q /F
 F_RRM=cmd /C rmdir /Q /S
 define F_DELETE
  $(F_RRM) $(subst /,\,$(1))
 endef
else
 
 #Check for android specific variables
 ifneq "$(or $(ANDROID_DATA),$(ANDRID_ASSETS),$(ANDROID_STORAGE),$(ANDROID_BOOTLOGO),$(ANDROID_ROOT))" ""
  F_LIB_DIR_NAME=libAnd
  F_PLATFORM=Android
  F_EXEC_EXT=
  F_RM = rm -f
  F_RRM = rm -f -r
  define F_DELETE
   $(F_RRM) $(1)
  endef
 else
  #assume plain linux w/ xlib and posix
  F_LIB_DIR_NAME=libLinux
  F_PLATFORM=Linux
  F_EXEC_EXT=
  F_RM = rm -f
  F_RRM = rm -f -r
  define F_DELETE
   $(F_RRM) $(1)
  endef
 endif
endif