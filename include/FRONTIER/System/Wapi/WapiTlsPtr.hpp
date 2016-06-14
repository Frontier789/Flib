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
#ifndef FRONTIER_WAPITLSPTR_HPP_INCLUDED
#define FRONTIER_WAPITLSPTR_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_WAPITLSPTR

namespace fm
{
	namespace Wapi
	{
		/////////////////////////////////////////////////////////////
		/// @brief Windows specific implementation of thread-local-storage
		/// 
		/////////////////////////////////////////////////////////////
		class FRONTIER_API TlsPtr : public fm::NonCopyable
		{
			void *m_id; ///< The id of the container
		public:
			typedef TlsPtr &reference;
			typedef const TlsPtr &const_reference;
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/////////////////////////////////////////////////////////////
			TlsPtr();
			
			/////////////////////////////////////////////////////////////
			/// @brief Default destructor
			/// 
			/////////////////////////////////////////////////////////////
			~TlsPtr();
			
			/////////////////////////////////////////////////////////////
			/// @brief Set the stored data
			/// 
			/// @param ptr The new value
			/// 
			/// @return True iff everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setPtr(void *ptr);
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the stored data
			/// 
			/// @return The currently stored value (NULL by default)
			/// 
			/////////////////////////////////////////////////////////////
			void *getPtr() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Find out if a internal error happened
			/// 
			/// @return True iff no error happend
			/// 
			/////////////////////////////////////////////////////////////
			bool isValid() const;
		};
	}
}


#endif // FRONTIER_WAPITLSPTR_HPP_INCLUDED