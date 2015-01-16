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
#ifndef FRONTIER_IS_INTEGRAL_HPP_INCLUDED
#define FRONTIER_IS_INTEGRAL_HPP_INCLUDED
#define FRONTIER_IS_INTEGRAL

/// @cond DOXYGEN_HIDE
#define IS_INTEGRAL_CREATE_TRUE_CASE(TYPE)  \
template<>                     			    \
class Is_integral<TYPE>        			    \
{                              				\
public:                        				\
	enum{                      				\
		value = true             				\
	};                         				\
}
/// @endcond
namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a integer type
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class Is_integral
	{
	public:
		enum {
			value /** @cond DOXYGEN_HIDE */ = false /** @endcond */ ///< True if T is a integer type
		};
	};

	/// @cond DOXYGEN_HIDE
	IS_INTEGRAL_CREATE_TRUE_CASE(bool			   );
	                                                
	IS_INTEGRAL_CREATE_TRUE_CASE(char			   );
	IS_INTEGRAL_CREATE_TRUE_CASE(unsigned char     );
	IS_INTEGRAL_CREATE_TRUE_CASE(signed char       );
                                                    
	IS_INTEGRAL_CREATE_TRUE_CASE(wchar_t           );
	                                                
	IS_INTEGRAL_CREATE_TRUE_CASE(unsigned short int);
	IS_INTEGRAL_CREATE_TRUE_CASE(signed short int  );
	                                                
	IS_INTEGRAL_CREATE_TRUE_CASE(unsigned int      );
	IS_INTEGRAL_CREATE_TRUE_CASE(signed int        );
                                                    
	IS_INTEGRAL_CREATE_TRUE_CASE(signed long int   );
	IS_INTEGRAL_CREATE_TRUE_CASE(unsigned long int );
	/// @endcond
	
}
/// @cond DOXYGEN_HIDE
#undef IS_ARITHMETIC_CREATE_TRUE_CASE
/// @endcond

#endif // FRONTIER_IS_INTEGRAL_HPP_INCLUDED
