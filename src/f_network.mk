#files
CPP_FILES_NETWORK := $(wildcard $(PATH_TO_SRC)/Network/*.cpp)
STATIC_OBJ_FILES_NETWORK  := $(addprefix $(LIBPATH)/static/Network/,$(notdir $(CPP_FILES_NETWORK:.cpp=.o)))
DYNAMIC_OBJ_FILES_NETWORK := $(addprefix $(LIBPATH)/dynamic/Network/,$(notdir $(CPP_FILES_NETWORK:.cpp=.o)))

# append targets
ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-network.dll $(LIBPATH)/libf-network-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-network.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_NETWORK)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_NETWORK)

###
# directory targets
###
$(LIBPATH)/static/Network: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) Network

$(LIBPATH)/dynamic/Network: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) Network

###
# static and dynamic targets
###
$(LIBPATH)/libf-network.a: $(STATIC_OBJ_FILES_NETWORK) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-network.dll $(LIBPATH)/libf-network-dll.a: $(DYNAMIC_OBJ_FILES_NETWORK) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-network.dll $(DYNAMIC_OBJ_FILES_NETWORK) -Wl,--out-implib,$(LIBPATH)/libf-network-dll.a

###
# object targets
###
$(LIBPATH)/static/Network/%.o: $(PATH_TO_SRC)/Network/%.cpp | $(LIBPATH)/static/Network
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@
	
$(LIBPATH)/dynamic/Network/%.o: $(PATH_TO_SRC)/Network/%.cpp | $(LIBPATH)/dynamic/Network
	$(CXX) $(CXXFLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@ 
