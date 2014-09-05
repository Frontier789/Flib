#files
CPP_FILES_GRAPHICS := $(wildcard $(PATH_TO_SRC)/Graphics/*.cpp)
OBJ_FILES_GRAPHICS := $(addprefix $(LIBPATH)/Graphics/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o)))

TARGETS+=$(LIBPATH)/libf-graphics.a
DIR_TARGETS+=$(LIBPATH)/Graphics

ifeq ($(COMPILE_GL_HEADER),1)
OBJ_FILES_GRAPHICS+=$(LIBPATH)/Graphics/ext/GL_FUNCTIONS.o
DIR_TARGETS+=$(LIBPATH)/Graphics/ext
endif

#created files
RESULT_FILES := $(OBJ_FILES_GRAPHICS) $(LIBPATH)/libf-graphics.a

###
# directory targets
###
$(LIBPATH)/Graphics: $(LIBPATH)
	$(CD) $(PATH_TO_ROOT); $(CD) $(LIBNAME); $(MKDIR) Graphics
	
$(LIBPATH)/Graphics/ext: $(LIBPATH)/Graphics
	$(CD) $(PATH_TO_ROOT); $(CD) $(LIBNAME); $(CD) Graphics; $(MKDIR) ext

###
# source targets
###
$(LIBPATH)/libf-graphics.a: $(OBJ_FILES_GRAPHICS)
	$(AR) rcs $@ $^

$(LIBPATH)/Graphics/%.o: $(PATH_TO_SRC)/Graphics/%.cpp | $(LIBPATH)/Graphics 
	$(CXX) $(CPPFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/Graphics/ext/%.o: $(PATH_TO_SRC)/Graphics/GL/%.cpp | $(LIBPATH)/Graphics/ext
	$(CXX) $(CPPFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@

###
# clean_targets
###
clean-graphics:
	$(RM) $(OBJ_FILES_GRAPHICS) $(LIBPATH)/libf-graphics.a
