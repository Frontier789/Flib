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
#ifndef FRONTIER_LAMBDADELEGATE_INL_INCLUDED
#define FRONTIER_LAMBDADELEGATE_INL_INCLUDED
#include <type_traits>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	LambdaDelegate<LambdaT,R,Args...>::LambdaDelegate(LambdaT lambda) : m_lambda(lambda)
	{
		
	}
	
	namespace priv
	{
		template<class LambdaT,class R,class... Args>
		class LambdaCaller
		{
		public:
			static inline R call(LambdaT lambda)
			{
				return lambda();
			}
		};
		
		template<bool lambdaCurCallable>
		class CallForwarder
		{
		public:
			template<class LambdaT,class R,class... Args>
			static inline R call(LambdaT lambda,Args... args)
			{
				return lambda(args...);
			}
		};
		
		template<>
		class CallForwarder<false>
		{
		public:
			template<class LambdaT,class R,class SkipArg,class... Args>
			static inline R call(LambdaT lambda,SkipArg skipArg,Args... args)
			{
				return LambdaCaller<LambdaT,R,Args...>::call(lambda,args...);
			}
		};
		
		template<class LambdaT,class R,class FirstArg,class... ArgsTail>
		class LambdaCaller<LambdaT,R,FirstArg,ArgsTail...>
		{
			class YES
			{
				char c[2];
			};
			
			typedef char NO;
			
			template<class A,class B>
			class Second
			{
			public:
				typedef B type;
			};
			
			template<class SLambdaT,class SFirstArg,class... SArgsTail> 
			static auto test(SLambdaT lambda,SFirstArg firstArg,SArgsTail... argsTail) -> typename Second<decltype(lambda(firstArg,argsTail...)),YES>::type;
			
			template<class SLambdaT,class SFirstArg,class... SArgsTail> 
			static NO  test(...);    
		public:
			
			static inline R call(LambdaT lambda,FirstArg firstArg,ArgsTail... argsTail)
			{
				return CallForwarder<
									 sizeof(test<LambdaT,FirstArg,ArgsTail...>( lambda,firstArg,argsTail... )) == sizeof(YES)
									>::template call<LambdaT,R,FirstArg,ArgsTail...>(lambda,firstArg,argsTail...);
			}
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	R LambdaDelegate<LambdaT,R,Args...>::call(Args... callArgs) const
	{
		return priv::LambdaCaller<LambdaT,R,Args...>::call(m_lambda,callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	BaseDelegate<R,Args...> *LambdaDelegate<LambdaT,R,Args...>::makeCopy() const
	{
		return new LambdaDelegate<LambdaT,R,Args...>(m_lambda);
	}
	
	/////////////////////////////////////////////////////////////
	template<class LambdaT,class R,class... Args>
	bool LambdaDelegate<LambdaT,R,Args...>::isBaseDelegate() const
	{
		return false;
	}
}

#endif // FRONTIER_LAMBDADELEGATE_INL_INCLUDED
