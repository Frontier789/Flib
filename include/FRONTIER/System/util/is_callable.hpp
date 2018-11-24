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

#include <type_traits>

// 
// credit for function to
// https://stackoverflow.com/questions/15393938/find-out-if-a-c-object-is-callable
// 

namespace fm
{
	namespace priv 
	{
		template<typename T>
		struct is_callable_impl {
		private:
				typedef char(&yes)[1];
				typedef char(&no)[2];

				struct Fallback { void operator()(); };
				struct Derived : T, Fallback { };

				template<typename U, U> struct Check;

				template<typename>
				static yes test(...);

				template<typename C>
				static no test(Check<void (Fallback::*)(), &C::operator()>*);

		public:
				static const bool value = sizeof(test<Derived>(nullptr)) == sizeof(yes);
		};
	}
	
	template<typename T>
	struct is_callable
			: std::conditional<
					std::is_class<T>::value,
					fm::priv::is_callable_impl<T>,
					std::false_type
			>::type
	{ };
}