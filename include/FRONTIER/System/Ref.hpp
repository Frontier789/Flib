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
#ifndef FRONTIER_REF_HPP_INCLUDED
#define FRONTIER_REF_HPP_INCLUDED
#define FRONTIER_REF

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to be able to handle input reference as pointer thus uniforming with pointer input
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
    template<class T>
    class Ref
    {
		T *m_ptr; ///< The underlying pointer

    public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor from pointer
		/// 
		/// @param objPtr The pointer
		/// 
		/////////////////////////////////////////////////////////////
        Ref(T *objPtr = nullptr);

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor from reference
		/// 
		/// @param objRef The reference
		/// 
		/////////////////////////////////////////////////////////////
        Ref(T &objRef);

		/////////////////////////////////////////////////////////////
		/// @brief Memeber access operator
		/// 
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		T *operator->();

		/////////////////////////////////////////////////////////////
		/// @brief Memeber access operator
		/// 
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		const T *operator->() const;

		/////////////////////////////////////////////////////////////
		/// @brief Dereference operator
		/// 
		/// @return The reference
		/// 
		/////////////////////////////////////////////////////////////
		T &operator*();

		/////////////////////////////////////////////////////////////
		/// @brief Dereference operator
		/// 
		/// @return The reference
		/// 
		/////////////////////////////////////////////////////////////
		const T &operator*() const;

		/////////////////////////////////////////////////////////////
		/// @brief Assignment operator
		/// 
		/// @param pointer The pointer to assign
		///
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		Ref &operator=(T *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Negation operator
		///
		/// @return True iff the wrapped pointer is nullptr
		/// 
		/////////////////////////////////////////////////////////////
		bool operator!() const;

		/////////////////////////////////////////////////////////////
		/// @brief Conversion operator for underlying pointer
		///
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		operator T*();

		/////////////////////////////////////////////////////////////
		/// @brief Conversion operator for underlying pointer
		///
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		operator const T*() const;

		/////////////////////////////////////////////////////////////
		/// @brief Conversion operator for bool
		///
		/// @return True iff the wrapped is not nullptr
		/// 
		/////////////////////////////////////////////////////////////
		operator bool() const;
    };
}

#endif // FRONTIER_REF_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Ref.inl>
#endif
