template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class MemberFuncDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV);
	Object *object_ptr;

	inline MemberFuncDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) = 0) : funcPtr(funcPtr),
                                                                                                    object_ptr(fm::nullPtr)
	{

	}

	inline MemberFuncDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),const Object &object) : funcPtr(funcPtr),
                                                                                                                     object_ptr(new Object(object))
	{

	}

	inline ~MemberFuncDelegateImpl()
	{
        delete object_ptr;
	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		if (funcPtr && object_ptr) return (object_ptr->*funcPtr)(FRONTIER_DELEGATE_CALL_LIST);
		return R();
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    MemberFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new MemberFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->funcPtr    = this->funcPtr;
        ptr->object_ptr = this->object_ptr;

	    if (object_ptr)
            ptr->object_ptr = new Object(*object_ptr);

        return ptr;
	}

	inline int getTypeID() const
    {
        return 2;
    }
};

template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class MemberCFuncDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const;
	const Object *object_ptr;

	inline MemberCFuncDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const = 0) : funcPtr(funcPtr),
                                                                                                           object_ptr(fm::nullPtr)
	{

	}

	inline MemberCFuncDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const Object &object) : funcPtr(funcPtr),
                                                                                                                            object_ptr(new Object(object))
	{

	}

	inline ~MemberCFuncDelegateImpl()
	{
        delete object_ptr;
	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		if (funcPtr && object_ptr) return (object_ptr->*funcPtr)(FRONTIER_DELEGATE_CALL_LIST);
		return R();
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    MemberCFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new MemberCFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->funcPtr    = this->funcPtr;
        ptr->object_ptr = this->object_ptr;

	    if (object_ptr)
            ptr->object_ptr = new Object(*object_ptr);

        return ptr;
	}

	inline int getTypeID() const
    {
        return 2;
    }
};

template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class MemberFuncPtrDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV);
    Object *object_ptr;

	inline MemberFuncPtrDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) = 0) : funcPtr(funcPtr),
                                                                                                       object_ptr(fm::nullPtr)
	{

	}

	inline MemberFuncPtrDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),Object *object) : funcPtr(funcPtr),
                                                                                                                  object_ptr(object)
	{

	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		if (funcPtr && object_ptr) return (object_ptr->*funcPtr)(FRONTIER_DELEGATE_CALL_LIST);
		return R();
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    MemberFuncPtrDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new MemberFuncPtrDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->funcPtr    = this->funcPtr;
        ptr->object_ptr = this->object_ptr;

        return ptr;
	}

	inline int getTypeID() const
    {
        return 2;
    }
};

template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class MemberFuncCPtrDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const;
    const Object *object_ptr;

	inline MemberFuncCPtrDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const = 0) : funcPtr(funcPtr),
                                                                                                              object_ptr(fm::nullPtr)
	{

	}

	inline MemberFuncCPtrDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) const,const Object *object) : funcPtr(funcPtr),
                                                                                                                         object_ptr(object)
	{

	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		if (funcPtr && object_ptr) return (object_ptr->*funcPtr)(FRONTIER_DELEGATE_CALL_LIST);
		return R();
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    MemberFuncCPtrDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new MemberFuncCPtrDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->funcPtr    = this->funcPtr;
        ptr->object_ptr = this->object_ptr;

        return ptr;
	}

	inline int getTypeID() const
    {
        return 2;
    }
};
