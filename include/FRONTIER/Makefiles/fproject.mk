include $(dir $(lastword $(MAKEFILE_LIST)))/common.mk

ifeq ($(F_PLATFORM),Windows)
 F_LINK_LIBS+=-lOpenGL32 -lgdi32 -lwinmm -lws2_32
 F_O_DIR=libW
else
 ifeq ($(F_PLATFORM),Linux)
  F_LINK_LIBS+=-lGL -lX11 -lpthread
  F_O_DIR=libL
 else
  ifeq ($(F_PLATFORM),Android)
  F_LINK_LIBS+=-lpthread
  F_O_DIR=libA
  endif
 endif
endif

F_LINK_LIBS:=-lf $(F_LINK_LIBS)

ifeq ($(LDFLAGS),)
 LDFLAGS=-s
endif

ifeq ($(CXXFLAGS),)
 CXXFLAGS=-std=gnu++11 -O3 -pedantic -Werror
endif

ifeq ($(EXEC),)
 EXEC=exec
endif

ifeq ($(FPATH),)
 FPATH=..
endif

ifeq ($(F_CPP_FILES),)
 F_CPP_FILES=$(notdir $(wildcard *.cpp))
endif

TARGET=$(EXEC)$(F_EXEC_EXT)
O_FILES=$(addprefix $(F_O_DIR)/,$(notdir $(F_CPP_FILES:.cpp=.o)))

all: $(TARGET)

$(TARGET): $(O_FILES) $(FPATH)/$(F_LIB_DIR_NAME)/libf.a
	$(CXX) $(LDFLAGS) -L $(FPATH)/$(F_LIB_DIR_NAME) -o $(TARGET) $(O_FILES) $(LIBS)

$(F_O_DIR)/%.o: %.cpp | $(F_O_DIR)
	$(CXX) $(CXXFLAGS) -I $(FPATH)/include -c $< -o $@

$(F_O_DIR):
	$(MKDIR) $(F_O_DIR)