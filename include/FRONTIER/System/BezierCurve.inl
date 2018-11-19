////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_BEZIERCURVE_INL_INCLUDED
#define FRONTIER_BEZIERCURVE_INL_INCLUDED
#include <FRONTIER/System/util/C.hpp>

namespace fm
{
	namespace priv
	{
		template<class T>
		void init_bezier(BezierCurve<T> &,fm::Size) {
			
		}
		
		template<class T>
		void init_bezier(BezierCurve<BezierCurve<T>> &surface,fm::Size n) {
			for (auto &curve : surface)
				curve.resize(n);
		}
	}
	
	template<class T>
	/////////////////////////////////////////////////////////////
	inline void BezierCurve<T>::resize(fm::Size size)
	{
		m_controlPts.resize(size);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Size BezierCurve<T>::size() const
	{
		return m_controlPts.size();
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline BezierCurve<T>::BezierCurve(fm::Size n) :
		m_controlPts(n)
	{
		priv::init_bezier(*this, n);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline BezierCurve<T>::BezierCurve(fm::Size n,T val) :
		m_controlPts(n,val)
	{
		
	}
	
	/////////////////////////////////////////////////////////////	
	template<class T>
	inline T &BezierCurve<T>::operator[](fm::Size index)
	{
		return m_controlPts[index];
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline const T &BezierCurve<T>::operator[](fm::Size index) const
	{
		return m_controlPts[index];
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T BezierCurve<T>::deCasteljou(double t) const
	{
		auto pts = m_controlPts;
		
		for (fm::Size lvl=0;lvl<size();++lvl)
			for (fm::Size i=1;i<size()-lvl;++i)
				pts[i-1] = pts[i-1] * (1-t) + pts[i] * t;
		
		return std::move(pts[0]);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T BezierCurve<T>::eval(double t) const
	{
		return deCasteljou(t);
	}
	
	/////////////////////////////////////////////////////////////	
	template<class T>
	inline BezierCurve<T> operator*(const BezierCurve<T> &curve,double t)
	{
		BezierCurve<T> cpy(curve.size());
		
		C(curve.size()) cpy[i] = curve[i] * t;
		
		return cpy;
	}
	
	/////////////////////////////////////////////////////////////	
	template<class T>
	inline BezierCurve<T> operator+(const BezierCurve<T> &left,const BezierCurve<T> &right)
	{
		auto s = std::min(left.size(),right.size());
		
		BezierCurve<T> ret(s);
		
		C(s) ret[i] = left[i] + right[i];
		
		return ret;
	}
	
	/////////////////////////////////////////////////////////////	
	template<class T>
	inline BezierCurve<T> operator-(const BezierCurve<T> &left,const BezierCurve<T> &right)
	{
		auto s = std::min(left.size(),right.size());
		
		BezierCurve<T> ret(s);
		
		C(s) ret[i] = left[i] - right[i];
		
		return ret;
	}
}

#endif // FRONTIER_BEZIERCURVE_INL_INCLUDED
