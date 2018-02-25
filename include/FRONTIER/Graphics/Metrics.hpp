////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_METRICS_HPP_INCLUDED
#define FRONTIER_METRICS_HPP_INCLUDED
#include <FRONTIER/System/util/API.h>
#define FRONTIER_METRICS

namespace fg
{
	//////////////////////////////////
	/// @brief This simple class holds some typical information about a font
	///
	/// @ingroup Graphics
	/// 
	//////////////////////////////////
	class FRONTIER_API Metrics
	{
	public:
		int maxH;    ///< The highest amount a glyph ascends above the baseline
		int minH;    ///< The highest amount a glyph descends below the baseline (usually negative)
		int lineGap; ///< The space between two rows (not always equal to maxH-minH)

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Metrics(int maxH=0,int minH=0,int lineGap=0);
	};
}

#endif // FRONTIER_METRICS_HPP_INCLUDED
