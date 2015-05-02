//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_NULLPTR_INL_INCLUDED
#define FRONTIER_NULLPTR_INL_INCLUDED

namespace fm
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		template<class T>
		inline NullPtr::operator T*() const
		{
			return (T*)0;
		}
		
		/////////////////////////////////////////////////////////////
		template<class T>
		inline bool NullPtr::operator==(const T *ptr) const
		{
			return ptr == (T*)0;
		}
	}
	
	extern const priv::NullPtr nullPtr;
}

#endif // FRONTIER_NULLPTR_INL_INCLUDED