# path to flib
F_PATH_TO_ROOT=..
F_PATH_TO_INCLUDE=$(F_PATH_TO_ROOT)/include

# include common definitions
include $(F_PATH_TO_INCLUDE)/FRONTIER/Makefiles/common.mk

# target library
F_LIB_TARGET=$(F_LIB_DIR_NAME)
F_O_FILES_DIR=$(F_PATH_TO_ROOT)/$(F_LIB_TARGET)

ifeq ($(F_PLATFORM),Linux)
RET := $(shell scripts/testReqs.sh $(CXX) > /dev/null; echo $$?)

ifneq ($(RET),0)
err:
	$(shell scripts/testReqs.sh $(CXX) >&2)
endif
endif

# all the .a targets
all: make_lib_files make_lib_f

# all the static targets
static: make_lib_files make_lib_f

# all the dynamic targets
dynamic: make_so_files make_so_f

# del all the targets
clean: del_f

####  ####  ####  #####  ####  ####  ####
#                                       #
#          directory targets            #
#                                       #
####  ####  ####  #####  ####  ####  ####

$(F_O_FILES_DIR):
	$(CD) $(F_PATH_TO_ROOT) && $(MKDIR) $(F_LIB_TARGET)

$(F_O_FILES_DIR)/static: | $(F_O_FILES_DIR)
	$(CD) $(F_O_FILES_DIR) && $(MKDIR) static

$(F_O_FILES_DIR)/dynamic: | $(F_O_FILES_DIR)
	$(CD) $(F_O_FILES_DIR) && $(MKDIR) dynamic

include module_defs.mk

####  ####  ####  ## generating targets ##  ####  ####  ####
	
$(foreach MODULE,$(F_MODULES), \
 $(eval F_CUR_MODULE=$(call F_UPPERCASE,$(MODULE))) \
 $(eval F_CUR_MODULE_LOWERCASE=$(call F_LOWERCASE,$(MODULE))) \
 \
 $(eval F_$(F_CUR_MODULE)_CPP_FILES=$(wildcard $(MODULE)/*.cpp)) \
 $(eval F_CUR_O_FILES=$(notdir $(F_$(F_CUR_MODULE)_CPP_FILES:.cpp=.o))) \
 \
 $(eval F_$(F_CUR_MODULE)_STATIC_O_FILES=$(addprefix $(F_O_FILES_DIR)/static/$(MODULE)/,$(F_CUR_O_FILES))) \
 $(eval F_$(F_CUR_MODULE)_DYNAMIC_O_FILES=$(addprefix $(F_O_FILES_DIR)/dynamic/$(MODULE)/,$(F_CUR_O_FILES))) \
 $(eval F_$(F_CUR_MODULE)_MODULE_A=$(F_O_FILES_DIR)/libf-$(F_CUR_MODULE_LOWERCASE).a) \
 $(eval F_$(F_CUR_MODULE)_MODULE_SO=$(F_O_FILES_DIR)/$(F_SO_PREFIX)f-$(F_CUR_MODULE_LOWERCASE)-shared$(F_SO_EXTENSION)) \
 \
 $(eval F_STATIC_O_FILES+=$(F_$(F_CUR_MODULE)_STATIC_O_FILES)) \
 $(eval F_DYNAMIC_O_FILES+=$(F_$(F_CUR_MODULE)_DYNAMIC_O_FILES)) \
 $(eval F_MODULE_A_FILES+=$(F_$(F_CUR_MODULE)_MODULE_A)) \
 $(eval F_MODULE_SO_FILES+=$(F_$(F_CUR_MODULE)_MODULE_SO)) \
 \
 $(eval $(MODULE): $(F_$(F_CUR_MODULE)_MODULE_SO)) \
 \
 $(eval $(F_O_FILES_DIR)/static/$(MODULE): | $(F_O_FILES_DIR)/static ; \
	$(CD) $(F_O_FILES_DIR) && $(CD) static && $(MKDIR) $(MODULE)) \
 $(eval $(F_O_FILES_DIR)/dynamic/$(MODULE): | $(F_O_FILES_DIR)/dynamic ; \
	$(CD) $(F_O_FILES_DIR) && $(CD) dynamic && $(MKDIR) $(MODULE)) \
 \
 $(eval $(F_$(F_CUR_MODULE)_MODULE_A): $(F_$(F_CUR_MODULE)_STATIC_O_FILES) ; \
	$(AR) $$@ $$^ ) \
 $(eval $(F_$(F_CUR_MODULE)_MODULE_SO): $(F_$(F_CUR_MODULE)_DYNAMIC_O_FILES) $(F_DYNREQUIRE_$(F_CUR_MODULE)) ; \
	$(CXX) -shared -o $$@ $(F_$(F_CUR_MODULE)_DYNAMIC_O_FILES) -L$(F_O_FILES_DIR) $(F_DYNLINK_TO_$(F_CUR_MODULE)) $(call F_SO_IMPLIB,$(F_O_FILES_DIR),-$(F_CUR_MODULE_LOWERCASE)) ) \
 \
 $(eval $(F_O_FILES_DIR)/static/$(MODULE)/%.o: $(MODULE)/%.cpp | $(F_O_FILES_DIR)/static/$(MODULE) ; \
	$(CXX) $(CXXFLAGS) $(CXX_WNO_FLAGS) -I $(F_PATH_TO_INCLUDE) -c $$< -o $$@ ) \
 \
 $(eval $(F_O_FILES_DIR)/dynamic/$(MODULE)/%.o: $(MODULE)/%.cpp | $(F_O_FILES_DIR)/dynamic/$(MODULE) ; \
	$(CXX) -shared $(F_SO_CXXFLAGS) $(CXXFLAGS) $(CXX_WNO_FLAGS) -I $(F_PATH_TO_INCLUDE) -c $$< -o $$@ ) \
)

####  ####  ####  ##                    ##  ####  ####  ####

make_lib_files: $(F_MODULE_A_FILES)

make_so_files: $(F_MODULE_SO_FILES)

make_lib_f: $(F_O_FILES_DIR)/libf.a

make_so_f: $(F_O_FILES_DIR)/$(F_SO_PREFIX)f-shared$(F_SO_EXTENSION)


$(F_O_FILES_DIR)/libf.a: $(F_STATIC_O_FILES) | $(F_O_FILES_DIR)
	$(AR) $@ $^


$(F_O_FILES_DIR)/$(F_SO_PREFIX)f-shared$(F_SO_EXTENSION): $(F_DYNAMIC_O_FILES) | $(F_O_FILES_DIR)
	$(CXX) -shared -o $@ $^ -L$(F_O_FILES_DIR) $(F_DYNLINK_TO_ALL) $(call F_SO_IMPLIB,$(F_O_FILES_DIR),)
	

del_f:
	$(call F_DELETE,$(F_O_FILES_DIR))
