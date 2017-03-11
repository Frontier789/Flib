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
#ifndef FRONTIER_TRANSITION_INL_INCLUDED
#define FRONTIER_TRANSITION_INL_INCLUDED

namespace fm
{
	template<class T>
	inline double Transition<T>::getRatio(double val) const
	{
		if (m_mode == Linear)        return val;
		if (m_mode == Smoothstep)    return (3 - 2*val)*val*val;
		if (m_mode == Smootherstep)  return (6*val*val - 15*val + 10)*val*val*val;
		if (m_mode == Trigonometric) return (1 - std::cos(fm::deg(val*180)))/2;
		
		return val;
	}	

	template<class T>
	inline Transition<T>::Transition()
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline Transition<T>::~Transition()
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline Transition<T>::Transition(T begin,T end,fm::Time duration,Mode mode) : m_duration(duration), m_mode(mode), m_start(begin), m_end(end)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T Transition<T>::get() const
	{
		double r = m_clock.getTime() / m_duration;
		
		if (r > 1.0) r = 1.0;
		if (r < 0.0) r = 0.0;
		
		r = getRatio(r);
		
		return (1.0 - r)*m_start + r*m_end;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Time Transition<T>::getTimeLeft() const
	{
		fm::Time t = m_clock.getTime();
		
		if (t > m_duration) return fm::seconds(0);
		
		return m_duration - t;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline bool Transition<T>::finished() const
	{
		return m_clock.getTime() >= m_duration;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline Transition<T> &Transition<T>::set(T begin,T end,fm::Time duration,Mode mode)
	{
		m_duration = duration;
		m_start    = begin;
		m_mode     = mode;
		m_end      = end;
		
		m_clock.restart();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline Transition<T> &Transition<T>::retarget(T newEnd,fm::Time addDuration,bool keepTimeLeft)
	{
		m_duration = keepTimeLeft ? (getTimeLeft() + addDuration) : addDuration;
		m_start    = get();
		m_end      = newEnd;
		
		m_clock.restart();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline Transition<T> &Transition<T>::restart()
	{
		m_clock.restart();
		
		return *this;
	}
}
#endif // FRONTIER_TRANSITION_INL_INCLUDED
