#ifndef HEY_ARRAY_GUARD_INL
#define HEY_ARRAY_GUARD_INL
/*
template<class T>
inline HexArray<T>::HexArray(const HexArray<T> &arr) : m_size(0), m_data(nullptr)
{
	m_uninited = arr.m_uninited;
	m_size = arr.m_size;
	fm::Size area = getArea();
	
	m_data = new T[area];
	
	C(area)
		m_data[i] = arr.m_data[i];
}
*/
template<class T>
inline HexArray<T>::HexArray(HexArray<T> &&arr) : m_size(0), m_data(nullptr)
{
	this->swap(arr);
}

template<class T>
inline HexArray<T> &HexArray<T>::operator=(HexArray<T> &&arr)
{
	return this->swap(arr);
}

template<class T>
HexArray<T> &HexArray<T>::swap(HexArray<T> &arr)
{
	std::swap(m_size,arr.m_size);
	std::swap(m_uninited,arr.m_uninited);
	std::swap(m_data,arr.m_data);
	
	return *this;
}

template<class T>
inline HexArray<T>::HexArray(fm::Size size,T uninited) : m_uninited(uninited), m_data(nullptr)
{
	resize(size);
}

template<class T>
inline HexArray<T>::~HexArray()
{
	delete[] m_data;
}

template<class T>
inline void HexArray<T>::resize(fm::Size size)
{
	delete[] m_data;
	
	m_size = size;
	fm::Size area = getArea();
	
	m_data = new T[area];
	
	C(area)
		m_data[i] = m_uninited;
}

template<class T>
inline void HexArray<T>::clear()
{
	C(getArea())
		m_data[i] = m_uninited;
}
	
template<class T>
inline HexArray<T> HexArray<T>::extendToTwice() const
{
	HexArray<T> ret(m_size*2,m_uninited);
	
	forAll([&](const T &val,vec2i p){
		
		ret.getHeight(p*2) = val;
		
	});
	
	return ret;
}

template<class T>
inline HexArray<T> HexArray<T>::reduceToHalf() const
{
	HexArray<T> ret(m_size/2,m_uninited);

	ret.forAll([&](T &val,vec2i p){
		
		ret.getHeight(p) = getHeight(p*2);

	});

	return ret;
}

namespace priv
{
	template<class HexArray_t,class Delegate_t>
	void forAllHelper(HexArray_t &hexAr,Delegate_t del)
	{
		vec2i arrayBases[] = {vec2i(1,0),vec2i(0,1),vec2i(-1,1),vec2i(-1,0),vec2i(0,-1),vec2i(1,-1),     vec2i(1,0)};
	
		del(hexAr.getHeight(vec2i()),vec2i());
		
		C(6)
		{
			vec2i base_i = arrayBases[i];
			vec2i base_j = arrayBases[i+1];
			
			Cxy(hexAr.getSize(),hexAr.getSize()-x)
			{
				vec2i p = base_i * x + base_j * y + base_i;
				
				del(hexAr.getHeight(p),p);
			}
		}
	}
}

template<class T>
inline void HexArray<T>::forAll(Delegate<void,T&,vec2i> func)
{
	::priv::forAllHelper(*this,func);
}

template<class T>
inline void HexArray<T>::forAll(Delegate<void,const T&,vec2i> func) const
{
	::priv::forAllHelper(*this,func);
}

template<class T>
inline fm::Size HexArray<T>::getSize() const
{
	return m_size;
}

template<class T>
inline fm::Size HexArray<T>::getArea() const
{
	return 1 + (m_size + m_size*m_size)*3;
}

template<class T>
inline fm::Size HexArray<T>::getSliceArea() const
{
	return (getArea()-1)/6;
}

template<class T>
inline vector3<T> HexArray<T>::getNormal(vec2i p) const
{
	auto isInRange = [&](vec2i pt) -> bool {
		
		if ((pt.x <= 0 && pt.y >= 0) || 
			(pt.x >= 0 && pt.y <= 0))
		{
			return std::max<fm::Size>(std::abs(pt.x),std::abs(pt.y)) <= m_size;
		}
		
		return std::abs(pt.x + pt.y) <= m_size;
	};
	
	const T h_scale = T(5.0);
	
	const T &height_C = getHeight(p);
	vector3<T> Cin3D(getDrawPos(p),height_C*h_scale);
	
	auto getTriangleNormal = [&](vec2i A,vec2i B) -> vector3<T> {
		
		T height_A = isInRange(A) ? getHeight(A) : height_C;
		T height_B = isInRange(B) ? getHeight(B) : height_C;
		
		vector3<T> Ain3D(getDrawPos(A),height_A*h_scale);
		vector3<T> Bin3D(getDrawPos(B),height_B*h_scale);
		
		vector3<T> a = Ain3D - Bin3D;
		vector3<T> b = Cin3D - Bin3D;
		
		return a.cross(b).sgn();
	};
	
	vec2i pts[] = {vec2i(1,0),vec2i(0,1),vec2i(-1,1),vec2i(-1,0),vec2i(0,-1),vec2i(1,-1),  vec2i(1,0)};
	
	vector3<T> normalSum;
	
	C(6)
	{
		normalSum += getTriangleNormal(p + pts[i],p + pts[i+1]);
	}
	
	return normalSum.sgn() * (height_C < T(0) ? T(0.8) : T(1));
}

template<class T>
inline vector2<T> HexArray<T>::getDrawPos(vec2i p) const
{
	vector2<T> midpBase_i = polar2<T>(T(1),deg(30));
	vector2<T> midpBase_j = polar2<T>(T(1),deg(90));
	
	return p.x * midpBase_i + p.y * midpBase_j;
}

template<class T>
inline vec2i HexArray<T>::transformPoint(vec2i p,int &offset)
{
	vec2i transp;
	
	if (p.x >  0 && p.y >= 0)       offset = 0, transp = vec2i( p.x       - 1, p.y      );
	if (p.x <= 0 && p.x + p.y >  0) offset = 1, transp = vec2i( p.x + p.y - 1,-p.x      );
	if (p.y >  0 && p.x + p.y <= 0) offset = 2, transp = vec2i( p.y       - 1,-p.x - p.y);
	if (p.x <  0 && p.y <= 0)       offset = 3, transp = vec2i(-p.x       - 1,-p.y      );
	if (p.x >= 0 && p.x + p.y <  0) offset = 4, transp = vec2i(-p.x - p.y - 1, p.x      );
	if (p.y <  0 && p.x + p.y >= 0) offset = 5, transp = vec2i(-p.y       - 1, p.x + p.y);
	
	return transp;
}

template<class T>
inline fm::Size HexArray<T>::locPosToOffset(vec2i locp) const
{
	return locp.y*m_size - (locp.y*locp.y - locp.y)/2 + locp.x;
}

template<class T>
inline fm::Size HexArray<T>::posToIndex(vec2i p) const
{
	if (p == vec2i()) return 0;
	
	int offset = 0;
	vec2i locp = transformPoint(p,offset);
	
	fm::Size index = 1 + offset * getSliceArea() + locPosToOffset(locp);
	
	return index;
}

template<class T>
inline T &HexArray<T>::getHeight(vec2i p)
{
	return m_data[posToIndex(p)];
}

template<class T>
inline T &HexArray<T>::getHeight(vec3i p)
{
	return getHeight(vec2i(p.x-p.z,p.y+p.z));
}

template<class T>
inline const T &HexArray<T>::getHeight(vec2i p) const 
{
	return m_data[posToIndex(p)];
}

template<class T>
inline const T &HexArray<T>::getHeight(vec3i p) const 
{
	return getHeight(vec2i(p.x-p.z,p.y+p.z));
}

#endif
