#files
CPP_FILES_SYSTEM := $(wildcard $(SRC)/System/*.cpp)
OBJ_FILES_SYSTEM := $(addprefix $(TARGETDIR)/System/,$(notdir $(CPP_FILES_SYSTEM:.cpp=.o)))

SYSTEM_TARGET: $(TARGETDIR)/libf-system.a
SYSTEM_DIR: $(TARGETDIR)/System

#created files
RESULT_FILES := $(OBJ_FILES_SYSTEM) $(TARGETDIR)/libf-system.a

###
# directory targets
###
$(TARGETDIR)/System: $(TARGETDIR)
	$(CD) $(PATHTOROOT); $(CD) lib$(LIBNAME); $(MKDIR) System

###
# source targets
###
$(TARGETDIR)/libf-system.a: $(OBJ_FILES_SYSTEM)
	$(AR) rcs $@ $^

$(TARGETDIR)/System/%.o: $(SRC)/System/%.cpp | $(TARGETDIR)/System
	$(CXX) $(CPPFLAGS) -I  $(INCLUDE) -c $< -o $@

###
# clean_targets
###
clean-system:
	$(RM) $(OBJ_FILES_SYSTEM) $(TARGETDIR)/libf-system.a
