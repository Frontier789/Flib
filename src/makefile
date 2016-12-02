# path to flib
F_PATH_TO_ROOT=..
F_PATH_TO_INCLUDE=$(F_PATH_TO_ROOT)/include

# target library TODO: check opoeraing system
F_LIB_TARGET=libWin
F_O_FILES_DIR=$(F_PATH_TO_ROOT)/$(F_LIB_TARGET)

# compilation tools and settings
CXX=g++
CXXFLAGS=-std=gnu++11

# common tools
MKDIR=mkdir
AR=ar rcs
CD=cd

# LowerCase function
F_LOWERCASE=$(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

# include System module
F_CURRENT_MODULE=System
include generic.mk
