////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/Graphics/Font.hpp>

namespace fgui
{
    namespace priv
    {
        float getCharWidth(const fg::Font *font,fm::Uint32 cp,fg::Glyph *retGlyph = fm::nullPtr,bool monoSpace = false,fm::Size tabSize = 4);

        void getDrawFromText(fg::DrawData &ret,
                             const fm::String **lines,
                             fm::Size linesCount,
                             const fg::Font *font,
                             fm::Size charSize,
                             const fg::Texture **retTex = fm::nullPtr,
                             fm::vec2 *retSize = fm::nullPtr,
                             fm::rect2i viewRect = fm::rect2i(),
                             fm::vec4 color = fm::vec4::White,
                             int spacing = 0, /* 0 tight  1 mono */
                             fm::Size spacesInTab = 4,
                             const fm::String &newLine = "\n",
                             const fm::String &notDrawn = "\n\r\b");
    }
}
