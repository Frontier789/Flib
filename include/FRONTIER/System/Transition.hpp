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
#ifndef FRONTIER_TRANSITION_HPP_INCLUDED
#define FRONTIER_TRANSITION_HPP_INCLUDED
#define FRONTIER_TRANSITION

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to define a transition from a value to an other
	///
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class Transition
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Encode a type of transition
		///
		/////////////////////////////////////////////////////////////
		enum Mode {
			Linear,       ///< Linear transition { x -> x }
			Smoothstep,   ///< Cubic Hermite interpolation { x -> 3x^2 - 2x^3 }
			Smootherstep, ///< Smootherstep interpolation { x -> 6x^5 - 15x^4 + 10x^3 }
			Trigonometric ///< Trigonometric interpolation { x -> (1 - cos(x*PI))/2 }
		};
		
	protected:
		fm::Time  m_duration; ///< The duration of the transition 
		fm::Clock m_clock;    ///< The internal clock
		Mode m_mode; ///< The mode of interpolation
		T m_start;   ///< The initial value
		T m_end;     ///< The value at the end of transition
		
		virtual double getRatio(double val) const; ///< internal funtion used to transform from linear to given interpolation
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Transition();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~Transition();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a transition from a start and end value
		/// 
		/// @param begin The start value
		/// @param end The end value
		/// @param duration The duration
		/// @param mode The type of transition to use
		/// 
		/////////////////////////////////////////////////////////////
		Transition(T begin,T end,fm::Time duration = fm::seconds(1),Mode mode = Smoothstep);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current value
		/// 
		/// @return The value based on elapsed time
		/// 
		/////////////////////////////////////////////////////////////
		T get() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get how much time is left of the transition
		/// 
		/// @return The time left
		/// 
		/////////////////////////////////////////////////////////////
		fm::Time getTimeLeft() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check whether the transition is finished
		/// 
		/// @return True iff finished
		/// 
		/////////////////////////////////////////////////////////////
		bool finished() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the transition's values
		/// 
		/// @param begin The starting value
		/// @param end The ending value
		/// @param duration The duration
		/// @param mode The type of transition to use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Transition<T> &set(T begin,T end,fm::Time duration = fm::seconds(1),Mode mode = Smoothstep);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the transition's end value
		/// 
		/// @param newEnd The new ending value
		/// @param addDuration The amount to increase the duration with
		/// @param keepTimeLeft Indicates whether the time left is to be thrown away or be kept
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Transition<T> &retarget(T newEnd,fm::Time addDuration = fm::seconds(1),bool keepTimeLeft = true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Restart the transition
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Transition<T> &restart();
	};
}

#endif // FRONTIER_TRANSITION_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Transition.inl>
#endif
