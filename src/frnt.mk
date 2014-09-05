CXX=g++
CPPFLAGS+=-Wall -Os -Wfatal-errors -pedantic
PATHTOROOT=..
INCLUDE=$(PATHTOROOT)/include
TARGETDIR=$(PATHTOROOT)/lib$(LIBNAME)
SRC=$(PATHTOROOT)/src
MKDIR=mkdir
AR=ar
CD=cd
RM=rm

ifeq ($(LIBNAME),)
$(warning LIBNAME wasn't specified /using plain 'lib'/)
endif

TARGET: SYSTEM_TARGET GRAPHICS_TARGET | SYSTEM_DIR GRAPHICS_DIR

include f_system.mk
include f_graphics.mk

###
# directory target
###
$(TARGETDIR):
	$(CD) $(PATHTOROOT); $(MKDIR) lib$(LIBNAME)

clean: clean-system clean-graphics
	
	