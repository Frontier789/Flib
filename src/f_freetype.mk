# files
C_FILES_FREETYPE := autofit.c bdf.c cff.c ftbase.c ftbitmap.c ftcache.c ftdebug.c ftfstype.c ftgasp.c ftglyph.c ftinit.c ftlzw.c ftstroke.c ftsystem.c smooth.c ftbbox.c ftmm.c ftpfr.c ftsynth.c fttype1.c ftwinfnt.c ftxf86.c ftlcdfil.c ftgxval.c ftotval.c ftpatent.c pcf.c pfr.c psaux.c pshinter.c psmodule.c raster.c sfnt.c truetype.c type1.c type1cid.c type42.c winfnt.c
STATIC_OBJ_FILES_FREETYPE  := $(addprefix $(LIBPATH)/static/FreeType/,$(notdir $(C_FILES_FREETYPE:.c=.o)))
DYNAMIC_OBJ_FILES_FREETYPE := $(addprefix $(LIBPATH)/dynamic/FreeType/,$(notdir $(C_FILES_FREETYPE:.c=.o)))

# append targets
ifeq ($(PLATFORM),Windows)
	DYNAMIC_TARGETS+=$(LIBPATH)/f-freetype.dll $(LIBPATH)/libf-freetype-dll.a
endif
STATIC_TARGETS+=$(LIBPATH)/libf-freetype.a
STATIC_OBJ_FILES+=$(STATIC_OBJ_FILES_FREETYPE)
DYNAMIC_OBJ_FILES+=$(DYNAMIC_OBJ_FILES_FREETYPE)

###
# directory targets
###
$(LIBPATH)/static/FreeType: | $(LIBPATH)/static
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) static && $(MKDIR) FreeType

$(LIBPATH)/dynamic/FreeType: | $(LIBPATH)/dynamic
	$(CD) $(PATH_TO_ROOT) && $(CD) $(LIBNAME) && $(CD) dynamic && $(MKDIR) FreeType

###
# static and dynamic targets
###
$(LIBPATH)/libf-freetype.a: $(STATIC_OBJ_FILES_FREETYPE) | $(LIBPATH)
	$(AR) rcs $@ $^

$(LIBPATH)/f-freetype.dll $(LIBPATH)/libf-freetype-dll.a: $(DYNAMIC_OBJ_FILES_FREETYPE) | $(LIBPATH)
	$(CXX) -shared -o $(LIBPATH)/f-freetype.dll $(DYNAMIC_OBJ_FILES_FREETYPE) -L $(LIBPATH) -Wl,--out-implib,$(LIBPATH)/libf-freetype-dll.a

###
# object targets
###
FREETYPE_CXX_FLAGS=-DFT2_BUILD_LIBRARY -I $(PATH_TO_SRC)/Graphics/FreeType/include/

$(LIBPATH)/static/FreeType/%.o: $(PATH_TO_SRC)/Graphics/FreeType/src/%.c | $(LIBPATH)/static/FreeType
	$(CXX) $(CXXFLAGS) $(FREETYPE_CXX_FLAGS) -I $(PATH_TO_INCLUDE) -c $< -o $@

$(LIBPATH)/dynamic/FreeType/%.o: $(PATH_TO_SRC)/Graphics/FreeType/src/%.c | $(LIBPATH)/dynamic/FreeType
	$(CXX) $(CXXFLAGS) $(FREETYPE_CXX_FLAGS) -I $(PATH_TO_INCLUDE) -DFRONTIER_DYNAMIC -c $< -o $@
