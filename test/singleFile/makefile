CPP_FILES=$(notdir $(wildcard *.cpp))
F_PROJ_MK=../../include/FRONTIER/Makefiles/fproject.mk

include ../../include/FRONTIER/Makefiles/common.mk

all:
	$(foreach cpp_file,$(CPP_FILES),$(MAKE) --no-print-directory -f $(F_PROJ_MK) F_CPP_FILES=$(cpp_file) EXEC=$(cpp_file:.cpp=) FPATH=../..;)

clean:
	$(RRM) lib? && $(RM) $(CPP_FILES:.cpp=$(F_EXEC_EXT))