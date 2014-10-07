#files
CPP_FILES_WINDOW := $(wildcard $(PATH_TO_SRC)/Window/*.cpp)
OBJ_FILES_WINDOW := $(addprefix $(LIBPATH)/Window/,$(notdir $(CPP_FILES_WINDOW:.cpp=.o)))

TARGETS+=$(LIBPATH)/libf-window.a
DIR_TARGETS+=$(LIBPATH)/Window
OBJ_FILES+=$(OBJ_FILES_WINDOW)

###
# directory targets
###
$(LIBPATH)/Window: | $(LIBPATH)
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(MKDIR) Window

###
# source targets
###
$(LIBPATH)/libf-window.a: $(OBJ_FILES_WINDOW)
	$(AR) rcs $@ $^

$(LIBPATH)/Window/%.o: $(PATH_TO_SRC)/Window/%.cpp | $(LIBPATH)/Window
	$(CXX) $(CPPFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@

###
# clean_targets
###
clean-window:
	$(RM) $(OBJ_FILES_WINDOW) $(LIBPATH)/libf-window.a
