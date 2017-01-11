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
#include <cstdint>
#define FRONTIER_C

	#define C(nnn)  for (size_t i=0,FRONTIER_C_N=(nnn);i<FRONTIER_C_N;++i)
	#define Cx(nnn) for (size_t x=0,FRONTIER_CX_N=(nnn);x<FRONTIER_CX_N;++x)
	#define Cy(nnn) for (size_t y=0,FRONTIER_CY_N=(nnn);y<FRONTIER_CY_N;++y)
	#define Cz(nnn) for (size_t z=0,FRONTIER_CZ_N=(nnn);z<FRONTIER_CZ_N;++z)
	#define Cf(nnn,ii) for (size_t ii=0,FRONTIER_CF_N_ ## ii=(nnn);ii<FRONTIER_CF_N_ ## ii;++ii)
	#define Cv(Vnnn) for (fm::vec2s p,FRONTIER_CV_WH=(Vnnn);p.x<FRONTIER_CV_WH.w;++p.x) for (p.y=0;p.y<FRONTIER_CV_WH.h;++p.y)
	#define Cxy(Xnnn,Ynnn) Cx(Xnnn) Cy(Ynnn)

#endif // FRONTIER_C_HPP_INCLUDED
