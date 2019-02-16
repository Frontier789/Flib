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
#ifndef FRONTIER_DELEGATE_INL_INCLUDED
#define FRONTIER_DELEGATE_INL_INCLUDED

#include <FRONTIER/System/DelegateImpl/ObjMemCFuncDelegate.hpp>
#include <FRONTIER/System/DelegateImpl/ObjMemFuncDelegate.hpp>
#include <FRONTIER/System/DelegateImpl/MemCFuncDelegate.hpp>
#include <FRONTIER/System/DelegateImpl/MemFuncDelegate.hpp>
#include <FRONTIER/System/DelegateImpl/LambdaDelegate.hpp>
#include <FRONTIER/System/DelegateImpl/FuncDelegate.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::Delegate() : m_impl(new BaseDelegate<R,Args...>())
	{
		
	}
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::Delegate(decltype(nullptr)) : m_impl(new BaseDelegate<R,Args...>())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::Delegate(BaseDelegate<R,Args...> *newBaseDelegate) : m_impl(newBaseDelegate)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::Delegate(const Delegate<R,Args...> &copy) : m_impl(copy.m_impl->makeCopy())
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::Delegate(Delegate<R,Args...> &&move) : m_impl(move.m_impl)
	{
		move.m_impl = nullptr;
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::~Delegate()
	{
		delete m_impl;
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class FunctorT>
	inline Delegate<R,Args...>::Delegate(const FunctorT &functor,typename std::enable_if<fm::is_callable<FunctorT>::value,void>::type*) :
		m_impl(new LambdaDelegate<FunctorT,R,Args...>(functor))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class FunctorT>
	inline Delegate<R,Args...>::Delegate(FunctorT *functor,typename std::enable_if<fm::is_callable<FunctorT>::value,void>::type*) :
		Delegate(&FunctorT::operator(),functor)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class AltR,class... CallArgs>
	inline Delegate<R,Args...>::Delegate(AltR (*funcptr)(CallArgs...)) : m_impl(new FuncDelegate<AltR,ArgumentList<CallArgs...>,R,Args...>(funcptr))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class ObjectType,class AltR,class... CallArgs>
	inline Delegate<R,Args...>::Delegate(AltR (ObjectType::*memFunc)(CallArgs...),ObjectType object) : m_impl(new ObjMemFuncDelegate<ObjectType,AltR,ArgumentList<CallArgs...>,R,Args...>(memFunc,object))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class ObjectType,class AltR,class... CallArgs>
	inline Delegate<R,Args...>::Delegate(AltR (ObjectType::*memFunc)(CallArgs...) const,ObjectType object) : m_impl(new ObjMemCFuncDelegate<ObjectType,AltR,ArgumentList<CallArgs...>,R,Args...>(memFunc,object))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class ObjectType,class AltR,class... CallArgs>
	inline Delegate<R,Args...>::Delegate(AltR (ObjectType::*memFunc)(CallArgs...),ObjectType *pointer) : m_impl(new ObjMemFuncDelegate<ObjectType,AltR,ArgumentList<CallArgs...>,R,Args...>(memFunc,pointer))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class ObjectType,class AltR,class... CallArgs>
	inline Delegate<R,Args...>::Delegate(AltR (ObjectType::*memFunc)(CallArgs...) const,const ObjectType *pointer) : m_impl(new ObjMemCFuncDelegate<ObjectType,AltR,ArgumentList<CallArgs...>,R,Args...>(memFunc,pointer))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class... ExtraArgs>
	inline R Delegate<R,Args...>::call(Args... callArgs,ExtraArgs...) const
	{
		return m_impl->call(callArgs...);
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	template<class... ExtraArgs>
	inline R Delegate<R,Args...>::operator()(Args... callArgs,ExtraArgs...) const
	{
		return call(callArgs...);
	}
	
	
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...>::operator bool() const
	{
		return !m_impl->isBaseDelegate();
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline bool Delegate<R,Args...>::operator!() const
	{
		return m_impl->isBaseDelegate();
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...> &Delegate<R,Args...>::operator=(const Delegate<R,Args...> &copy)
	{
		delete m_impl;
		
		m_impl = copy.m_impl->makeCopy();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class R,class... Args>
	inline Delegate<R,Args...> &Delegate<R,Args...>::operator=(Delegate<R,Args...> &&move)
	{
		delete m_impl;
		
		m_impl = move.m_impl;
		
		move.m_impl = nullptr;
		
		return *this;
	}
}

#endif // FRONTIER_DELEGATE_INL_INCLUDED
