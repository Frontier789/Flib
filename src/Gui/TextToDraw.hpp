#include <FRONTIER/System/macros/Size.hpp>
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
