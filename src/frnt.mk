MKDIR=mkdir
CXX=g++
AR=ar
CD=cd
RM=rm

override CXXFLAGS+=-Wno-long-long -Wfatal-errors -Wall -Wextra -Os -pedantic 
#override CXXFLAGS+= -pg -g -fno-inline-functions -Werror
override CXXFLAGS+=-DFRONTIER_LIBBUILD -DFRONTIER_PROTECT_SHARED_VARIABLES

ifeq ($(LIBNAME),)
LIBNAME=lib
endif

PATH_TO_ROOT=..
PATH_TO_LIB=$(PATH_TO_ROOT)
PATH_TO_SRC=$(PATH_TO_ROOT)/src
PATH_TO_INCLUDE=$(PATH_TO_ROOT)/include

LIBPATH=$(PATH_TO_LIB)/$(LIBNAME)

static: MAKE_STATIC_TARGETS
dynamic: MAKE_DYNAMIC_TARGETS

#include system submake
include f_system.mk

#include OpenGL submake
include f_gl.mk

#include Window submake
include f_window.mk

#include Window submake
include f_graphics.mk

STATIC_TARGETS+=$(LIBPATH)/libf.a
DYNAMIC_TARGETS+=$(LIBPATH)/f.dll $(LIBPATH)/libf-dll.a

MAKE_STATIC_TARGETS: $(STATIC_TARGETS)
MAKE_DYNAMIC_TARGETS: $(DYNAMIC_TARGETS)

###
# static and dynamic targets
###
$(LIBPATH)/libf.a: $(STATIC_OBJ_FILES) | $(LIBPATH) 
	$(AR) rcs $@ $(STATIC_OBJ_FILES)

$(LIBPATH)/f.dll $(LIBPATH)/f-dll.a: $(DYNAMIC_OBJ_FILES) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f.dll $(DYNAMIC_OBJ_FILES) -lOpenGL32 -lgdi32 -Wl,--out-implib,$(LIBPATH)/libf-dll.a

###
# directory target
###
$(LIBPATH):
	$(CD) $(PATH_TO_ROOT) && $(MKDIR) $(LIBNAME)
	
$(LIBPATH)/static: | $(LIBPATH)
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(MKDIR) static
	
$(LIBPATH)/dynamic: | $(LIBPATH)
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(MKDIR) dynamic
	
	