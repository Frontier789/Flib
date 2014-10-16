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
#ifndef FRONTIER_CLOCK_HPP_INCLUDED
#define FRONTIER_CLOCK_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Time.hpp>
#define FRONTIER_CLOCK
namespace fm
{
	/////////////////////////////////////////////////////////////
	///
	/// @brief A class that functions as a stop-watch
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Clock
	{
		Time m_startTime; ///< The time since we check the elapsed time
		Time m_pauseTime; ///< The time when we stopped
	public:
		typedef Time component_type;
		typedef Clock &reference;
		typedef const Clock &const_reference;
		enum {
			components = 2u ///< Public value indicating the amount of component_type's in the class
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Initializes Clock with zero elapsed time and not paused
		///
		/////////////////////////////////////////////////////////////
		Clock();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct clock paused or unpaused
		///
		/// Initializes Clock with zero elapsed time and paused if @a paused is true
		/// 
		/// @param paused If true then the Clock is paused by default
		/// 
		/////////////////////////////////////////////////////////////
		explicit Clock(bool paused);
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// Initializes Clock from an other Clock
		/// 
		/// @param copy Instance to copy
		/// 
		/////////////////////////////////////////////////////////////
		Clock(const_reference copy);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from elapsed time, paused or unpaused
		///
		/// Initializes Clock with @a startTime elapsed time and if @a paused is true paused
		/// 
		/// @param startTime Elapsed time to initialize with
		/// @param paused If true then the Clock is paused by default
		/// 
		/////////////////////////////////////////////////////////////
		Clock(const Time &startTime,bool paused=false);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get time since started
		///
		/// If the Clock paused then this function will return the difference between
		/// the startTime and pauseTime
		/// 
		/// @return Elapsed time
		/// 
		/////////////////////////////////////////////////////////////
		Time getTime() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Pause the clock
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference pause();
		
		/////////////////////////////////////////////////////////////
		/// @brief Resumes the clock
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference unPause();
		
		/////////////////////////////////////////////////////////////
		/// @brief Resumes the clock
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference resume();
		
		/////////////////////////////////////////////////////////////
		/// @brief Restart the clock
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference restart();
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the elapsed time
		/// 
		/// @param elapsed The elapsed time to use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setTime(const Time &elapsed);
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if the Clock is paused
		/// 
		/// @return True if paused
		/// 
		/////////////////////////////////////////////////////////////
		bool isPaused() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief static const Clock that is started when the object is created
		/// 
		/////////////////////////////////////////////////////////////
		static const Clock now;
	};
}
#endif //FRONTIER_CLOCK_HPP_INCLUDED

////////////////////////////////////////////////////////////
/// @class fm::Clock
/// @ingroup System
///
/// Usage example:
/// @code
///
/// fm::Clock clk;
/// unsigned long k=1;
/// for (int i=0;i<50000000;i++) //do some work
/// 	k*=(i%5)*(i%5);
/// std::cout<<clk.getTime().asMilliseconds()<<"ms elapsed"<<std::endl;
/// 
/// @endcode
////////////////////////////////////////////////////////////
