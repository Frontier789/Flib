#files
CPP_FILES_SYSTEM := $(wildcard $(PATH_TO_SRC)/System/*.cpp)
STATIC_OBJ_FILES_SYSTEM  := $(addprefix $(LIBPATH)/static/System/,$(notdir $(CPP_FILES_SYSTEM:.cpp=.o)))
DYNAMIC_OBJ_FILES_SYSTEM := $(addprefix $(LIBPATH)/dynamic/System/,$(notdir $(CPP_FILES_SYSTEM:.cpp=.o)))

ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-system.dll $(LIBPATH)/libf-system-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-system.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_SYSTEM)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_SYSTEM)

###
# directory targets
###
$(LIBPATH)/static/System: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) System

$(LIBPATH)/dynamic/System: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) System

###
# static and dynamic targets
###
$(LIBPATH)/libf-system.a: $(STATIC_OBJ_FILES_SYSTEM) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-system.dll $(LIBPATH)/libf-system-dll.a: $(DYNAMIC_OBJ_FILES_SYSTEM) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-system.dll $(DYNAMIC_OBJ_FILES_SYSTEM) -Wl,--out-implib,$(LIBPATH)/libf-system-dll.a

###
# object targets
###
$(LIBPATH)/static/System/%.o: $(PATH_TO_SRC)/System/%.cpp | $(LIBPATH)/static/System
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/System/%.o: $(PATH_TO_SRC)/System/%.cpp | $(LIBPATH)/dynamic/System
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
