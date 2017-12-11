WD=$(dir $(lastword $(MAKEFILE_LIST)))

include $(WD)/detect_os.mk

test_cmd = $(strip $(shell make -f $(WD)/testCmd.mk TEST_CMD=$(1) > /dev/null 2>&1 && echo 1 || echo 0))

HAS_GCC := $(call test_cmd,"g++ -v")
HAS_CLANG := $(call test_cmd,"clang++ -v")

# default compilation tools and settings
CXX ?= g++
CXXFLAGS ?= -std=gnu++11 -pedantic -Werror -Wextra -Wall -DFRONTIER_DEBUG
CXX_WNO_FLAGS ?= -DFRONTIER_MISSING_IMPL_NO_WARNING

ifeq ($(CXX),g++)
 ifeq ($(HAS_GCC),0)
  override CXX:=clang++
 endif
endif

ifeq ($(CXX),clang++)
 ifeq ($(HAS_CLANG),0)
  override CXX:=g++
 endif
endif

ifeq ($(CXX),g++)
 ifeq ($(HAS_GCC),0)
error:
	$(error no compiler specified and g++ and clang++ not available)
 endif
endif

# common tools
MAKE=make
MKDIR=mkdir
RRM=$(F_RRM)
RM=$(F_RM)
AR=ar rcs
CD=cd

# LowerCase and UpperCase function
F_LOWERCASE=$(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))
F_UPPERCASE=$(subst a,A,$(subst b,B,$(subst c,C,$(subst d,D,$(subst e,E,$(subst f,F,$(subst g,G,$(subst h,H,$(subst i,I,$(subst j,J,$(subst k,K,$(subst l,L,$(subst m,M,$(subst n,N,$(subst o,O,$(subst p,P,$(subst q,Q,$(subst r,R,$(subst s,S,$(subst t,T,$(subst u,U,$(subst v,V,$(subst w,W,$(subst x,X,$(subst y,Y,$(subst z,Z,$1))))))))))))))))))))))))))
