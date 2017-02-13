////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			 ///
/// condition that this disclaimer is not modified/removed.			///
/// You may not misclaim the origin of this software.				  ///
///																	///
/// If you use this software in your program/project a				 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							///
///																	///
/// You should have received a copy of GNU GPL with this software	  ///
///																	///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_SPRITE_HPP_INCLUDED
#define FRONTIER_SPRITE_HPP_INCLUDED

#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Time.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/System/Ref.hpp>

#define FRONTIER_SPRITE

namespace fg
{
	class ShaderManager;
	class Texture;
	class Glyph;

	/////////////////////////////////////////////////////////////
	/// @brief Class used to easily manage a lightweight sprite
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Sprite : public Drawable
	{
		const fg::Texture *m_tex; ///< The texture the sprite is on
		fm::rect2s m_texRect; ///< The rectangle of the sprite on the texture 
		fg::DrawData m_draw;  ///< The draw data of the sprite
		fm::vec2 m_size; ///< The size of the sprite on the screen
		fm::vec2 m_pos;  ///< The position of the sprite onscreen

		void init(); ///< Internal function
	public:
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Sprite();
		
		/////////////////////////////////////////////////////////////
		/// @brief construct the sprite from a glyph
		///
		/////////////////////////////////////////////////////////////
		Sprite(const Glyph &glyph);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the sprite from a whole texture
		/// 
		/// @param tex The texture
		/// 
		/////////////////////////////////////////////////////////////
		Sprite(fm::Ref<const fg::Texture> tex);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the sprite from a part of a texture
		/// 
		/// @param tex The texture
		/// @param texRect The rectangle on the texture
		///
		/////////////////////////////////////////////////////////////
		Sprite(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the sprite from a part of a texture with given position and size
		/// 
		/// @param tex The texture
		/// @param texRect The rectangle on the texture
		/// @param pos The position on the screen
		/// @param size The size on the screen
		///
		/////////////////////////////////////////////////////////////
		Sprite(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect,const fm::vec2 &pos,const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen position of the sprite
		/// 
		/// @param pos The position on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setPos(const fm::vec2 &pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen position of the sprite
		/// 
		/// @return The onscreen position
		/// 
		/////////////////////////////////////////////////////////////
		const fm::vec2 &getPos() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen size of the sprite
		/// 
		/// @param size The size on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setSize(const fm::vec2 &size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen size of the sprite
		/// 
		/// @return The onscreen size
		/// 
		/////////////////////////////////////////////////////////////
		const fm::vec2 &getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the covered rectange of the sprite
		/// 
		/// @param texRect The rectangle on the texture
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setTexRect(const fm::rect2s &texRect);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the rectangle that the sprite covers
		/// 
		/// @return The rectangle covered
		/// 
		/////////////////////////////////////////////////////////////
		const fm::rect2s &getTexRect() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the sprite to use a whole texture
		/// 
		/// @param tex The texture to use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setTexture(fm::Ref<const fg::Texture> tex);

		/////////////////////////////////////////////////////////////
		/// @brief Set the sprite to use a texture
		/// 
		/// @param tex The texture to use
		/// @param texRect The rectangle on the texture
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Sprite &setTexture(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the used texture
		/// 
		/// @return The texture
		/// 
		/////////////////////////////////////////////////////////////
		const fg::Texture *getTexture() const;

		/////////////////////////////////////////////////////////////
		/// @brief Draw the sprite
		/// 
		/// @param The shader to use
		/// 
		/////////////////////////////////////////////////////////////
		void onDraw(ShaderManager &shader);

		/////////////////////////////////////////////////////////////
		/// @brief Update the sprite
		/// 
		/// @param dt The time passed since last update
		/// 
		/////////////////////////////////////////////////////////////
		void onUpdate(const fm::Time &dt = fm::Time::Zero);
	};
}

#endif // FRONTIER_SPRITE_HPP_INCLUDED