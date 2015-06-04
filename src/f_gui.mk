# files
CPP_FILES_GUI := $(wildcard $(PATH_TO_SRC)/Gui/*.cpp)
STATIC_OBJ_FILES_GUI  := $(addprefix $(LIBPATH)/static/Gui/,$(notdir $(CPP_FILES_GUI:.cpp=.o)))
DYNAMIC_OBJ_FILES_GUI := $(addprefix $(LIBPATH)/dynamic/Gui/,$(notdir $(CPP_FILES_GUI:.cpp=.o)))

# append targets
ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-gui.dll $(LIBPATH)/libf-gui-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-gui.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_GUI)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_GUI)

###
# directory targets
###
$(LIBPATH)/static/Gui: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) Gui

$(LIBPATH)/dynamic/Gui: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) Gui

###
# static and dynamic targets
###
$(LIBPATH)/libf-gui.a: $(STATIC_OBJ_FILES_GUI) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-gui.dll $(LIBPATH)/libf-gui-dll.a: $(LIBPATH)/libf-system-dll.a $(LIBPATH)/libf-graphics-dll.a $(LIBPATH)/libf-gl-dll.a $(LIBPATH)/libf-freetype-dll.a $(DYNAMIC_OBJ_FILES_GUI) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-gui.dll $(DYNAMIC_OBJ_FILES_GUI) -L $(LIBPATH) -lf-system-dll -lf-graphics-dll -lf-gl-dll -lf-freetype-dll -lOpenGL32 -lgdi32 -Wl,--out-implib,$(LIBPATH)/libf-gui-dll.a

###
# object targets
###
$(LIBPATH)/static/Gui/%.o: $(PATH_TO_SRC)/Gui/%.cpp | $(LIBPATH)/static/Gui
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/Gui/%.o: $(PATH_TO_SRC)/Gui/%.cpp | $(LIBPATH)/dynamic/Gui
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
