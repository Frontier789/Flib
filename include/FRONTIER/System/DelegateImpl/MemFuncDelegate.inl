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
#ifndef FRONTIER_MEMFUNCDELEGATE_INL_INCLUDED
#define FRONTIER_MEMFUNCDELEGATE_INL_INCLUDED
#include <utility>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline MemFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::MemFuncDelegate(typename MemFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::FuncType memFuncPtr,ObjectType object) : m_pointer(memFuncPtr),
																																															 m_object(std::move(object))
	{
		
	}
	
	namespace priv
	{
		template<class ObjectType,class R,class... Args>
		class ObjFuncCaller
		{
		public:
			template<class... ExtraArgs>
			static inline R call(ObjectType &object,R (ObjectType::*funcPtr)(Args...),Args... args,ExtraArgs... eargs)
			{
				return (object .* funcPtr)(args...);
			}
			
		};
		
		template<class ObjectType,class AltR,class FuncArgList>
		class ObjFuncCallerDeducer
		{
		public:
			template<class... CallArgs>
			using FuncCallerProt = ObjFuncCaller<ObjectType,AltR,CallArgs...>;
			using type = typename FuncArgList::template Apply<FuncCallerProt>;
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline R MemFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::call(Args... callArgs) const
	{
		return (R)priv::ObjFuncCallerDeducer<ObjectType,AltR,FuncArgList>::type::call(m_object,m_pointer,callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline BaseDelegate<R,Args...> *MemFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::makeCopy() const
	{
		return new MemFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>(m_pointer);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	bool MemFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::isBaseDelegate() const
	{
		return false;
	}
}

#endif // FRONTIER_MEMFUNCDELEGATE_INL_INCLUDED
