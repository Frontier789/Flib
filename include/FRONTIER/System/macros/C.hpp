//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_C_HPP_INCLUDED
#define FRONTIER_C_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#define FRONTIER_C

	#define C(nnn)  for (fm::Size i=0,FRONTIER_C_N=(nnn);i<FRONTIER_C_N;++i)
	#define Cx(nnn) for (fm::Size x=0,FRONTIER_CX_N=(nnn);x<FRONTIER_CX_N;++x)
	#define Cy(nnn) for (fm::Size y=0,FRONTIER_CY_N=(nnn);y<FRONTIER_CY_N;++y)
	#define Cz(nnn) for (fm::Size z=0,FRONTIER_CZ_N=(nnn);z<FRONTIER_CZ_N;++z)
	#define Cf(nnn,ii) for (fm::Size ii=0,FRONTIER_CF_N=(nnn);ii<FRONTIER_CF_N;++ii)
	#define Cxy(Xnnn,Ynnn) Cx(Xnnn) Cy(Ynnn)

#endif // FRONTIER_C_HPP_INCLUDED