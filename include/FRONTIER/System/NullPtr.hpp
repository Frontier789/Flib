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
#ifndef FRONTIER_NULLPTR_HPP_INCLUDED
#define FRONTIER_NULLPTR_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_NULLPTR

namespace fm
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		/// @brief a type that can be implicitly cast to any pointer (like NULL) but cannot be used as a number
		///
		/////////////////////////////////////////////////////////////
		class NullPtr
		{
		public:

			/////////////////////////////////////////////////////////////
			/// @brief The result of the conversion is always 0 with the given type
			///
			/////////////////////////////////////////////////////////////
			template<class T>
			operator T*() const;

			/////////////////////////////////////////////////////////////
			/// @brief Compare to pointer
			///
			/// @param ptr The pointer
			///
			/// @return True iff ptr is null
			///
			/////////////////////////////////////////////////////////////
			template<class T>
			bool operator==(const T *ptr) const;
		};
	}

	typedef priv::NullPtr NullPtr;

	FRONTIER_API extern const priv::NullPtr nullPtr;
}

#endif // FRONTIER_NULLPTR_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/NullPtr.inl>
#endif
