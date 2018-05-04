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
#ifndef FRONTIER_NONCOPYABLE_HPP_INCLUDED
#define FRONTIER_NONCOPYABLE_HPP_INCLUDED
#include <FRONTIER/System/util/API.h>
#define FRONTIER_NONCOPYABLE
namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Virtual base class for types that cannot be copied
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
    class FRONTIER_API NonCopyable
    {
    public:
    
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
        inline NonCopyable() {}
        
		/////////////////////////////////////////////////////////////
		/// @brief Virtual destructor
		///
		/////////////////////////////////////////////////////////////
        inline virtual ~NonCopyable() {}
    private:
        
		/////////////////////////////////////////////////////////////
		/// @brief Private deleted copy constructor
		/// 
		/////////////////////////////////////////////////////////////
		NonCopyable(const NonCopyable &) = delete;
        
		/////////////////////////////////////////////////////////////
		/// @brief Private deleted overload of the assignment operator
		/// 
		/////////////////////////////////////////////////////////////
        NonCopyable &operator=(const NonCopyable &) = delete;
    };
}
#endif //FRONTIER_NONCOPYABLE_HPP_INCLUDED
