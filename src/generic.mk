F_CURRENT_MODULE_A=$(F_O_FILES_DIR)/libf-$(call F_LOWERCASE,$(F_CURRENT_MODULE)).a

F_CPP_FILES=$(wildcard $(F_CURRENT_MODULE)/*.cpp)
F_CURRENT_STATIC_O_FILES=$(F_O_FILES_DIR)/static/$(F_CURRENT_MODULE)/$(notdir $(F_CPP_FILES:.cpp=.o))

F_STATIC_O_FILES+=$(F_CURRENT_STATIC_O_FILES)
F_MODULE_A_FILES+=$(F_CURRENT_MODULE_A)

####  ####  ####  #####  ####  ####  ####
#                                       #
#            library target             #
#                                       #
####  ####  ####  #####  ####  ####  ####

$(F_CURRENT_MODULE_A): $(F_CURRENT_STATIC_O_FILES)
	$(AR) $@ $^

####  ####  ####  #####  ####  ####  ####
#                                       #
#          directory targets            #
#                                       #
####  ####  ####  #####  ####  ####  ####

$(F_O_FILES_DIR):
	$(MKDIR) $(F_O_FILES_DIR)
	
$(F_O_FILES_DIR)/static/$(F_CURRENT_MODULE): | $(F_O_FILES_DIR)/static
	$(CD) $(F_O_FILES_DIR) && $(CD) static && $(MKDIR) $(F_CURRENT_MODULE)

$(F_O_FILES_DIR)/static: | $(F_O_FILES_DIR)
	$(CD) $(F_O_FILES_DIR) && $(MKDIR) static

####  ####  ####  #####  ####  ####  ####
#                                       #
#             file targets              #
#                                       #
####  ####  ####  #####  ####  ####  ####
 
$(F_O_FILES_DIR)/static/$(F_CURRENT_MODULE)/%.o: $(F_CURRENT_MODULE)/%.cpp | $(F_O_FILES_DIR)/static/$(F_CURRENT_MODULE)
	$(CXX) $(CXXFLAGS) -I $(F_PATH_TO_INCLUDE) -c $< -o $@








