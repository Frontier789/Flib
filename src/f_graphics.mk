# files
CPP_FILES_GRAPHICS := $(wildcard $(PATH_TO_SRC)/Graphics/*.cpp)
STATIC_OBJ_FILES_GRAPHICS  := $(addprefix $(LIBPATH)/static/Graphics/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o)))
DYNAMIC_OBJ_FILES_GRAPHICS := $(addprefix $(LIBPATH)/dynamic/Graphics/,$(notdir $(CPP_FILES_GRAPHICS:.cpp=.o)))

# append targets
ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-graphics.dll $(LIBPATH)/libf-graphics-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-graphics.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_GRAPHICS)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_GRAPHICS)

###
# directory targets
###
$(LIBPATH)/static/Graphics: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) Graphics

$(LIBPATH)/dynamic/Graphics: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) Graphics

###
# static and dynamic targets
###
$(LIBPATH)/libf-graphics.a: $(STATIC_OBJ_FILES_GRAPHICS) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-graphics.dll $(LIBPATH)/libf-graphics-dll.a: $(LIBPATH)/libf-system-dll.a $(LIBPATH)/libf-gl-dll.a $(LIBPATH)/libf-freetype-dll.a $(DYNAMIC_OBJ_FILES_GRAPHICS) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-graphics.dll $(DYNAMIC_OBJ_FILES_GRAPHICS) -L $(LIBPATH) -lf-system-dll -lf-gl-dll -lf-freetype-dll -lOpenGL32 -lgdi32 -Wl,--out-implib,$(LIBPATH)/libf-graphics-dll.a

###
# object targets
###
$(LIBPATH)/static/Graphics/%.o: $(PATH_TO_SRC)/Graphics/%.cpp | $(LIBPATH)/static/Graphics
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_SRC)/Graphics/FreeType/include -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/Graphics/%.o: $(PATH_TO_SRC)/Graphics/%.cpp | $(LIBPATH)/dynamic/Graphics
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_SRC)/Graphics/FreeType/include -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
