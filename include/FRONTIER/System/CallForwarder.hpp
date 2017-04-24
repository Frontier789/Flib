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
#ifndef FRONTIER_CALLFORWARDER_HPP_INCLUDED
#define FRONTIER_CALLFORWARDER_HPP_INCLUDED

namespace fm
{
	namespace priv
	{
		template<bool,class FuncType,class RType,class... ArgTypes>
		class DoesReturnMatch
		{
			class YES {char c[2];};
			typedef char NO;
			
			template<class T>
			static T instate();
			
			template<class T> static YES convertTest(T);
			template<class  > static NO  convertTest(...);
			
		public:
			enum {
				value = sizeof(convertTest<RType>((*(FuncType*)nullptr)(instate<ArgTypes>()...))) == sizeof(YES)
			};	
		};
		
		template<class FuncType,class... ArgTypes>
		class DoesReturnMatch<true,FuncType,void,ArgTypes...>
		{
		public:
			enum {
				value = true
			};	
		};

		template<class F,class R,class... Param>
		class DoesReturnMatch<false,F,R,Param...>
		{
		public:
			enum {
				value = false
			};	
		};

		template<class FuncType,class RType,class... ArgTypes>
		class IsCallableWith
		{
			class YES {char c[2];};
			typedef char NO;
			
			template<class A,class B>
			class Second
			{
			public:
				typedef B type;
			};
			
			template<class TFuncType,class... TArgTypes> 
			static auto test(TFuncType func,TArgTypes... args) -> typename Second<decltype(func(args...)),YES>::type;
			
			template<class TFuncType,class... TArgTypes> 
			static NO test(...);
			
			template<class T>
			static T instate();
			
		public:
			enum {
				value = DoesReturnMatch<sizeof(test<FuncType,ArgTypes...>((*(FuncType*)nullptr),instate<ArgTypes>()... )) == sizeof(YES),FuncType,RType,ArgTypes...>::value
			};
		};

		template<bool,class FuncType,class RType,class... UnusedArgs>
		class CallDeducer
		{
		public:
			
			template<class... UsedArgs>
			class Helper
			{
			public:
				static RType call(FuncType func,UsedArgs... usedArgs,UnusedArgs... /* unusedArgs */)
				{
					return func(usedArgs...);
				}
				
				static RType call(FuncType *func,UsedArgs... usedArgs,UnusedArgs... /* unusedArgs */)
				{
					return (*func)(usedArgs...);
				}

				enum {
					Callable = true
				};
			};
		};

		template<class FuncType,class RType,class UnusedArgsHead,class... UnusedArgsTail>
		class CallDeducer<false,FuncType,RType,UnusedArgsHead,UnusedArgsTail...>
		{
		public:
			template<class... UsedArgs>
			class Helper : public CallDeducer<IsCallableWith<FuncType,RType,UsedArgs...,UnusedArgsHead>::value,
											  FuncType,RType,UnusedArgsTail...>::
											  template Helper<UsedArgs...,UnusedArgsHead>
			{
				
			};
		};

		template<class FuncType,class RType>
		class CallDeducer<false,FuncType,RType>
		{
		public:
			template<class... UsedArgs>
			class Helper
			{
			public:
				enum {
					Callable = false
				};
			};
		};
	}
	
	template<class FuncType,class RType,class... ArgTypes>
	class CallForwarder : public priv::CallDeducer<priv::IsCallableWith<FuncType,RType>::value,
												   FuncType,RType,ArgTypes...>::template Helper<>
	{
		
	};		
}
#endif // FRONTIER_CALLFORWARDER_HPP_INCLUDED
