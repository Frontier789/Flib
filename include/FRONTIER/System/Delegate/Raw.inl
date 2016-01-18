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


namespace fm
{
    #define FRONTIER_DELEGATE_IMPL DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE_IMPL::~DelegateImpl()
    {

    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    R FRONTIER_DELEGATE_IMPL::call(FRONTIER_DELEGATE_CALL_PARAMS)
    {
        return R();
    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    R FRONTIER_DELEGATE_IMPL::operator()(FRONTIER_DELEGATE_CALL_PARAMS)
    {
        return this->call(FRONTIER_DELEGATE_CALL_LIST);
    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE_IMPL *FRONTIER_DELEGATE_IMPL::makeNewCopy() const
    {
        return new FRONTIER_DELEGATE_IMPL ();
    }

    #define FRONTIER_DELEGATE Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST>
    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE::Delegate() : m_impl(new FRONTIER_DELEGATE_IMPL())
    {

    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE::Delegate(const FRONTIER_DELEGATE &d) : m_impl(new FRONTIER_DELEGATE_IMPL())
    {
        (*this) = d;
    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE::Delegate(const FRONTIER_DELEGATE_IMPL &delCont) : m_impl(delCont.makeNewCopy())
    {

    }

    #include <FRONTIER/System/Delegate/FuncImpl.hpp>

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE::Delegate(R (*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV)) : m_impl(new FuncDelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>(funcPtr))
    {

    }

    #include <FRONTIER/System/Delegate/MemberFuncImpl.hpp>

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    template<class Object>
    Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST>::Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),const typename Identity<Object>::type &object) : m_impl(new MemberFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>(funcPtr,object))
    {

    }

    #include <FRONTIER/System/Delegate/CallableImpl.hpp>

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    template<class Object>
    Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST>::Delegate(const Object &object,typename Enable_if<priv::IsCallable<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>::value,int>::type *) : m_impl(new CallableDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>(object))
    {

    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE::~Delegate()
    {

    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    R FRONTIER_DELEGATE::call(FRONTIER_DELEGATE_CALL_PARAMS)
    {
        return m_impl->call(FRONTIER_DELEGATE_CALL_LIST);
    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    R FRONTIER_DELEGATE::operator()(FRONTIER_DELEGATE_CALL_PARAMS)
    {
        return this->call(FRONTIER_DELEGATE_CALL_LIST);
    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL>
    FRONTIER_DELEGATE &FRONTIER_DELEGATE::operator=(const Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> &d)
    {
        delete m_impl;
		m_impl = d.m_impl->makeNewCopy();

		return *this;
    }
}

#include <FRONTIER/System/Delegate/Undef.hpp>
