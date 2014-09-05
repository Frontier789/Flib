#files
ifeq ($(COMPILE_GL_HEADER),1)

CPP_FILES_GRAPHICS := $(wildcard $(SRC)/Graphics/*.cpp)
OBJ_FILES_GRAPHICS := $(addprefix $(TARGETDIR)/Graphics/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o))) $(TARGETDIR)/Graphics/ext/GL_FUNCTIONS.o

GRAPHICS_TARGET: $(TARGETDIR)/libf-graphics.a $(TARGETDIR)/Graphics/ext
GRAPHICS_DIR: $(TARGETDIR)/Graphics

else

CPP_FILES_GRAPHICS := $(wildcard $(SRC)/Graphics/*.cpp)
OBJ_FILES_GRAPHICS := $(addprefix $(TARGETDIR)/Graphics/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o)))

GRAPHICS_TARGET: $(TARGETDIR)/libf-graphics.a
GRAPHICS_DIR: $(TARGETDIR)/Graphics

endif

#created files
RESULT_FILES := $(OBJ_FILES_GRAPHICS) $(TARGETDIR)/libf-graphics.a

###
# directory targets
###
$(TARGETDIR)/Graphics: $(TARGETDIR)
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(MKDIR) Graphics
	
$(TARGETDIR)/Graphics/ext: $(TARGETDIR)/Graphics
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(CD) Graphics; $(MKDIR) ext

###
# source targets
###
$(TARGETDIR)/libf-graphics.a: $(OBJ_FILES_GRAPHICS)
	$(AR) rcs $@ $^

$(TARGETDIR)/Graphics/%.o: $(SRC)/Graphics/%.cpp | $(TARGETDIR)/Graphics 
	$(CXX) $(CPPFLAGS) -I  $(INCLUDE) -c $< -o $@
	
$(TARGETDIR)/Graphics/ext/%.o: $(SRC)/Graphics/GL/%.cpp | $(TARGETDIR)/Graphics/ext
	$(CXX) $(CPPFLAGS) -I $(INCLUDE) -c $< -o $@

###
# clean_targets
###
clean-graphics:
	$(RM) $(OBJ_FILES_GRAPHICS) $(TARGETDIR)/libf-graphics.a
