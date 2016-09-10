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

namespace fm
{
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
    class DelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC : public NonCopyable
    {
    public:

        inline virtual ~DelegateImpl();

        inline virtual R call(FRONTIER_DELEGATE_CALL_PARAMS);

        inline R operator()(FRONTIER_DELEGATE_CALL_PARAMS);

        inline virtual DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const;

		inline virtual int getTypeID() const;
    };

    /////////////////////////////////////////////////////////////
    namespace priv
    {
        template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
        class IsCallable FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ
        {
        public:
            typedef char Yes;
            class No
            {
                Yes t[2];
            };

            template <class U, U> class ReallHas;

            template <class Cls,class Rc FRONTIER_DELEGATE_TEMPLATE_PARAMSc> static Yes Test(ReallHas <Rc (Cls::*)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONVc),&Cls::operator()> *);

            template <class Cls,class Rc FRONTIER_DELEGATE_TEMPLATE_PARAMSc> static No& Test(...);

            public:
            enum {
                value = sizeof(Test<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>(0)) == sizeof(Yes)
            };
        };
    }

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
    class Delegate FRONTIER_DELEGATE_TEMPLATE_SPEC
    {
		mutable DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *m_impl;
    public:
        inline Delegate();

        inline Delegate(fm::priv::NullPtr theNullPtr);

        inline Delegate(const Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> &d);

        inline Delegate(const DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> &delCont);

        inline Delegate(R (*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV));

        template<class Object>
        inline Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),const typename Identity<Object>::type &object);

        template<class Object>
        inline Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const typename Identity<Object>::type &object);

        template<class Object>
        inline Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),typename Identity<Object>::type *object);

        template<class Object>
        inline Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const typename Identity<Object>::type *object);

        template<class Object>
        inline Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV));

        template<class Object>
        inline Delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const);

        template<class Object>
        inline Delegate(const Object &object,typename Enable_if<priv::IsCallable<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>::value,int>::type * = 0);

        inline ~Delegate();

		inline R call(FRONTIER_DELEGATE_CALL_PARAMS) const;

		inline R operator()(FRONTIER_DELEGATE_CALL_PARAMS) const;

        inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> &operator=(const Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> &d);

		inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> &operator=(fm::priv::NullPtr theNullPtr);

		inline operator bool() const;
    };
    
    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),const typename Identity<Object>::type &object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const typename Identity<Object>::type &object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),typename Identity<Object>::type *object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const typename Identity<Object>::type *object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV));

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> makeDelegate(const Object &object,typename Enable_if<priv::IsCallable<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>::value,int>::type * = 0);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),const typename Identity<Object>::type &object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const typename Identity<Object>::type &object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),typename Identity<Object>::type *object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const typename Identity<Object>::type *object);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV));

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const);

    /////////////////////////////////////////////////////////////
    template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS_INL,class Object>
	inline Delegate<R FRONTIER_DELEGATE_TEMPLATE_LIST> delegate(const Object &object,typename Enable_if<priv::IsCallable<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>::value,int>::type * = 0);

}

#include <FRONTIER/System/Delegate/Undef.hpp>
