include $(dir $(lastword $(MAKEFILE_LIST)))/detect_os.mk

ifeq ($(PLATFORM),Windows) 
 EXEC_EXT=.exe
 OBJDIR=Wobj
 LIBS=-lOpenGL32 -lgdi32
else
 ifeq ($(PLATFORM),Linux)
  EXEC_EXT=
  OBJDIR=Lobj
  LIBS=-lGL -lX11 -lpthread
 else
  ifeq ($(PLATFORM),Android)
  EXEC_EXT=
  OBJDIR=Aobj
  LIBS=-lpthread
  endif
 endif
endif

LIBS:=-lf $(LIBS)
CXX=g++
MKDIR=mkdir

ifeq ($(LINKFLAGS),)
 LINKFLAGS=-s
endif

ifeq ($(COMPILEFLAGS),)
 COMPILEFLAGS=-O3 -pedantic -Werror -Wno-long-long
endif

ifeq ($(EXEC),)
 EXEC=exec
endif

ifeq ($(FPATH),)
 FPATH=..
endif

ifeq ($(CPP_FILES),)
 CPP_FILES=$(notdir $(wildcard *.cpp))
endif

TARGET=$(EXEC)$(EXEC_EXT)
O_FILES=$(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: $(TARGET)

$(TARGET): $(O_FILES) $(FPATH)/$(LIBNAME)/libf.a
	$(CXX) $(LINKFLAGS) -L $(FPATH)/$(LIBNAME) -o $(TARGET) $(O_FILES) $(LIBS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(COMPILEFLAGS) -I $(FPATH)/include -c $< -o $@

$(OBJDIR):
	$(MKDIR) $(OBJDIR)