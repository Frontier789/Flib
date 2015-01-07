////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_SPRITE_HPP_INCLUDED
#define FRONTIER_SPRITE_HPP_INCLUDED
#include <FRONTIER/System/Transformable.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Vertex.hpp>
#include <FRONTIER/System/Rect.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_SPRITE
namespace fg
{
	class Texture;

    /////////////////////////////////////////////////////////////
	/// @brief Class used to store and draw a rect of a texture
	///
    /////////////////////////////////////////////////////////////
	class FRONTIER_API Sprite : public Drawable, public fm::Transformable
    {
		const Texture *m_texture;   ///< A pointer to the texture
		fm::vertex2f m_vertices[4]; ///< The vertices used when drawing
		fm::rect2s m_textureRect;   ///< The drawn rect in the texture
		fm::vec4   m_color;			///< The associated color
    public:
        typedef Sprite &reference;
        typedef const Sprite &const_reference;

        /////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Sprite();

        /////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/////////////////////////////////////////////////////////////
		Sprite(const_reference copy);

        /////////////////////////////////////////////////////////////
		/// @brief Construct a sprite that takes up the whole texture
		///
		/// @param texture The texture
		///
		/////////////////////////////////////////////////////////////
		explicit Sprite(const Texture &texture);

        /////////////////////////////////////////////////////////////
		/// @brief Construct a sprite that from a texture and texture rect
		///
		/// @a textureRect is understood to hold values in pixels
		///
		/// @param texture The texture
		/// @param textureRect Describes the portion of the texture which will be drawn
		/// @param accountTexPosInPos If true setPosition is called with textureRect.pos
		///
		/////////////////////////////////////////////////////////////
		Sprite(const Texture &texture,const fm::rect2s &textureRect,bool accountTexPosInPos = false);

        /////////////////////////////////////////////////////////////
		/// @brief Change the color of the vertices
		///
		/// @param color The new color
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setColor(const fm::vec4 &color);

        /////////////////////////////////////////////////////////////
		/// @brief Change the texture
		///
		/// @param texture The new texture
		/// @param resetTextureRect If true the texture rect is set to the whole texture
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setTexture(const Texture &texture,bool resetTextureRect = false);

        /////////////////////////////////////////////////////////////
		/// @brief Change the texture
		///
		/// @param texture A pointer to the new texture
		/// @param resetTextureRect If true the texture rect is set to the whole texture
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setTexture(const Texture *texture,bool resetTextureRect = false);

        /////////////////////////////////////////////////////////////
		/// @brief Change the texture rect
		///
		/// @a textureRect is understood to hold values in pixels
		///
		/// @param textureRect Describes the portion of the texture which will be drawn
		/// @param accountTexPosInPos If true setPosition is called with textureRect.pos
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setTextureRect(fm::rect2s textureRect,bool accountTexPosInPos = false);

        /////////////////////////////////////////////////////////////
		/// @brief Get the texture associated with the sprite
		///
		/// @return A pointer to the texture
		///
		/////////////////////////////////////////////////////////////
		const Texture *getTexture() const;

        /////////////////////////////////////////////////////////////
		/// @brief Get the color associated with the sprite
		///
		/// @return The associated color
		///
		/////////////////////////////////////////////////////////////
		const fm::vec4 &getColor() const;

        /////////////////////////////////////////////////////////////
		/// @brief Get the texture rect
		///
		/// The returned rect contains values in pixels
		///
		/// @return The texture rect
		///
		/////////////////////////////////////////////////////////////
		const fm::rect2s &getTextureRect() const;

        /////////////////////////////////////////////////////////////
		/// @brief Draw the sprite
		///
		/// @param states The options used when rendering
		///
		/////////////////////////////////////////////////////////////
		void draw(const fg::RenderStates &states) const;

        /////////////////////////////////////////////////////////////
		/// @brief Access the vertices of the sprite
		///
		/// @param index The index of the vertex [0-3]
		///
		/// @return The vertex
		///
		/////////////////////////////////////////////////////////////
        fm::vertex2f &operator[](unsigned int index);

        /////////////////////////////////////////////////////////////
		/// @brief Access the vertices of the sprite
		///
		/// @param index The index of the vertex [0-3]
		///
		/// @return The vertex
		///
		/////////////////////////////////////////////////////////////
        const fm::vertex2f &operator[](unsigned int index) const;
	};
}

#endif //FRONTIER_SPRITE_HPP_INCLUDED
