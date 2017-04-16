include $(dir $(lastword $(MAKEFILE_LIST)))/common.mk

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

override F_LINK_LIBS:=-lf $(F_DEF_LINK_LIBS) $(F_LINK_LIBS)

# compile falgse
CXXFLAGS=-std=gnu++11 -pedantic -Wall

# linker flags
ifeq ($(LDFLAGS),)
 ifeq ($(F_DEBUG),)
  LDFLAGS=-s
 endif
endif

# compile falgs
ifeq ($(F_DEBUG),)
 CXXFLAGS+=-O3
endif

# executable name
ifeq ($(EXEC),)
 EXEC=exec
endif

# path to flib root
ifeq ($(FPATH),)
 FPATH=..
endif

# cpp files to compile
ifeq ($(F_CPP_FILES),)
 F_CPP_FILES=$(notdir $(wildcard *.cpp))
endif

TARGET=$(EXEC)$(F_EXEC_EXT)
O_FILES=$(addprefix $(F_O_DIR)/,$(notdir $(F_CPP_FILES:.cpp=.o)))

all: $(TARGET)

run: all
	$(TARGET)

clean: clean_o_dir clean_exec

$(TARGET): $(O_FILES) $(FPATH)/$(F_LIB_DIR_NAME)/libf.a
	$(CXX) $(LDFLAGS) -L $(FPATH)/$(F_LIB_DIR_NAME) -o $(TARGET) $(O_FILES) $(F_LINK_LIBS)

$(F_O_DIR)/%.o: %.cpp | $(F_O_DIR)
	$(CXX) $(CXXFLAGS) -I $(FPATH)/include -c $< -o $@ 

$(F_O_DIR):
	$(call F_MKDIR,$(F_O_DIR))

clean_o_dir:
	$(call F_DELETE,$(F_O_DIR))

clean_exec:
	$(F_RM) $(TARGET)
