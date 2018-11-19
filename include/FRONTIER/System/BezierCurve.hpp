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
#ifndef FRONTIER_BEZIERCURVE_HPP_INCLUDED
#define FRONTIER_BEZIERCURVE_HPP_INCLUDED
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#define FRONTIER_BEZIERCURVE
#include <vector>

namespace fm
{
	template<class T>
	struct BezierCurve;
	
	namespace priv
	{
		template<class T>
		T eval_bezier(const BezierCurve<T> &curve,double t) {return curve.eval(t);}
		
		template<class T>
		T eval_bezier(const BezierCurve<BezierCurve<T>> &surface,fm::vec2d t) {
			return surface.eval(t.x).eval(t.y);
		}
		
		template<class T,class U>
		void eval_bezier(const T &,const U &) {}
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold and evaluate bezier curves and surface
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	struct BezierCurve
	{
		typedef typename std::vector<T>::iterator iterator; ///< Iterator for the bezier class
		typedef typename std::vector<T>::const_iterator const_iterator; ///< Const iterator for the bezier class
		
		/////////////////////////////////////////////////////////////
		/// @brief Access the control points of the curve
		///
		/// @param index The index of the control point requested
		///
		/// @return Reference to the control point
		///
		/////////////////////////////////////////////////////////////
		T &operator[](fm::Size index);
		
		/////////////////////////////////////////////////////////////
		/// @brief Access the control points of the curve
		///
		/// @param index The index of the control point requested
		///
		/// @return Reference to the control point
		///
		/////////////////////////////////////////////////////////////
		const T &operator[](fm::Size index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the surface at a given 2d point
		///
		/// This function has a return type of void if the curve is not a surface (T is not BezierCurve) 
		///
		/// @param p The position to evaluate at from range [0,1]x[0,1]
		///
		/// @return The point on the surface
		///
		/////////////////////////////////////////////////////////////
		auto eval(fm::vec2d p) const -> decltype(fm::priv::eval_bezier(*this,p)) {return fm::priv::eval_bezier(*this,p);}
		
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the curve at a given point
		///
		/// @param t The position to evaluate at from range [0,1]
		///
		/// @return The point on the curve
		///
		/////////////////////////////////////////////////////////////
		T eval(double t) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Query the number of control points of the curve
		///
		/// @return The number of control points
		///
		/////////////////////////////////////////////////////////////
		fm::Size size() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the number of control points
		///
		/// @param size The number of control points
		///
		/////////////////////////////////////////////////////////////
		void resize(fm::Size size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a curve with given number of control points
		///
		/// @param n The number of control points
		///
		/////////////////////////////////////////////////////////////
		BezierCurve(fm::Size n = 4);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a curve with given control points
		///
		/// @param n The number of control points
		/// @param val The default value of the control points
		///
		/////////////////////////////////////////////////////////////
		BezierCurve(fm::Size n,T val);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get an iterator to the first control point
		///
		/// @return Iterator to the first control point
		///
		/////////////////////////////////////////////////////////////
		iterator begin() {return m_controlPts.begin();}
		
		/////////////////////////////////////////////////////////////
		/// @brief Get an iterator to the first control point
		///
		/// @return Iterator to the first control point
		///
		/////////////////////////////////////////////////////////////
		const_iterator begin() const {return m_controlPts.begin();}
		
		/////////////////////////////////////////////////////////////
		/// @brief Get an iterator pointing right after the last control point
		///
		/// @return Iterator pointing right after the last control point
		///
		/////////////////////////////////////////////////////////////
		iterator end() {return m_controlPts.end();}
		
		/////////////////////////////////////////////////////////////
		/// @brief Get an iterator pointing right after the last control point
		///
		/// @return Iterator pointing right after the last control point
		///
		/////////////////////////////////////////////////////////////
		const_iterator end() const {return m_controlPts.end();}
		
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the curve at a given point
		///
		/// @param t The position to evaluate at from range [0,1]
		///
		/// @return The point on the curve
		///
		/////////////////////////////////////////////////////////////
		T operator()(double t) const {return eval(t);}
		
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the surface at a given 2d point
		///
		/// This function has a return type of void if the curve is not a surface (T is not BezierCurve) 
		///
		/// @param p The position to evaluate at from range [0,1]x[0,1]
		///
		/// @return The point on the surface
		///
		/////////////////////////////////////////////////////////////
		auto operator()(fm::vec2d t) const -> decltype(eval(fm::vec2d())) const {return eval(t);}
		
	private:
		std::vector<T> m_controlPts; ///< The control points of the curve
		
		T deCasteljou(double t) const; ///< Calculate a point on the curve using the de casteljou algorithm
	};
	
	template<class T>
	using BezierSurface = BezierCurve<BezierCurve<T>>;
}

#endif // FRONTIER_BEZIERCURVE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/BezierCurve.inl>
#endif
