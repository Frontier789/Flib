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
#ifndef FRONTIER_STATEANIMATION_INL_INCLUDED
#define FRONTIER_STATEANIMATION_INL_INCLUDED

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void StateAnimation<StateType>::onStateChange()
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline StateAnimation<StateType>::StateAnimation(StateType initial) : m_curState(initial)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void StateAnimation<StateType>::setState(StateType state)
	{
		m_curState = state;
		onStateChange();
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline StateType StateAnimation<StateType>::getState() const
	{
		return m_curState;
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void StateAnimation<StateType>::onDraw(ShaderManager &)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class StateType>
	inline void StateAnimation<StateType>::onUpdate(const fm::Time &)
	{
		
	}
}

#endif // FRONTIER_STATEANIMATION_INL_INCLUDED
