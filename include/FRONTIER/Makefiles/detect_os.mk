#assume windws if Windows_NT is set
ifeq ($(OS),Windows_NT)
 LIBNAME=libWin
 PLATFORM=Windows
else
 
 #Check for android specific variables
 ifneq "$(or $(ANDROID_DATA),$(ANDRID_ASSETS),$(ANDROID_STORAGE),$(ANDROID_BOOTLOGO),$(ANDROID_ROOT))" ""
  LIBNAME=libAnd
  PLATFORM=Android
 else
  #assume plain linux w/ xlib
  LIBNAME=libLinux
  PLATFORM=Linux
 endif
endif