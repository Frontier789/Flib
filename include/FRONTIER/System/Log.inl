////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_LOG_INL_INCLUDED
#define FRONTIER_LOG_INL_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/macros/dont_include_inl_end>

namespace fm
{
		/////////////////////////////////////////////////////////////
		template<class T>
		Log::reference Log::operator<<(const fm::vector2<T> &vec)
		{
			return (*this)<<vec.x<<' '<<vec.y;
		}
		
		/////////////////////////////////////////////////////////////
		template<class T>
		Log::reference Log::operator<<(const fm::vector3<T> &vec)
		{
			return (*this)<<vec.x<<' '<<vec.y<<' '<<vec.z;
		}
		
		/////////////////////////////////////////////////////////////
		template<class T>
		Log::reference Log::operator<<(const fm::vector4<T> &vec)
		{
			return (*this)<<vec.x<<' '<<vec.y<<' '<<vec.z<<' '<<vec.w;
		}
		
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size W,fm::Size H>
		Log::reference Log::operator<<(const fm::matrix<W,H,T> &mat)
		{
			for (fm::Size x=0;x<W;x++)
				for (fm::Size y=0;y<H;y++)
		            if (y+1==H && x+1<W)
		                (*this)<<mat[x][y]<<'\n';
					else
		                (*this)<<mat[x][y]<<' ';
			
			return *this;
		}
}
	
#endif // FRONTIER_LOG_INL_INCLUDED