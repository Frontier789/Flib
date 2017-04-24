#ifndef HEY_ARRAY_GUARD_HPP
#define HEY_ARRAY_GUARD_HPP

#include <Frontier.hpp>

template<class T>
class HexArray
{
	fm::Size m_size;
	T m_uninited;
	T *m_data;
public:
	
	HexArray(fm::Size size = 0,T uninited = T());
	
	//HexArray(const HexArray<T> &arr);
	HexArray(HexArray<T> &&arr);
	HexArray<T> &operator=(HexArray<T> &&arr);
	HexArray<T> &swap(HexArray<T> &arr);
	
	~HexArray();
	
	void resize(fm::Size size);
	void clear();
	
	HexArray<T> extendToTwice() const;
	HexArray<T> reduceToHalf() const;

	void forAll(Delegate<void,T&,vec2i> func);
	void forAll(Delegate<void,const T&,vec2i> func) const;
	
	fm::Size getSize() const;
	fm::Size getArea() const;
	fm::Size getSliceArea() const;
	
	vector3<T> getNormal(vec2i p) const;
	vector2<T> getDrawPos(vec2i p) const;
	
	T &getHeight(vec2i p);
	T &getHeight(vec3i p);
	const T &getHeight(vec2i p) const;
	const T &getHeight(vec3i p) const;
	
	static vec2i transformPoint(vec2i p,int &offset);
	fm::Size locPosToOffset(vec2i locp) const;
	fm::Size posToIndex(vec2i p) const;
};

#endif

#include "HexArray.inl"
