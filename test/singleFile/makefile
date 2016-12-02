FPATH=../..
include $(FPATH)/include/FRONTIER/Makefiles/common.mk

CPP_FILES=$(notdir $(wildcard *.cpp))
F_PROJ_MK=$(FPATH)/include/FRONTIER/Makefiles/fproject.mk

all:
	$(foreach cpp_file,$(CPP_FILES),$(MAKE) --no-print-directory -f $(F_PROJ_MK) F_CPP_FILES=$(cpp_file) EXEC=$(cpp_file:.cpp=) FPATH=$(FPATH) CXX=$(CXX) CXXFLAGS=$(CXXFAGS) LDFLAGS=$(LDFLAGS);)

clean:
	$(RRM) lib? && $(RM) $(CPP_FILES:.cpp=$(F_EXEC_EXT))