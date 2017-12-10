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
#ifndef FRONTIER_TIMEQUERY_HPP_INCLUDED
#define FRONTIER_TIMEQUERY_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Time.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#define FRONTIER_TIMEQUERY

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Keep  
	/// 
	/////////////////////////////////////////////////////////////
	class TimeQuery : public GlObject
	{
		mutable fm::Time m_runTime;
		bool m_running;
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Create a new query
		/// 
		/////////////////////////////////////////////////////////////
		TimeQuery();

		/////////////////////////////////////////////////////////////
		/// @brief Begin a new time query 
		/// 
		/// Starting an already running query is a no-op
		/// 
		/// @return The result of the opration
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result start();
		
		/////////////////////////////////////////////////////////////
		/// @brief Stop the currently running query 
		/// 
		/// Stopping a not running query is a no-op
		/// 
		/// @return The result of the opration
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result stop();

		/////////////////////////////////////////////////////////////
		/// @brief Check if the query is finished
		/// 
		/// @return True iff the query is finished
		/// 
		/////////////////////////////////////////////////////////////
		bool finished() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the elapsed time 
		/// 
		/// This is a blocking call meaning
		/// The call wont return until the query
		/// is finished
		/// 
		/// @return The time the query took to finish
		/// 
		/////////////////////////////////////////////////////////////
		fm::Time getTime() const;
	};

}
#endif // FRONTIER_TIMEQUERY_INCLUDED
