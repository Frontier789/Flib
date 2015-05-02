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
#ifndef FRONTIER_Is_function_HPP_INCLUDED
#define FRONTIER_Is_function_HPP_INCLUDED
#define FRONTIER_Is_function

#if __cplusplus < 201103L // pre-c++11 implementation (supports up to 20 arguments)

	namespace fm
	{
		/////////////////////////////////////////////////////////////
		/// @brief Has a member enum 'value' that is true if the template parameter is a function (without c++11 only up to 20 arguments)
		/// 
		/// @ingroup System
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		class Is_function
		{
		public:
			enum {
				value /** @cond DOXYGEN_HIDE */ = false /** @endcond */ ///< True if T is a function (without c++11 only up to 20 arguments)
			};
		};
		
		/// @cond DOXYGEN_HIDE
		/*************
		std::ofstream ki("asd.c");
		C(21)
		{
			ki<<"template<class R";
			Cx(i)
				ki<<",class A"<<x+1;
			ki<<">class Is_function<R(";
			Cx(i)
				ki<<(x?",A":"A")<<x+1;
			ki<<")>{public:enum{value = true};};\n";
		}
		ki.close();
		**************/
		
		template<class R>class Is_function<R()>{public:enum{value = true};};
		template<class R,class A1>class Is_function<R(A1)>{public:enum{value = true};};
		template<class R,class A1,class A2>class Is_function<R(A1,A2)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3>class Is_function<R(A1,A2,A3)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4>class Is_function<R(A1,A2,A3,A4)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5>class Is_function<R(A1,A2,A3,A4,A5)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6>class Is_function<R(A1,A2,A3,A4,A5,A6)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7>class Is_function<R(A1,A2,A3,A4,A5,A6,A7)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14,class A15>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14,class A15,class A16>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14,class A15,class A16,class A17>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14,class A15,class A16,class A17,class A18>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14,class A15,class A16,class A17,class A18,class A19>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19)>{public:enum{value = true};};
		template<class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14,class A15,class A16,class A17,class A18,class A19,class A20>class Is_function<R(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>{public:enum{value = true};};
		/// @endcond
		
	}

#else // c++11 implementation (supports any number of arguments)

	namespace fm
	{
		template<class T>
		class Is_function
		{
		public:
			enum {
				value = false
			};
		};

		template<class R,typename... Args>
		class Is_function<R(Args...)>
		{
		public:
			enum {
				value = true
			};
		};
	}

#endif // __cplusplus < 201103L

#endif // FRONTIER_Is_function_HPP_INCLUDED
