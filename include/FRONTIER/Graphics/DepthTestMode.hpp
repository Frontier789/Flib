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
#ifndef FRONTIER_DEPTHTESTMODE_HPP_INCLUDED
#define FRONTIER_DEPTHTESTMODE_HPP_INCLUDED
#define FRONTIER_DEPTHTESTMODE

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Encodes types of depth testing
	///
	/// By default depth test is not used
	///
	/////////////////////////////////////////////////////////////
	enum DepthTestMode {
		Less,    ///< Writes the fragment iff it's distance is smaller than the current in the depth buffer
		LEqual,  ///< Writes the fragment iff it's distance is smaller or equal than the current in the depth buffer
		GEqual,  ///< Writes the fragment iff it's distance is greater or equal than the current in the depth buffer
		Greater, ///< Writes the fragment iff it's distance is greater than the current in the depth buffer
		Always,  ///< Always writes the fragment in the depth buffer
		Unused   ///< Does not perform depth test
	};
}
#endif // FRONTIER_DEPTHTESTMODE_INCLUDED
