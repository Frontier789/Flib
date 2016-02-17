template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class MemberFuncDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV);
	Object object;

	inline MemberFuncDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) = 0) : funcPtr(funcPtr)
	{

	}

	inline MemberFuncDelegateImpl(R (Object::*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV),const Object &object) : funcPtr(funcPtr),
                                                                                                              object(object)
	{

	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		if (funcPtr) return (object.*funcPtr)(FRONTIER_DELEGATE_CALL_LIST);
		return R();
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    MemberFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new MemberFuncDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->funcPtr = this->funcPtr;
	    ptr->object  = this->object;

        return ptr;
	}

	inline int getTypeID() const
    {
        return 2;
    }
};
