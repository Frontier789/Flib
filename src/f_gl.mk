#files
CPP_FILES_GL := $(wildcard $(PATH_TO_SRC)/GL/*.cpp)
STATIC_OBJ_FILES_GL  := $(addprefix $(LIBPATH)/static/GL/,$(notdir $(CPP_FILES_GL:.cpp=.o)))
DYNAMIC_OBJ_FILES_GL := $(addprefix $(LIBPATH)/dynamic/GL/,$(notdir $(CPP_FILES_GL:.cpp=.o)))

ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-gl.dll $(LIBPATH)/libf-gl-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-gl.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_GL)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_GL)

###
# directory targets
###
$(LIBPATH)/static/GL: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) GL

$(LIBPATH)/dynamic/GL: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) GL

###
# static and dynamic targets
###
$(LIBPATH)/libf-gl.a: $(STATIC_OBJ_FILES_GL) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-gl.dll $(LIBPATH)/libf-gl-dll.a: $(LIBPATH)/libf-system-dll.a $(DYNAMIC_OBJ_FILES_GL) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-gl.dll $(DYNAMIC_OBJ_FILES_GL) -L $(LIBPATH) -lf-system-dll -lOpenGL32 -Wl,--out-implib,$(LIBPATH)/libf-gl-dll.a

###
# object targets
###
$(LIBPATH)/static/GL/%.o: $(PATH_TO_SRC)/GL/%.cpp | $(LIBPATH)/static/GL
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/GL/%.o: $(PATH_TO_SRC)/GL/%.cpp | $(LIBPATH)/dynamic/GL
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
