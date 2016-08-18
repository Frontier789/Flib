#include <FRONTIER/Graphics/IndexArrayHolder.hpp>
#include <FRONTIER/System/macros/Size.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/Graphics/Font.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <vector>

namespace fgui
{
    namespace priv
    {
        float getCharWidth(const fg::Font *font,fm::Uint32 cp,fg::Glyph *retGlyph = fm::nullPtr,bool monoSpace = false,fm::Size tabSize = 4)
        {
            if (!font)
                return 0;

            if (cp == ' ')
                return font->getGlyph('w').size.w+1;

            if (cp == '\t')
                return font->getGlyph('w').size.w*tabSize+tabSize;

            fg::Glyph glyph = font->getGlyph(cp);

            if (retGlyph)
                *retGlyph = glyph;

            return monoSpace ? font->getGlyph('w').size.w+1 : glyph.size.w+2;
        }

        void getDrawFromText(fg::DrawData &ret,
                             const fm::String **lines,
                             fm::Size linesCount,
                             const fg::Font *font,
                             fm::Size charSize,
                             const fg::Texture **retTex,
                             fm::vec2 *retSize,
                             fm::rect2i viewRect,
                             fm::vec4 color,
                             int spacing, /* 0 tight  1 mono */
                             fm::Size spacesInTab,
                             const fm::String &newLine,
                             const fm::String &notDrawn)
        {
            // check input
            if (!font) return;
            if (!linesCount) return;

            // prepare variables
            font->setCharacterSize(charSize);

            const fg::Texture *tex = &font->getTexture();
            if (retTex) *retTex = tex;

            fg::Metrics metrics = font->getMetrics();

            fm::vec2 size;
            fm::Size quadId = 0;

            bool limitedView = viewRect.area() != 0;


            // allocate memory
            std::vector<fm::vec2> pts;
            std::vector<fm::vec2> tpt;
            std::vector<fm::vec4> clr;
            std::vector<fm::Uint32> inds;

            fm::vec2 cursor;

            std::vector<float> lineWidths;

            // loop through all characters
            Cx(linesCount) if (lines[x])
            {
                const fm::String &text = *lines[x];

                if (limitedView && cursor.y > viewRect.pos.y + viewRect.size.h)
                    break;

                if (!limitedView || cursor.y + metrics.lineGap > viewRect.pos.y)
                {
                    C(text.size())
                    {
                        if (notDrawn.find(text[i]) != fm::String::npos)
                            continue;

                        if (newLine.size() && text.find(newLine,i,1) == i)
                        {
                            cursor.x = 0;
                            cursor.y += metrics.lineGap;

                            if (limitedView && cursor.y > viewRect.pos.y + viewRect.size.h)
                                break;

                            i += newLine.size()-1;
                            continue;
                        }

                        // check for whitespace
                        if (text[i] == ' ' || text[i] == '\t')
                        {
                            cursor.x += getCharWidth(font,text[i],fm::nullPtr,spacing == 1,spacesInTab);
                            continue;
                        }


                        fg::Glyph g;

                        // find advance to the next character
                        float xadvance = getCharWidth(font,text[i],&g,spacing == 1,spacesInTab);

                        if (limitedView && cursor.x + xadvance < viewRect.pos.x)
                        {
                            cursor.x += xadvance;
                            continue;
                        }

                        if (limitedView && cursor.x > viewRect.pos.x + viewRect.size.w)
                            break;

                        // build a quad
                        Cxy(2,2)
                        {
                            fm::vec2 fixOffset = cursor - g.leftdown*fm::vec2(0,1) - viewRect.pos;
                            fm::vec2 locP = fm::vec2(x,y)*g.size;
                            locP.x += fm::Size((xadvance - g.size.w)/2.f);

                            fm::vec2 lineP = locP - fm::vec2(0,g.size.h) + fm::vec2(0,metrics.maxH);

                            fm::vec2 p = lineP + fixOffset;
                            fm::vec2 boundOffset;

                            if (limitedView)
                            {
                                if (p.x > viewRect.size.w) boundOffset.x = p.x - viewRect.size.w;
                                if (p.y > viewRect.size.h) boundOffset.y = p.y - viewRect.size.h;

                                if (p.x < 0) boundOffset.x = p.x;
                                if (p.y < 0) boundOffset.y = p.y;
                            }

                            pts.push_back(p - boundOffset),
                            tpt.push_back(fm::vec2(x,y)*g.size + g.pos - boundOffset),
                            clr.push_back(color);
                        }

                        // calculate indexes
                        Cx(2) Cy(3) inds.push_back(quadId*4+x+y);

                        // advance variables
                        quadId++;
                        cursor.x += xadvance;

                        // keep size
                        if (size.w < pts.back().x) size.w = pts.back().x;
                        if (size.h < pts.back().y) size.h = pts.back().y;
                    }
                }

                cursor.x = 0;
                cursor.y += metrics.lineGap;
            }

            if (retSize) *retSize = limitedView ? (fm::vec2)viewRect.size : size;

            if (pts.size() == 0) return;

            // build drawData
            ret[fg::Assoc::Position].set(&pts[0],pts.size());
            ret[fg::Assoc::TextureUV].set(&tpt[0],tpt.size());
            ret[fg::Assoc::Color].set(&clr[0],clr.size());
            ret.addDraw(fg::IndexArrayHolder(&inds[0],inds.size()),fg::Triangles);
        }
    }
}
