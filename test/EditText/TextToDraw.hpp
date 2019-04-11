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
#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/Graphics/Font.hpp>

#include <deque>

namespace fgui
{
    namespace priv
    {
        
        float getCharWidth(fg::Font font,fm::Uint32 cp,fg::Glyph *retGlyph = nullptr,bool monoSpace = false,fm::Size tabSize = 4);

        void getDrawFromText(std::vector<fg::FontSprite> &sprites,
                             const std::deque<fm::String> &lines,
                             fg::Font font,
                             fm::Size charSize,
                             fm::rect2i viewRect,
                             fm::vec4 color,
                             bool monospace,
                             fm::Size spacesInTab = 4,
                             fm::String notDrawn = "\n\r\b");
    }
}
