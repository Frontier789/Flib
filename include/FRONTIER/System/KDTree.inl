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
#ifndef FRONTIER_KDTREE_INL_INCLUDED
#define FRONTIER_KDTREE_INL_INCLUDED
#include <algorithm>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType>::KDTree() : m_distFunc(defaultDistanceFunc)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType>::KDTree(const KDTree<VectorType,Dim,AxisType> &copy) : m_distFunc(defaultDistanceFunc)
	{
		(*this) = copy;
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType>::KDTree(KDTree<VectorType,Dim,AxisType> &&move) : m_distFunc(defaultDistanceFunc)
	{
		move.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType> &KDTree<VectorType,Dim,AxisType>::swap(KDTree<VectorType,Dim,AxisType> &tree)
	{
		m_axes.swap(tree.m_axes);
		m_data.swap(tree.m_data);
		std::swap(m_distFunc,tree.m_distFunc);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType> &KDTree<VectorType,Dim,AxisType>::operator=(const KDTree &tree)
	{
		m_axes = tree.m_axes;
		m_data = tree.m_data;
		m_distFunc = tree.m_distFunc;
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType> &KDTree<VectorType,Dim,AxisType>::operator=(KDTree &&tree)
	{
		return this->swap(tree);
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType>::KDTree(VectorType *ptr,
												   fm::Size vecCount,
												   fm::Delegate<AxisType,
																const VectorType &,
																const VectorType &> distFunc) : m_axes(vecCount,AxisType()),
																								m_data(ptr,ptr+vecCount),
																								m_distFunc(distFunc)
	{
		buildKDTRange(0,vecCount,0);
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline KDTree<VectorType,Dim,AxisType>::KDTree(std::vector<VectorType> &&data,
												   fm::Delegate<AxisType,
																const VectorType &,
																const VectorType &> distFunc) : m_axes(data.size()),
																								m_data(std::move(data)),
																								m_distFunc(distFunc)
	{
		buildKDTRange(0,m_data.size(),0);
	}
		
	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline AxisType KDTree<VectorType,Dim,AxisType>::defaultDistanceFunc(const VectorType &v1,const VectorType &v2)
	{
		AxisType ret = AxisType();
		
		for (fm::Size i=0;i<Dim;++i)
			ret += (v1[i] - v2[i])*(v1[i] - v2[i]);
		
		return ret;
	}
		
	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline fm::Size KDTree<VectorType,Dim,AxisType>::getSize() const
	{
		return m_data.size();
	}
	
	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline const AxisType *KDTree<VectorType,Dim,AxisType>::getAxesPtr() const
	{
		return &m_axes[0];
	}
	
	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline const VectorType *KDTree<VectorType,Dim,AxisType>::getPointsPtr() const
	{
		return &m_data[0];
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	inline AxisType KDTree<VectorType,Dim,AxisType>::getDist1Dim(const AxisType &a1,const AxisType &a2) const
	{
		VectorType v1 = m_data[0];
		VectorType v2 = v1;
		
		v1[0] = a1;
		v2[0] = a2;
		
		for (fm::Size i=1;i<Dim;++i)
		{
			v1[i] = AxisType();
			v2[i] = AxisType();
		}
		
		return m_distFunc(v1,v2);
	}

	namespace priv
	{
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size Dim = T::components>
		class Sorter
		{
		public:
			fm::Size dim;
			Sorter(fm::Size dim) : dim(dim%Dim) {}
			
			bool operator()(const T &a,const T &b) const
			{
				return a[dim] < b[dim];
			}
		};	
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	void KDTree<VectorType,Dim,AxisType>::buildKDTRange(fm::Size first,fm::Size last,fm::Size depth)
	{
		if (first+1 == last) return;
		
		fm::Size mid = (first+last)/2;
		
		std::nth_element(m_data.begin() + first, 
						 m_data.begin() + mid, 
						 m_data.begin() + last,
						 fm::priv::Sorter<vec2>(depth));
		
		m_axes[mid] = m_data[mid][depth%Dim];
		
		buildKDTRange(first,mid,depth+1);
		buildKDTRange(mid,last,depth+1);
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	VectorType KDTree<VectorType,Dim,AxisType>::getNearestRange(VectorType p,
																fm::Size first,
																fm::Size last,
																fm::Size depth) const
	{
		if (first+1 == last) return m_data[first];
		
		fm::Size mid = (first+last)/2;
		
		bool pInLeft = m_axes[mid] > p[depth%Dim];
		VectorType estimate;

		if (pInLeft) estimate = getNearestRange(p,first,mid,depth+1);
		else		 estimate = getNearestRange(p,mid,last,depth+1);
		
		AxisType pToEstDist  = m_distFunc(estimate,p);
		AxisType pToAxisDist = getDist1Dim(m_axes[mid] , p[depth%Dim]);
		
		if (pToAxisDist < pToEstDist)
		{
			VectorType estimate2;
			
			if (pInLeft) estimate2 = getNearestRange(p,mid,last,depth+1);
			else		 estimate2 = getNearestRange(p,first,mid,depth+1);
			
			if (m_distFunc(estimate2,p) < pToEstDist)
				return estimate2;
		}
		return estimate;
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	VectorType KDTree<VectorType,Dim,AxisType>::getNearest(VectorType p) const
	{
		return getNearestRange(p,0,m_data.size(),0);
	}

	/////////////////////////////////////////////////////////////
	template<class VectorType,fm::Size Dim,class AxisType>
	VectorType KDTree<VectorType,Dim,AxisType>::operator()(VectorType p) const
	{
		return getNearestRange(p,0,m_data.size(),0);
	}
}

#endif // FRONTIER_KDTREE_INL_INCLUDED
