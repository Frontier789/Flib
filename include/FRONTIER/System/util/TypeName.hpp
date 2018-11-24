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

#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <ostream>

// 
// credit for function and macros to 
// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
// 

#ifndef _MSC_VER
  #if __cplusplus < 201103
    #define FRONTIER_CONSTEXPR_CPP11_TN
    #define FRONTIER_CONSTEXPR_CPP14_TN
    #define FRONTIER_NEXCEPT_TN
  #elif __cplusplus < 201402
    #define FRONTIER_CONSTEXPR_CPP11_TN constexpr
    #define FRONTIER_CONSTEXPR_CPP14_TN
    #define FRONTIER_NEXCEPT_TN noexcept
  #else
    #define FRONTIER_CONSTEXPR_CPP11_TN constexpr
    #define FRONTIER_CONSTEXPR_CPP14_TN constexpr
    #define FRONTIER_NEXCEPT_TN noexcept
  #endif
#else  // _MSC_VER
  #if _MSC_VER < 1900
    #define FRONTIER_CONSTEXPR_CPP11_TN
    #define FRONTIER_CONSTEXPR_CPP14_TN
    #define FRONTIER_NEXCEPT_TN
  #elif _MSC_VER < 2000
    #define FRONTIER_CONSTEXPR_CPP11_TN constexpr
    #define FRONTIER_CONSTEXPR_CPP14_TN
    #define FRONTIER_NEXCEPT_TN noexcept
  #else
    #define FRONTIER_CONSTEXPR_CPP11_TN constexpr
    #define FRONTIER_CONSTEXPR_CPP14_TN constexpr
    #define FRONTIER_NEXCEPT_TN noexcept
  #endif
#endif  // _MSC_VER

namespace fm
{
	namespace priv
	{
		class StaticString
		{
			const char *const p_;
			const std::size_t sz_;

		public:
			template <std::size_t N>
			FRONTIER_CONSTEXPR_CPP11_TN StaticString(const char(&a)[N]) FRONTIER_NEXCEPT_TN
				: p_(a)
				, sz_(N-1)
				{}

			FRONTIER_CONSTEXPR_CPP11_TN StaticString(const char* p, std::size_t N) FRONTIER_NEXCEPT_TN
				: p_(p)
				, sz_(N)
				{}

			FRONTIER_CONSTEXPR_CPP11_TN const char* data() const FRONTIER_NEXCEPT_TN {return p_;}
			FRONTIER_CONSTEXPR_CPP11_TN std::size_t size() const FRONTIER_NEXCEPT_TN {return sz_;}
		};
	}
	
	using priv::StaticString;

	inline std::ostream &operator<<(std::ostream& os, StaticString const& s)
	{
		return os.write(s.data(), s.size());
	}

	template <class T>
	FRONTIER_CONSTEXPR_CPP14_TN StaticString typeName()
	{
	#ifdef __clang__
		StaticString p = __PRETTY_FUNCTION__;
		return StaticString(p.data() + 34, p.size() - 34 - 1);
	#elif defined(__GNUC__)
		StaticString p = __PRETTY_FUNCTION__;
	  #if __cplusplus < 201402
		return StaticString(p.data() + 36, p.size() - 36 - 1);
	  #else
		return StaticString(p.data() + 49, p.size() - 49);
	  #endif
	#elif defined(_MSC_VER)
		StaticString p = __FUNCSIG__;
		return StaticString(p.data() + 84, p.size() - 84 - 7);
	#endif
	}
}

#undef FRONTIER_CONSTEXPR_CPP11_TN
#undef FRONTIER_CONSTEXPR_CPP14_TN
#undef FRONTIER_NEXCEPT_TN
