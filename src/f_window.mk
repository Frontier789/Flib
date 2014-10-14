#files
CPP_FILES_WINDOW := $(wildcard $(PATH_TO_SRC)/Window/*.cpp)
STATIC_OBJ_FILES_WINDOW  := $(addprefix $(LIBPATH)/static/Window/,$(notdir $(CPP_FILES_WINDOW:.cpp=.o)))
DYNAMIC_OBJ_FILES_WINDOW := $(addprefix $(LIBPATH)/dynamic/Window/,$(notdir $(CPP_FILES_WINDOW:.cpp=.o)))

ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-window.dll $(LIBPATH)/libf-window-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-window.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_WINDOW)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_WINDOW)

###
# directory targets
###
$(LIBPATH)/static/Window: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) Window

$(LIBPATH)/dynamic/Window: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) Window

###
# static and dynamic targets
###
$(LIBPATH)/libf-window.a: $(STATIC_OBJ_FILES_WINDOW) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-window.dll $(LIBPATH)/libf-window-dll.a: $(LIBPATH)/libf-system-dll.a $(DYNAMIC_OBJ_FILES_WINDOW) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-window.dll $(DYNAMIC_OBJ_FILES_WINDOW) -L $(LIBPATH) -lf-system.dll -lOpenGL32 -lgdi32 -Wl,--out-implib,$(LIBPATH)/libf-window-dll.a

###
# object targets
###
$(LIBPATH)/static/Window/%.o: $(PATH_TO_SRC)/Window/%.cpp | $(LIBPATH)/static/Window
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/Window/%.o: $(PATH_TO_SRC)/Window/%.cpp | $(LIBPATH)/dynamic/Window
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
