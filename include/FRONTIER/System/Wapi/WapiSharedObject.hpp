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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_WAPISHAREDOBJECT_HPP_INCLUDED
#define FRONTIER_WAPISHAREDOBJECT_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_WAPISHAREDOBJECT
#include <windows.h>

namespace fm
{
	namespace Wapi
	{
		/////////////////////////////////////////////////////////////
		/// @brief Windows OS specific class that is capable of loading functions from dlls
		/// 
		/// @ingroup System
		/// 
		/// Upon internal error this class prompts to fm::fm_log
		/// 
		/////////////////////////////////////////////////////////////
		class FRONTIER_API SharedObject
		{
			HINSTANCE m_handle; ///< The handle of the dll
		public:
			typedef SharedObject &reference;
			typedef const SharedObject &const_reference;
			typedef HINSTANCE Handle;
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/////////////////////////////////////////////////////////////
			SharedObject();
			
			/////////////////////////////////////////////////////////////
			/// @brief Constructor that loads a dll
			/// 
			/// @param objName The name of the dll
			/// 
			/////////////////////////////////////////////////////////////
			SharedObject(const char *objName);
			
			/////////////////////////////////////////////////////////////
			/// @brief Default destructor
			/// 
			/////////////////////////////////////////////////////////////
			~SharedObject();
			
			/////////////////////////////////////////////////////////////
			/// @brief Load a dll
			/// 
			/// This function frees up the previously loaded dll thus making
			/// every associated address invalid
			/// 
			/// @param objName The name of the dll
			/// 
			/// @return True iff everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool load(const char *objName);
			
			/////////////////////////////////////////////////////////////
			/// @brief Find out if the class has a opened dll
			/// 
			/// @return True iff a dll is loaded
			/// 
			/////////////////////////////////////////////////////////////
			bool isValid() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Find out if the class has a opened dll
			/// 
			/// @return True iff a dll is loaded
			/// 
			/////////////////////////////////////////////////////////////
			operator bool() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Get a function's address from the loaded dll
			/// 
			/// One should cast the returned value to the correct type!
			/// 
			/// @return The address of the function (NULL iff the function couldn't be found or no dll is loaded)
			/// 
			/////////////////////////////////////////////////////////////
			void (*getProcAddress(const char *funcName) const)();
			
			/////////////////////////////////////////////////////////////
			/// @brief Get a function's address from the loaded dll
			/// 
			/// One should supply the correct type of the function as
			/// a template parameter
			/// 
			/// @return The address of the function (NULL iff the function couldn't be found or no dll is loaded)
			/// 
			/////////////////////////////////////////////////////////////
			template<class F>
			F getProcAddress(const char *funcName) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the handle of the loaded dll
			/// 
			/// @return The handle (NULL if no dll is loaded)
			/// 
			/////////////////////////////////////////////////////////////
			HINSTANCE getHandle() const;
		};
	}
}

#endif // FRONTIER_WAPISHAREDOBJECT_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Wapi/WapiSharedObject.inl>
#endif
