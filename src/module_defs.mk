

F_MODULES=Graphics System Window GL Gui
F_DYNLINK_TO_GRAPHICS=-lf-system-shared -lf-gl-shared
F_DYNREQUIRE_GRAPHICS=System GL

F_DYNLINK_TO_SYSTEM=
F_DYNREQUIRE_SYSTEM=

F_DYNLINK_TO_WINDOW= -lf-system-shared -lf-gl-shared -lf-graphics-shared
F_DYNREQUIRE_WINDOW=System GL Graphics

F_DYNLINK_TO_GUI=-lf-system-shared -lf-gl-shared -lf-graphics-shared -lf-window-shared
F_DYNREQUIRE_GUI=System GL Graphics Window

F_DYNLINK_TO_GL=-lf-system-shared
F_DYNREQUIRE_GL=System

F_DYNLINK_TO_ALL=

# platform dependent libs
ifeq ($(F_PLATFORM),Windows)
 F_DYNLINK_TO_GRAPHICS+=-lOpenGL32 -lgdi32
 F_DYNLINK_TO_SYSTEM+=-lwinmm
 F_DYNLINK_TO_WINDOW+=-lOpenGL32 -lgdi32
 F_DYNLINK_TO_GUI+=-lOpenGL32 -lgdi32
 F_DYNLINK_TO_GL+=-lOpenGL32
 F_DYNLINK_TO_ALL+=-lwinmm -lOpenGL32 -lgdi32
else
 ifeq ($(F_PLATFORM),Linux)

 else
  ifeq ($(F_PLATFORM),Android)

  endif
 endif
endif
