CPP_FILES=$(notdir $(wildcard *.cpp))
F_PROJ_MK=../../include/FRONTIER/Makefiles/fproject.mk

all:
	$(foreach cpp_file,$(CPP_FILES),$(MAKE) --no-print-directory -f $(F_PROJ_MK) F_CPP_FILES=$(cpp_file) EXEC=$(cpp_file:.cpp=) FPATH=../..;)
