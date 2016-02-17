template<class Object,class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class CallableDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC_OBJ : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	Object object;

	inline CallableDelegateImpl()
	{

	}

	inline CallableDelegateImpl(const Object &object) : object(object)
	{

	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		return object(FRONTIER_DELEGATE_CALL_LIST);
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    CallableDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new CallableDelegateImpl<Object,R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->object  = this->object;

        return ptr;
	}

	inline int getTypeID() const
    {
        return 1;
    }
};
