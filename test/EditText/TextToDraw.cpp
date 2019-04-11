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
#include "TextToDraw.hpp"
#include <iostream>
using namespace std;

namespace fgui
{
    namespace priv
    {
        float getCharWidth(fg::Font font,fm::Uint32 cp,fg::Glyph *retGlyph,bool monoSpace,fm::Size tabSize)
        {
            if (!font)
                return 0;

            if (cp == ' ')
                return font.getGlyph('p').size.w;

            if (cp == '\t')
                return font.getGlyph('p').size.w*tabSize;

            fg::Glyph glyph = font.getGlyph(cp);

            if (retGlyph)
                *retGlyph = glyph;

            return monoSpace ? font.getGlyph('w').size.w : glyph.size.w;
        }
        
        void getDrawFromText(std::vector<fg::FontSprite> &sprites,
                             const std::deque<fm::String> &lines,
                             fg::Font font,
                             fm::Size charSize,
                             fm::rect2i viewRect,
                             fm::vec4 color,
                             bool monospace,
                             fm::Size spacesInTab,
                             fm::String notDrawn)
        // void getDrawFromText(fg::DrawData &ret,
        //                      const fm::String **lines,
        //                      fm::Size linesCount,
        //                      const fg::Font *font,
        //                      fm::Size charSize,
        //                      const fg::Texture **retTex,
        //                      fm::vec2 *retSize,
        //                      fm::rect2i viewRect,
        //                      fm::vec4 color,
        //                      int spacing, /* 0 tight  1 mono */
        //                      fm::Size spacesInTab,
        //                      const fm::String &eol,
        //                      const fm::String &notDrawn)
        {
            auto linesCount = lines.size();
            
            // check input
            if (!font) return;
            if (!linesCount) return;

            // prepare variables
            font.setCharacterSize(charSize);
            fg::Metrics metrics = font.getMetrics();

            fm::vec2 size;
            fm::Size quadId = 0;

            bool limitedView = viewRect.size.area() != 0;
            
            sprites.clear();

            fm::vec2 cursor(0,metrics.minH);

            std::vector<float> lineWidths;
            fm::String eol = "\n";

            // loop through all characters
            for (auto text : lines)
            {
                if (limitedView && cursor.y > viewRect.pos.y + viewRect.size.h)
                    break;

                if (!limitedView || cursor.y + metrics.lineGap > viewRect.pos.y)
                {
                    C(text.size())
                    {
                        if (notDrawn.find(text[i]) != fm::String::npos)
                            continue;

                        if (eol.size() && text.find(eol,i,1) == i)
                        {
                            cursor.x = 0;
                            cursor.y += metrics.lineGap;

                            if (limitedView && cursor.y > viewRect.pos.y + viewRect.size.h)
                                break;

                            i += eol.size()-1;
                            continue;
                        }

                        // check for whitespace
                        if (text[i] == ' ' || text[i] == '\t')
                        {
                            cursor.x += getCharWidth(font,text[i],nullptr,monospace,spacesInTab);
                            continue;
                        }


                        fg::Glyph g;

                        // find advance to the next character
                        float xadvance = getCharWidth(font,text[i],&g,monospace,spacesInTab);

                        if (limitedView && cursor.x + xadvance < viewRect.pos.x)
                        {
                            cursor.x += xadvance;
                            continue;
                        }

                        if (limitedView && cursor.x > viewRect.pos.x + viewRect.size.w)
                            break;
                        
                        auto sprite = font.getSprite(text[i],fg::Glyph::Regular);
                        {
                            fm::vec2 p = cursor - viewRect.pos;
                            p.y += metrics.lineGap + g.leftdown.y;
                            p.x += int((xadvance - sprite.getSize().w)/2);

                            sprite.setPosition(p);
                            sprite.setColor(color);
                        }

                        // advance variables
                        quadId++;
                        cursor.x += xadvance;

                        // keep size
                        fm::vec2 corner = sprite.getPosition() + sprite.getSize(); 
                        if (size.w < corner.x) size.w = corner.x;
                        if (size.h < corner.y) size.h = corner.y;
                        
                        sprites.emplace_back(std::move(sprite));
                    }
                }

                cursor.x = 0;
                cursor.y += metrics.lineGap;
            }

            // if (retSize) *retSize = limitedView ? (fm::vec2)viewRect.size : size;
        }
    }
}
