////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_TLSPTR_HPP_INCLUDED
#define FRONTIER_TLSPTR_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#define FRONTIER_TLSPTR

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to store pointers thread-locally
	/// 
	/// @ingroup System
	/// 
	/// Upon error this class prompts to fm::fm_log
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class TlsPtr : public fm::NonCopyable
	{
		void *m_impl; ///< A pointer to the platform specific implementation
	public:
		typedef TlsPtr &reference;
		typedef const TlsPtr &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// Initializes the pointer to NULL for every thread
		/// 
		/////////////////////////////////////////////////////////////
		TlsPtr();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// The user is responsible for freeing 
		/// the data associated with the assigned pointer
		/// 
		/////////////////////////////////////////////////////////////
		~TlsPtr();
		
		/////////////////////////////////////////////////////////////
		/// @brief Overload of operator ->
		/// 
		/// One can acces the assigned pointer's target's fields using -> from a TlsPtr
		/// 
		/// @return The assigned pointer
		/// 
		/////////////////////////////////////////////////////////////
		T *operator->();
		
		/////////////////////////////////////////////////////////////
		/// @brief Overload of operator ->
		/// 
		/// One can acces the assigned pointer's target's fields using -> from a TlsPtr
		/// 
		/// @return The assigned pointer
		/// 
		/////////////////////////////////////////////////////////////
		const T *operator->() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Overload of operator *
		/// 
		/// One can dereference the assigned pointer using *
		/// Warning: does not prevent NULL dereferencing!
		/// 
		/// @return The assigned pointer's target
		/// 
		/////////////////////////////////////////////////////////////
		T &operator*();
		
		/////////////////////////////////////////////////////////////
		/// @brief Overload of operator *
		/// 
		/// One can dereference the assigned pointer using *
		/// Warning: does not prevent NULL dereferencing!
		/// 
		/// @return The assigned pointer's target
		/// 
		/////////////////////////////////////////////////////////////
		const T &operator*() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Assign a pointer
		/// 
		/// The user is responsible for freeing 
		/// the data associated with the assigned pointer
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator=(T *pointer);
		
		/////////////////////////////////////////////////////////////
		/// @brief Assign a pointer
		/// 
		/// The user is responsible for freeing 
		/// the data associated with the assigned pointer
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference set(T *pointer);
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if a internal error happend
		/// 
		/// In case an internal error other function wont do anything
		/// 
		/// @return True iff everything went right
		/// 
		/////////////////////////////////////////////////////////////
		bool isValid() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to pointer
		/// 
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		T *get();
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to pointer
		/// 
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		const T *get() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to pointer
		/// 
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		operator T*();
		
		/////////////////////////////////////////////////////////////
		/// @brief Convert to pointer
		/// 
		/// @return The pointer
		/// 
		/////////////////////////////////////////////////////////////
		operator const T*() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if not NULL
		/// 
		/// @return True iff the associated pointer is not NULL
		/// 
		/////////////////////////////////////////////////////////////
		operator bool() const;
	};	
}

#endif // FRONTIER_TLSPTR_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/TlsPtr.inl>
#endif
