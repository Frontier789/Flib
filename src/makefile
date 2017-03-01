# path to flib
F_PATH_TO_ROOT=..
F_PATH_TO_INCLUDE=$(F_PATH_TO_ROOT)/include

# include common definitions
include $(F_PATH_TO_INCLUDE)/FRONTIER/Makefiles/common.mk

# target library TODO: check opoeraing system
F_LIB_TARGET=$(F_LIB_DIR_NAME)
F_O_FILES_DIR=$(F_PATH_TO_ROOT)/$(F_LIB_TARGET)

# all the .a targets
all: make_lib_files make_lib_f

# all the .a targets
clean: del_lib

####  ####  ####  #####  ####  ####  ####
#                                       #
#          directory targets            #
#                                       #
####  ####  ####  #####  ####  ####  ####

$(F_O_FILES_DIR):
	$(CD) $(F_PATH_TO_ROOT) && $(MKDIR) $(F_LIB_TARGET)

$(F_O_FILES_DIR)/static: | $(F_O_FILES_DIR)
	$(CD) $(F_O_FILES_DIR) && $(MKDIR) static

F_MODULES=Graphics System Window GL Gui

####  ####  ####  ## generating targets ##  ####  ####  ####

$(foreach MODULE,$(F_MODULES), \
 $(eval F_CUR_MODULE=$(call F_UPPERCASE,$(MODULE))) \
 \
 $(eval F_$(F_CUR_MODULE)_CPP_FILES=$(wildcard $(MODULE)/*.cpp)) \
 $(eval F_$(F_CUR_MODULE)_O_FILES=$(addprefix $(F_O_FILES_DIR)/static/$(MODULE)/,$(notdir $(F_$(F_CUR_MODULE)_CPP_FILES:.cpp=.o)))) \
 $(eval F_$(F_CUR_MODULE)_MODULE_A=$(F_O_FILES_DIR)/libf-$(call F_LOWERCASE,$(MODULE)).a) \
 \
 $(eval F_STATIC_O_FILES+=$(F_$(F_CUR_MODULE)_O_FILES)) \
 $(eval F_MODULE_A_FILES+=$(F_$(F_CUR_MODULE)_MODULE_A)) \
 \
 $(eval $(F_O_FILES_DIR)/static/$(MODULE): | $(F_O_FILES_DIR)/static ; \
	$(CD) $(F_O_FILES_DIR) && $(CD) static && $(MKDIR) $(MODULE)) \
 \
 $(eval $(F_$(F_CUR_MODULE)_MODULE_A): $(F_$(F_CUR_MODULE)_O_FILES) ; \
	$(AR) $$@ $$^ ) \
 \
 $(eval $(F_O_FILES_DIR)/static/$(MODULE)/%.o: $(MODULE)/%.cpp | $(F_O_FILES_DIR)/static/$(MODULE) ; \
	$(CXX) $(CXXFLAGS) -I $(F_PATH_TO_INCLUDE) -c $$< -o $$@ ) \
)

####  ####  ####  ##                    ##  ####  ####  ####

make_lib_files: $(F_MODULE_A_FILES)

make_lib_f: $(F_O_FILES_DIR)/libf.a

$(F_O_FILES_DIR)/libf.a: $(F_STATIC_O_FILES) | $(F_O_FILES_DIR)
	$(AR) $@ $^
	
	
del_lib:
	$(call F_DELETE,$(F_O_FILES_DIR))