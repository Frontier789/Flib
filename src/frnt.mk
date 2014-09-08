MKDIR=mkdir
CXX=g++
AR=ar
CD=cd
RM=rm

CPPFLAGS+=-Wall -Os -Wfatal-errors -pedantic

PATH_TO_ROOT=..
PATH_TO_LIB=$(PATH_TO_ROOT)
PATH_TO_SRC=$(PATH_TO_ROOT)/src
PATH_TO_INCLUDE=$(PATH_TO_ROOT)/include

LIBPATH=$(PATH_TO_LIB)/$(LIBNAME)

COMPILE_SYSTEM=1
COMPILE_GRAPHICS=1
COMPILE_GL_HEADER=1
COMPILE_MERGED=1


ifeq ($(LIBNAME),)
	$(warning LIBNAME wasn't specified /using plain 'lib'/)
endif

all: MAKE_TARGETS | MAKE_DIR_TARGETS

#only include the submake if compile system
ifeq ($(COMPILE_SYSTEM),1)
include f_system.mk
endif

#only include the submake if compile graphics
ifeq ($(COMPILE_GRAPHICS),1)
include f_graphics.mk
endif

ifeq ($(COMPILE_MERGED),1)
TARGETS+=$(LIBPATH)/libf.a
clean-merged:
	$(RM) $(LIBPATH)/libf.a
else
clean-merged:
endif

MAKE_TARGETS: $(TARGETS) 
MAKE_DIR_TARGETS: | $(DIR_TARGETS) 

###
# directory target
###
$(LIBPATH)/libf.a: $(OBJ_FILES) | $(LIBPATH) 
	$(AR) rcs $@ $(OBJ_FILES)
###
# directory target
###
$(PATH_TO_LIB)/$(LIBNAME):
	$(CD) $(PATH_TO_ROOT); $(MKDIR) $(LIBNAME)

clean: clean-system clean-graphics clean-merged
	
	