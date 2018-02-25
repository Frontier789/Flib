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
#ifndef FRONTIER_HEAVYTOCOPY_HPP_INCLUDED
#define FRONTIER_HEAVYTOCOPY_HPP_INCLUDED

#ifndef FRONTIER_HEAVYCOPY_ALLOW
	#define FRONTIER_HEAVYCOPY_FORBID
#endif

#ifdef FRONTIER_HEAVYCOPY_FORBID
	#define FRONTIER_HEAVYCOPY_QUALIFIER = delete
#else
	#define FRONTIER_HEAVYCOPY_QUALIFIER
#endif

#ifndef FRONTIER_HEAVYCOPY_NOTE
	#define FRONTIER_HEAVYCOPY_NOTE
#endif

#endif // FRONTIER_HEAVYTOCOPY_HPP_INCLUDED

