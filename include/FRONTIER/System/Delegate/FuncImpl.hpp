template<class R FRONTIER_DELEGATE_TEMPLATE_PARAMS>
class FuncDelegateImpl FRONTIER_DELEGATE_TEMPLATE_SPEC : public DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>
{
public:
	R (*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV);

	inline FuncDelegateImpl(R (*funcPtr)(FRONTIER_DELEGATE_TEMPLATE_CALL_CONV) = 0) : funcPtr(funcPtr)
	{

	}

	inline R call(FRONTIER_DELEGATE_CALL_PARAMS)
	{
		if (funcPtr) return funcPtr(FRONTIER_DELEGATE_CALL_LIST);
		return R();
	}

	inline DelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *makeNewCopy() const
	{
	    FuncDelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST> *ptr = new FuncDelegateImpl<R FRONTIER_DELEGATE_TEMPLATE_LIST>();
	    ptr->funcPtr = this->funcPtr;

        return ptr;
	}

	inline int getTypeID() const
    {
        return 3;
    }
};
