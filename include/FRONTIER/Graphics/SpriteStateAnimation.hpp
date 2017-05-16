////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_SPRITESTATEANIMATION_HPP_INCLUDED
#define FRONTIER_SPRITESTATEANIMATION_HPP_INCLUDED

#include <FRONTIER/Graphics/StateAnimation.hpp>
#include <FRONTIER/Graphics/Transformable.hpp>
#include <FRONTIER/Graphics/FramedSprite.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <map>

#define FRONTIER_SPRITESTATEANIMATION

namespace fm
{
	class String;
}

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief State based sprite animation
	/// 
	/////////////////////////////////////////////////////////////
	template<class StateType>
	class SpriteStateAnimation : public StateAnimation<StateType>, public Transformable
	{
		std::map<StateType,fg::FramedSprite> m_stateToSprite; ///< State to sprite map
		fg::FramedSprite *m_activeSprite; ///< The current sprite
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the state changes
		///
		/////////////////////////////////////////////////////////////
		virtual void onStateChange() override;
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param initState Initial state
		/// 
		/////////////////////////////////////////////////////////////
		SpriteStateAnimation(StateType initState = StateType());
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw using a given shader
		/// 
		/// @param shader Shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(ShaderManager &shader) override;

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen position of the animation
		/// 
		/// @param pos The position on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		virtual SpriteStateAnimation &setPosition(const fm::vec2 &pos) override;

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen size of the animation
		/// 
		/// @param size The size on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		virtual SpriteStateAnimation &setSize(const fm::vec2 &size) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Associate a state with a sprite
		/// 
		/// @param stateName The name of the state
		/// @param sprite The sprite
		/// 
		/////////////////////////////////////////////////////////////
		void setSprite(const fm::String &stateName,fg::FramedSprite sprite);
		
		/////////////////////////////////////////////////////////////
		/// @brief Remove a state
		/// 
		/// @param stateName The name of the state
		/// 
		/////////////////////////////////////////////////////////////
		void eraseSprite(const fm::String &stateName);
	};
	
	typedef SpriteStateAnimation<fm::String> SpriteAnimation;
}

#endif // FRONTIER_SPRITESTATEANIMATION_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/SpriteStateAnimation.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
