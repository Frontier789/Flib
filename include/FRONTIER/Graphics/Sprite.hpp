////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_SPRITE_HPP_INCLUDED
#define FRONTIER_SPRITE_HPP_INCLUDED
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Time.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/System/Ref.hpp>
#define FRONTIER_SPRITE

namespace fg
{
    class ShaderManager;
    class Texture;

	//////////////////////////////////
    class FRONTIER_API Sprite : public Drawable
    {
        const fg::Texture *m_tex;
        fm::rect2s m_texRect;
        fg::DrawData m_draw;
        fm::vec2 m_size;
        fm::vec2 m_pos;

        void init();
    public:

        /////////////////////////////////////////////////////////////
        explicit Sprite(fm::Ref<const fg::Texture> tex = fm::nullPtr,
                        const fm::rect2s &texRect = fm::rect2s(),
                        const fm::vec2 &pos = fm::vec2(),
                        const fm::vec2 &size = fm::vec2());

        /////////////////////////////////////////////////////////////
        Sprite &setPos(const fm::vec2 &pos);
        const fm::vec2 &getPos() const;

        /////////////////////////////////////////////////////////////
        Sprite &setSize(const fm::vec2 &size);
        const fm::vec2 &getSize() const;

        /////////////////////////////////////////////////////////////
        Sprite &setTexRect(const fm::rect2s &texRect);
        const fm::rect2s &getTexRect() const;

        /////////////////////////////////////////////////////////////
        Sprite &setTexture(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect = fm::rect2s());
        const fg::Texture *getTexture() const;

        /////////////////////////////////////////////////////////////
        void onDraw(ShaderManager &shader) const;

        /////////////////////////////////////////////////////////////
        void onUpdate(const fm::Time &dt = fm::Time::Zero);
    };
}

#endif // FRONTIER_SPRITE_HPP_INCLUDED
