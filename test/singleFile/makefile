FPATH=../..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

CPP_FILES=$(notdir $(wildcard *.cpp))
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

define EOL


endef

# for every cpp file call make w/ fproject.mk on it
all:
	$(foreach cpp_file,$(CPP_FILES),$(MAKE) -j4 --no-print-directory -f $(F_PROJ_MK) F_CPP_FILES=$(cpp_file) EXEC=$(cpp_file:.cpp=) FPATH=$(FPATH) CXX=$(CXX) ${EOL})

clean:
	$(RRM) $(F_O_DIR) && $(RM) $(CPP_FILES:.cpp=$(F_EXEC_EXT))