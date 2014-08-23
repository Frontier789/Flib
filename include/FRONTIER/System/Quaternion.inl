#ifndef FRONTIER_QUATERNION_INL_INCLUDED
#define FRONTIER_QUATERNION_INL_INCLUDED
namespace fm
{
	template<MATRIX::StorageOrder storeOrder>
	inline mat4 Quat::getMatrix() const
	{
		return getMatrix(storeOrder);
	}
}

#endif