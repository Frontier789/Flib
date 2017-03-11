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
#ifndef FRONTIER_OBJMEMFUNCDELEGATE_INL_INCLUDED
#define FRONTIER_OBJMEMFUNCDELEGATE_INL_INCLUDED

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline ObjMemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::ObjMemCFuncDelegate(typename ObjMemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::FuncType funcPtr,fm::Ref<ObjectType> obj) : m_pointer(funcPtr),
																																																		 m_objptr(obj)
	{
		
	}
	
	namespace priv
	{
		template<class ObjectType,class R,class... Args>
		class ObjMemCFuncCaller
		{
		public:
			template<class... ExtraArgs>
			static inline R call(ObjectType *object,R (ObjectType::*funcPtr)(Args...) const,Args... args,ExtraArgs...)
			{
				return (object ->* funcPtr)(args...);
			}
			
		};
		
		template<class ObjectType,class AltR,class FuncArgList>
		class ObjMemCFuncCallerDeducer
		{
		public:
			template<class... CallArgs>
			using FuncCallerProt = ObjMemCFuncCaller<ObjectType,AltR,CallArgs...>;
			using type = typename FuncArgList::template Apply<FuncCallerProt>;
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline R ObjMemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::call(Args... callArgs) const
	{
		return (R)priv::ObjMemCFuncCallerDeducer<ObjectType,AltR,FuncArgList>::type::call(m_objptr,m_pointer,callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	inline BaseDelegate<R,Args...> *ObjMemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::makeCopy() const
	{
		return new ObjMemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>(m_pointer,m_objptr);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ObjectType,class AltR,class FuncArgList,class R,class... Args>
	bool ObjMemCFuncDelegate<ObjectType,AltR,FuncArgList,R,Args...>::isBaseDelegate() const
	{
		return false;
	}
}

#endif // FRONTIER_OOBJMEMFUNCDELEGATE_INL_INCLUDED
