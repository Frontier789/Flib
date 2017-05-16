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
#ifndef FRONTIER_STATEANIMATION_HPP_INCLUDED
#define FRONTIER_STATEANIMATION_HPP_INCLUDED

#include <FRONTIER/Graphics/Drawable.hpp>

#define FRONTIER_STATEANIMATION

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief State based animation class
	///
	/////////////////////////////////////////////////////////////
	template<class StateType>
	class StateAnimation : public Drawable
	{
		StateType m_curState; ///< The current state

	protected:
		/////////////////////////////////////////////////////////////
		/// @brief Called when the state is changed
		///
		/////////////////////////////////////////////////////////////
		virtual void onStateChange();
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Deafult constructor
		/// 
		/// @param initial The initial state
		/// 
		/////////////////////////////////////////////////////////////
		StateAnimation(StateType initial = StateType());
		
		/////////////////////////////////////////////////////////////
		/// @brief Change the current state
		/// 
		/// @param state The new state
		/// 
		/////////////////////////////////////////////////////////////
		void setState(StateType state);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current state
		/// 
		/// @return The current state
		/// 
		/////////////////////////////////////////////////////////////
		StateType getState() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Draw using a given shader
		/// 
		/// @param shader Shader to use
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(ShaderManager &shader) override;

		/////////////////////////////////////////////////////////////
		/// @brief Regular update function
		/// 
		/// @param dt The elapsed time since last calling update
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onUpdate(const fm::Time &dt) override;	
	};
}

#endif // FRONTIER_STATEANIMATION_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/StateAnimation.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
