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
#ifndef FRONTIER_SPRITESTATEANIMATION_INL_INCLUDED
#define FRONTIER_SPRITESTATEANIMATION_INL_INCLUDED

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void SpriteStateAnimation<StateType>::onStateChange()
	{
		m_activeSprite = &m_stateToSprite[StateAnimation<StateType>::getState()];
		
		if (m_activeSprite)
		{
			m_activeSprite->setPosition(getPosition());
			m_activeSprite->setSize(getSize());
		}
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline SpriteStateAnimation<StateType>::SpriteStateAnimation(StateType initState) : StateAnimation<StateType>(initState),
																						m_activeSprite(nullptr)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void SpriteStateAnimation<StateType>::onDraw(ShaderManager &shader)
	{
		if (m_activeSprite)
			m_activeSprite->onDraw(shader);
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline SpriteStateAnimation<StateType> &SpriteStateAnimation<StateType>::setPosition(const fm::vec2 &pos)
	{
		Transformable::setPosition(pos);
		
		if (m_activeSprite) m_activeSprite->setPosition(getPosition());
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline SpriteStateAnimation<StateType> &SpriteStateAnimation<StateType>::setSize(const fm::vec2 &size)
	{
		if (m_activeSprite)
		{
			m_activeSprite->setSize(size);
			
			Transformable::setSize(m_activeSprite->getSize());
		}
		else
		{
			Transformable::setSize(size);		
		}
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void SpriteStateAnimation<StateType>::setSprite(const fm::String &stateName,fg::FramedSprite sprite)
	{
		m_stateToSprite[stateName] = sprite;
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void SpriteStateAnimation<StateType>::eraseSprite(const fm::String &stateName)
	{
		m_stateToSprite.erase(stateName);
	}
}

#endif // FRONTIER_SPRITESTATEANIMATION_INL_INCLUDED
