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
#ifndef FRONTIER_ENABLE_IF_HPP_INCLUDED
#define FRONTIER_ENABLE_IF_HPP_INCLUDED
#define FRONTIER_ENABLE_IF

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member typedef of the second template argument iff the first template argument is true
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<bool B,class T = void>
	class Enable_if
	{
	public:
		typedef T type; ///< Alias to T if B is true
	};
	
	/// @cond DOXYGEN_HIDE
	template<class U>
	class Enable_if<false,U>
	{
		
	};
	/// @endcond
}

////////////////////////////////////////////////////////////
/// @class fm::Enable_if
/// 
/// Usage example:
/// @code
/// 
/// #include <FRONTIER/System/Type_traits.hpp>
/// #include <iostream>
/// 
/// template<class T>
/// fm::Enable_if<fm::is_arithmetic<T>,void>::type addFive(T &target)
/// {
/// 	target+=5;
/// }
/// 
/// class CustomType
/// {
/// public:
/// 	CustomType &operator+=(int i)
/// 	{
/// 		std::cout << "you tried to add "<<i<<" to a CustomType!"<<std::endl;
/// 		return *this;
/// 	}
/// };
/// 
/// int main()
/// {
/// 	int i=42;
/// 	float f=11.11;
/// 	unsigned long ul=1099511627776;
/// 	char Array[9]="abcdefgh\0";
/// 	char *cstr = &Array[1];
/// 	CustomType ct;
/// 	
/// 	std::cout<<"i was "<<i<<std::endl;
/// 	addFive(i);
/// 	std::cout<<"now i is "<<i<<std::endl;
/// 	
/// 	std::cout<<"f was "<<f<<std::endl;
/// 	addFive(f);
/// 	std::cout<<"now f is "<<f<<std::endl;
/// 	
/// 	std::cout<<"ul was "<<ul<<std::endl;
/// 	addFive(ul);
/// 	std::cout<<"now ul is "<<ul<<std::endl;
/// 	
/// 	addFive(cstr); // compilation error
/// 	addFive(ct);   // compilation error
/// 	
/// 	cstr += 5; // legit but char* is not arithmetic
/// 	ct   += 5; // legit but CustomType is not arithmetic
/// 	
/// }
/// @endcode
////////////////////////////////////////////////////////////

#endif // FRONTIER_ENABLE_IF_HPP_INCLUDED
