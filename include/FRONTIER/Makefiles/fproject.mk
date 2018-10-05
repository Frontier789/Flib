include $(dir $(lastword $(MAKEFILE_LIST)))common.mk

# platform dependent libs
ifeq ($(F_PLATFORM),Windows)
 F_DEF_LINK_LIBS+=-lOpenGL32 -lgdi32 -lwinmm -lws2_32
 F_O_DIR=libW
else
 ifeq ($(F_PLATFORM),Linux)
  F_DEF_LINK_LIBS+=-lGL -lX11 -lpthread
  F_O_DIR=libL
 else
  ifeq ($(F_PLATFORM),Android)
  F_DEF_LINK_LIBS+=-lpthread
  F_O_DIR=libA
  endif
 endif
endif

# determine where o files go
F_BUILD_DIR?=build
ifeq ($(F_BUILD_DIR),.)
	F_O_PATH=$(F_O_DIR)
else
	F_O_PATH=$(F_BUILD_DIR)/$(F_O_DIR)
endif

# what tpye of flib build to use
ifeq ($(F_DYNLINK),1)
 override F_LINK_LIBS:=-lf-shared $(F_DEF_LINK_LIBS) $(F_LINK_LIBS)
 F_NEEDED_LIB=libf-shared$(F_SO_USEDTARGET)
else
 override F_LINK_LIBS:=-lf $(F_DEF_LINK_LIBS) $(F_LINK_LIBS)
 F_NEEDED_LIB=libf.a
endif

# executable name
EXEC?=exec

# path to flib root
ifeq ($(wildcard $(FPATH)),)
 $(info FPATH automatically set to ..)
 FPATH=..
endif

# cpp files to compile
CPP_FILES?=*.cpp
F_CPP_FILES?=$(wildcard $(CPP_FILES))

TARGET=$(EXEC)$(F_EXEC_EXT)
O_FILES=$(addprefix $(F_O_PATH)/,$(notdir $(F_CPP_FILES:.cpp=.o)))

all: $(TARGET)

run: all
	$(TARGET)

run_debug: all
	gdb $(TARGET) -ex start -ex continue

clean: clean_o_dir clean_exec

$(TARGET): $(O_FILES) $(FPATH)/$(F_LIB_DIR_NAME)/$(F_NEEDED_LIB)
	$(CXX) $(LDFLAGS) -L $(FPATH)/$(F_LIB_DIR_NAME) -o $(TARGET) $(O_FILES) $(F_LINK_LIBS)

$(foreach TUNIT,$(basename $(F_CPP_FILES)), \
 $(eval $(F_O_PATH)/$(notdir $(TUNIT)).o: $(TUNIT).cpp | $(F_O_PATH); \
	$(CXX) $(CXXFLAGS) -I $(FPATH)/include -c $(TUNIT).cpp -o $(F_O_PATH)/$(notdir $(TUNIT)).o) \
)

$(F_O_PATH):
	$(call F_MKDIR,$(F_O_PATH))

clean_o_dir:
	$(call F_DELETE,$(F_O_PATH))

clean_exec:
	$(F_RM) $(TARGET)
