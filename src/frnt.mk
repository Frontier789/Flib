
ifneq ($(LIBNAME),)

CXX=g++
CPPFLAGS+=-Wall -Os -Wfatal-errors -pedantic
PATHTOROOT=..
INCLUDE=$(PATHTOROOT)/include
TARGETDIR=$(PATHTOROOT)/lib$(LIBNAME)
SRC=$(PATHTOROOT)/src
MKDIR=mkdir
RMDIR=rmdir
AR=ar
CD=cd
RM=rm

#lib-system files
CPP_FILES_SYSTEM := $(wildcard $(SRC)/System/*.cpp)
OBJ_FILES_SYSTEM := $(addprefix $(TARGETDIR)/System/,$(notdir $(CPP_FILES_SYSTEM:.cpp=.o)))

#lib-graphics files
CPP_FILES_GRAPHICS := $(wildcard $(SRC)/Graphics/*.cpp)
OBJ_FILES_GRAPHICS := $(addprefix $(TARGETDIR)/Graphics/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o)))

#lib-network files
CPP_FILES_NETWORK := $(wildcard $(SRC)/Network/*.cpp)
OBJ_FILES_NETWORK := $(addprefix $(TARGETDIR)/Network/,$(notdir $(CPP_FILES_NETWORK:.cpp=.o)))

###
# targets to create
###
ALL_TARGETS  := $(TARGETDIR) $(TARGETDIR)/System $(TARGETDIR)/Graphics $(TARGETDIR)/Network 

ifeq ($(COMPILE_GL_HEADER),1)
	ALL_TARGETS 	   += $(TARGETDIR)/Graphics/ext
	OBJ_FILES_GRAPHICS += $(TARGETDIR)/Graphics/ext/gl_comp_4_2.o
endif

ALL_TARGETS += $(TARGETDIR)/libfrontier-system.a $(TARGETDIR)/libfrontier-graphics.a $(TARGETDIR)/libfrontier-network.a


#created files
RESULT_FILES := $(OBJ_FILES_SYSTEM) $(OBJ_FILES_GRAPHICS) $(OBJ_FILES_NETWORK) $(TARGETDIR)/libfrontier-system.a $(TARGETDIR)/libfrontier-graphics.a $(TARGETDIR)/libfrontier-network.a

all: $(ALL_TARGETS)

###
# Directory_target
###
$(TARGETDIR):
	$(CD) $(PATHTOROOT); $(MKDIR) lib$(LIBNAME)
$(TARGETDIR)/System: $(TARGETDIR)
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(MKDIR) System
$(TARGETDIR)/Network: $(TARGETDIR)
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(MKDIR) Network
$(TARGETDIR)/Graphics: $(TARGETDIR)
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(MKDIR) Graphics
$(TARGETDIR)/Graphics/ext: $(TARGETDIR)/Graphics
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(CD) Graphics; $(MKDIR) ext

###
# System_target
###
$(TARGETDIR)/libfrontier-system.a: $(OBJ_FILES_SYSTEM)
	$(AR) rcs $@ $^

$(TARGETDIR)/System/%.o: $(SRC)/System/%.cpp
	$(CXX) $(CPPFLAGS) -I  $(INCLUDE) -c $< -o $@


###
# Network_target
###
$(TARGETDIR)/libfrontier-network.a: $(OBJ_FILES_NETWORK)
	$(AR) rcs $@ $^

$(TARGETDIR)/Network/%.o: $(SRC)/Network/%.cpp
	$(CXX) $(CPPFLAGS) -I  $(INCLUDE) -c $< -o $@



###
# Graphics_target
###
$(TARGETDIR)/libfrontier-graphics.a: $(OBJ_FILES_GRAPHICS)
	$(AR) rcs $@ $^
	
$(TARGETDIR)/Graphics/%.o: $(SRC)/Graphics/%.cpp
	$(CXX) $(CPPFLAGS) -I $(INCLUDE) -c $< -o $@ 
	
$(TARGETDIR)/Graphics/ext/%.o: $(SRC)/Graphics/GL/%.c
	$(CXX) $(CPPFLAGS) -I $(INCLUDE) -c $< -o $@

###
# clean_targets
###
clean: 
	$(RM) $(RESULT_FILES)
	
clean-graphics:
	$(RM) $(OBJ_FILES_GRAPHICS) $(TARGETDIR)/libfrontier-graphics.a
	
clean-system:
	$(RM) $(OBJ_FILES_SYSTEM) $(TARGETDIR)/libfrontier-system.a
	
clean-network:
	$(RM) $(OBJ_FILES_NETWORK) $(TARGETDIR)/libfrontier-network.a

clean-all:
	$(RM)  $(RESULT_FILES) &&\
	$(RMDIR) $(TARGETDIR)/Graphics/ext $(TARGETDIR)/Graphics $(TARGETDIR)/System $(TARGETDIR)/Network

else

all: 
	@echo error: LIBNAME wasnt specified

endif
	
	