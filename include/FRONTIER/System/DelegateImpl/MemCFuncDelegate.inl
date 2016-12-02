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
#ifndef FRONTIER_MEMCFUNCDELEGATE_INL_INCLUDED
#define FRONTIER_MEMCFUNCDELEGATE_INL_INCLUDED
#include <utility>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline MemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::MemCFuncDelegate(typename MemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::FuncType memFuncPtr,ObjectType object) : m_pointer(memFuncPtr),
																																																m_object(std::move(object))
	{
		
	}
	
	namespace priv
	{
		template<class ObjectType,class R,class... Args>
		class ObjCFuncCaller
		{
		public:
			template<class... ExtraArgs>
			static inline R call(ObjectType &object,R (ObjectType::*funcPtr)(Args...) const,Args... args,ExtraArgs... eargs)
			{
				return (object .* funcPtr)(args...);
			}
			
		};
		
		template<class ObjectType,class AltR,class FuncArgList>
		class ObjCFuncCallerDeducer
		{
		public:
			template<class... CallArgs>
			using FuncCallerProt = ObjCFuncCaller<ObjectType,AltR,CallArgs...>;
			using type = typename FuncArgList::template Apply<FuncCallerProt>;
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline R MemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::call(Args... callArgs) const
	{
		return (R)priv::ObjCFuncCallerDeducer<ObjectType,AltR,FuncArgList>::type::call(m_object,m_pointer,callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline BaseDelegate<R,Args...> *MemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::makeCopy() const
	{
		return new MemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>(m_pointer);
	}
}

#endif // FRONTIER_MemCFuncDelegate_INL_INCLUDED
