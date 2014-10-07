#files
CPP_FILES_SYSTEM := $(wildcard $(PATH_TO_SRC)/System/*.cpp)
OBJ_FILES_SYSTEM := $(addprefix $(LIBPATH)/System/,$(notdir $(CPP_FILES_SYSTEM:.cpp=.o)))

TARGETS+=$(LIBPATH)/libf-system.a
DIR_TARGETS+=$(LIBPATH)/System
OBJ_FILES+=$(OBJ_FILES_SYSTEM)

###
# directory targets
###
$(LIBPATH)/System: | $(LIBPATH)
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(MKDIR) System

###
# source targets
###
$(LIBPATH)/libf-system.a: $(OBJ_FILES_SYSTEM)
	$(AR) rcs $@ $^

$(LIBPATH)/System/%.o: $(PATH_TO_SRC)/System/%.cpp | $(LIBPATH)/System
	$(CXX) $(CPPFLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@

###
# clean_targets
###
clean-system:
	$(RM) $(OBJ_FILES_SYSTEM) $(LIBPATH)/libf-system.a
