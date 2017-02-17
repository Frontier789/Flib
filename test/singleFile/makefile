FPATH=../..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

CPP_FILES=$(notdir $(wildcard *.cpp))
EXE_FILES=$(CPP_FILES:.cpp=$(F_EXEC_EXT))
F_PROJ_MK=$(FPATH)/include/FRONTIER/Makefiles/fproject.mk

ifeq ($(F_PLATFORM),Windows)
 F_O_DIR=libW
else
 ifeq ($(F_PLATFORM),Linux)
  F_O_DIR=libL
 else
  ifeq ($(F_PLATFORM),Android)
  F_O_DIR=libA
  endif
 endif
endif

# for every cpp file call make w/ fproject.mk on it
all: $(EXE_FILES)

%$(F_EXEC_EXT): %.cpp
	$(MAKE) --no-print-directory -f $(F_PROJ_MK) F_CPP_FILES=$< EXEC=$(@:.exe=) FPATH=$(FPATH) CXX=$(CXX)

clean:
	$(RRM) $(F_O_DIR) && $(RM) $(CPP_FILES:.cpp=$(F_EXEC_EXT))
