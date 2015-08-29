#files
CPP_FILES_UTIL := $(wildcard $(PATH_TO_SRC)/Util/*.cpp)
STATIC_OBJ_FILES_UTIL  := $(addprefix $(LIBPATH)/static/Util/,$(notdir $(CPP_FILES_UTIL:.cpp=.o)))
DYNAMIC_OBJ_FILES_UTIL := $(addprefix $(LIBPATH)/dynamic/Util/,$(notdir $(CPP_FILES_UTIL:.cpp=.o)))

# append targets
ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-util.dll $(LIBPATH)/libf-util-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-util.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_UTIL)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_UTIL)

###
# directory targets
###
$(LIBPATH)/static/Util: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) Util

$(LIBPATH)/dynamic/Util: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) Util

###
# static and dynamic targets
###
$(LIBPATH)/libf-util.a: $(STATIC_OBJ_FILES_UTIL) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-util.dll $(LIBPATH)/libf-util-dll.a: $(DYNAMIC_OBJ_FILES_UTIL) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-util.dll $(DYNAMIC_OBJ_FILES_UTIL) -lf-system-dll -Wl,--out-implib,$(LIBPATH)/libf-util-dll.a

###
# object targets
###
$(LIBPATH)/static/Util/%.o: $(PATH_TO_SRC)/Util/%.cpp | $(LIBPATH)/static/Util
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/Util/%.o: $(PATH_TO_SRC)/Util/%.cpp | $(LIBPATH)/dynamic/Util
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
