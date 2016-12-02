# path to flib
F_PATH_TO_ROOT=..
F_PATH_TO_INCLUDE=$(F_PATH_TO_ROOT)/include

# include common definitions
include $(F_PATH_TO_INCLUDE)/FRONTIER/Makefiles/common.mk

# target library TODO: check opoeraing system
F_LIB_TARGET=$(F_LIB_DIR_NAME)
F_O_FILES_DIR=$(F_PATH_TO_ROOT)/$(F_LIB_TARGET)

all: make_lib_files make_lib_f

# include System module
F_CURRENT_MODULE=System
include generic.mk


make_lib_files: $(F_MODULE_A_FILES)

make_lib_f: $(F_O_FILES_DIR)/libf.a

$(F_O_FILES_DIR)/libf.a: $(F_STATIC_O_FILES) | $(F_O_FILES_DIR)
	$(AR) $@ $^