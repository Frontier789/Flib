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
#ifndef FRONTIER_MATRIX_INL_INCLUDED
#define FRONTIER_MATRIX_INL_INCLUDED

#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/util/C.hpp>

#include <type_traits>
#include <cstdint>
#include <cmath>

namespace fm
{
	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T>::matrix()
	{
		reset();
	}
	
	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T>::matrix(const T &all)
	{
		reset(all);
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T>::matrix(const T (&data)[W*H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *(data+x*H+y);
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T>::matrix(const T (&data)[W][H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *((const T*)data+x*H+y);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T>::matrix(fm::Delegate<T,vec2i> fun)
	{
		Cx(W)Cy(H)
			m_data[x][y] = fun(vec2i(x,y));
	}
	
	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T>::matrix(const matrix<W,H,T> &mat)
	{
		Cx(W)Cy(H)
			m_data[x][y] = mat[x][y];
	}

	/////////////////////////////////////////////////////////////
	template<Size W, Size H, class T>
	template<class... ArgTypes,class>
	inline matrix<W, H, T>::matrix(ArgTypes... args) : 
		m_data{T(args)...}
	{

	}

	/////////////////////////////////////////////////////////////
	template<Size W, Size H, class T>
	template<class... ArgTypes,class,class>
	inline matrix<W, H, T>::matrix(ArgTypes... args) : 
		m_data{T(args.x)...,T(args.y)...}
	{

	}

	/////////////////////////////////////////////////////////////
	template<Size W, Size H, class T>
	template<class... ArgTypes,class,class,class>
	inline matrix<W, H, T>::matrix(ArgTypes... args) : 
		m_data{T(args.x)...,T(args.y)...,T(args.z)...}
	{

	}

	/////////////////////////////////////////////////////////////
	template<Size W, Size H, class T>
	template<class... ArgTypes,class,class,class,class>
	inline matrix<W, H, T>::matrix(ArgTypes... args) : 
		m_data{T(args.x)...,T(args.y)...,T(args.z)...,T(args.w)...}
	{

	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<class K,class>
	inline matrix<W,H,T>::operator K() const
	{
		return m_data[0][0];
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<class K,class>
	inline matrix<W,H,T>::operator vector2<K>() const
	{
		return vector2<K>(m_data[0][0],m_data[1][0]);
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<class K,class>
	inline matrix<W,H,T>::operator vector3<K>() const
	{
		return vector3<K>(m_data[0][0],m_data[1][0],m_data[2][0]);
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<class K,class>
	inline matrix<W,H,T>::operator vector4<K>() const
	{
		return vector4<K>(m_data[0][0],m_data[1][0],m_data[2][0],m_data[3][0]);
	}
	
	/// functions /////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline T matrix<W,H,T>::at(Size x,Size y) const
	{
		return (x<W && y<H && x>=0 && y>=0) ? m_data[x][y] : T();
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::set(Size x,Size y,const T &value)
	{
		if (x<W && y<H && x>=0 && y>=0)
			m_data[x][y] = value;
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset()
	{
		Cx(W)Cy(H)
			m_data[x][y] = T(int(x == y));
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset(const T &all)
	{
		Cx(W)Cy(H)
			m_data[x][y] = all;
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset(const T &diag,const T &rest)
	{
		Cx(W)Cy(H)
		{
			if (x == y)
				m_data[x][y] = diag;
			else
				m_data[x][y] = rest;
		}
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<H,W,T> matrix<W,H,T>::transpose() const
	{
		matrix<H,W,T> ret;
		Cx(W)Cy(H)
			ret[y][x] = m_data[x][y];
			
		return ret;
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<H,W,T> matrix<W,H,T>::t() const
	{
		return transpose();
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::byComp(const matrix<W,H,T> &mat) const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = m_data[x][y] * mat[x][y];
			
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<Size H2>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::preMul(const matrix<H,H2,T> &mat)
	{
		return (*this) = mat * (*this);
	}


	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<Size W2,Size H2,class T2>
	inline matrix<W2,H2,T2> matrix<W,H,T>::convert() const
	{
		matrix<W2,H2,T2> ret;
		Cx(W)Cy(H)
			ret[x][y] = (x<W2 && y<H2) ? T2(m_data[x][y]) : T2(int(x == y));
			
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	template<Size W2,Size H2,Size X,Size Y>
	inline matrix<W2,H2,T> matrix<W,H,T>::slice() const
	{
		matrix<W2,H2,T> ret;
		Cx(W2)Cy(H2)
			ret[x][y] = m_data[x+X][y+Y];
			
		return ret;
	}
	
	namespace priv
	{
		template<Size N,class T>
		struct getDet
		{
			static T getDeterminant(const matrix<N,N,T> &mat)
			{
				if (mat[0][0] == T())
				{
					Size mxp = 0;

					C(N)
						if (std::abs(mat[i][0]) > std::abs(mat[mxp][0]))
							mxp = i;

					if (mxp)
					{
						matrix<N,N,T> cpy;
						Cx(N)Cy(N)
							cpy[ (x==0 ? mxp : (x==mxp ? 0 : x))  ][y] = mat[x][y];

						return -getDeterminant(cpy);
					}

					return T();
				}

				T sum = T();

				fm::matrix<N,N,T> U(T(1));
				fm::matrix<N,N,T> L;

				Cx(N)
				{
					for (Size y = x;y<N;y++)
					{
						sum = T();
						for (Size k = 0;k<x;k++)
							sum += L[y][k] * U[k][x];

						L[y][x] = mat[y][x] - sum;
					}


					for (Size y = x;y<N;y++)
					{
						sum = T();
						for (Size k = 0;k<x;k++)
							sum += L[x][k] * U[k][y];

						if (L[x][x] == T())
							return T();

						U[x][y] = (mat[x][y] - sum) / L[x][x];
					}
				}

				T det = T(1);

				Cx(N)
					det *= U[x][x] * L[x][x];

				return det;
			}
		};
		
		template<class U>
		struct getDet<0,U>
		{
			static U getDeterminant(const matrix<0,0,U> &mat)
			{
				(void)mat;
				return U(1);
			}
		};
		
		template<class U>
		struct getDet<1,U>
		{
			static U getDeterminant(const matrix<1,1,U> &mat)
			{
				return mat[0][0];
			}
		};
		
		template<class U>
		struct getDet<2,U>
		{
			static U getDeterminant(const matrix<2,2,U> &mat)
			{
				return mat[0][0]*mat[1][1] - mat[1][0]*mat[0][1];
			}
		};
		
		template<class U>
		struct getDet<3,U>
		{
			static U getDeterminant(const matrix<3,3,U> &mat)
			{
				return mat[0][0]*(mat[1][1]*mat[2][2] - mat[2][1]*mat[1][2]) + 
				       mat[0][2]*(mat[1][0]*mat[2][1] - mat[2][0]*mat[1][1]) + 
					   mat[0][1]*(mat[2][0]*mat[1][2] - mat[1][0]*mat[2][2]);
			}
		};
		
		template<Size N,class T>
		struct InverseCounter {
			static matrix<N,N,T> get(const matrix<N,N,T> &mat) {
				return mat.adjugate() / mat.det();
			}
		};
		
		template<class T>
		struct InverseCounter<1,T> {
			static matrix<1,1,T> get(const matrix<1,1,T> &mat) {
				return T(1) / mat[0][0];
			}
		};
		
		template<class T>
		struct InverseCounter<2,T> {
			static matrix<2,2,T> get(const matrix<2,2,T> &mat) {
				return matrix<2,2,T>(mat[1][1],-mat[0][1],-mat[1][0],mat[0][0]) / mat.det();
			}
		};
		
		template<class T>
		struct InverseCounter<3,T> {
			static matrix<3,3,T> get(const matrix<3,3,T> &m) {
				T cache1 = m[1][1]*m[2][2] - m[2][1]*m[1][2];
				T cache2 = m[1][0]*m[2][1] - m[2][0]*m[1][1];
				T cache3 = m[2][0]*m[1][2] - m[1][0]*m[2][2];
				
				T det = m[0][0]*cache1 + 
				        m[0][2]*cache2 + 
					    m[0][1]*cache3;
				
				matrix<3,3,T> minv;
				minv[0][0] = cache1;
				minv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]);
				minv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
				minv[1][0] = cache3;
				minv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
				minv[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]);
				minv[2][0] = cache2;
				minv[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]);
				minv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
				
				return minv / det;
			}
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline T matrix<W,H,T>::det() const
	{
		return priv::getDet<W,T>::getDeterminant(*this);
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::minors() const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
		{
			matrix<W-1u,H-1u,T> mx;
			
			for (Size dx=0;dx<W;dx++)
				for (Size dy=0;dy<H;dy++)
					if (dx!=x && dy!=y)
					{
						Size px = dx;
						Size py = dy;
						
						if (px > x) px--;
						if (py > y) py--;
						
						mx[px][py] = m_data[dx][dy];
					}
					
			ret[x][y] = mx.det();
		}
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::adjugate() const
	{
		matrix<W,H,T> cofactors = minors();
		Cx(W)Cy(H)
			cofactors[x][y] = cofactors[x][y] * T((x+y)%2 ? -1.f : 1.f);
			
		return cofactors.transpose();
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,W,T> matrix<W,H,T>::inverse() const
	{
		return priv::InverseCounter<W,T>::get(*this);
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline T matrix<W,H,T>::trace() const
	{
		T ret = T();
		C(W>H ? H : W)
			ret = ret + m_data[i][i];
		
		return ret;
	}

	/// operators /////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline std::array<T,H> &matrix<W,H,T>::operator[](Size index)
	{
		return m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline const std::array<T,H> &matrix<W,H,T>::operator[](Size index) const
	{
		return m_data[index];
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T> &matrix<W,H,T>::operator()(fm::Delegate<T,T,vec2i> fun)
	{
		return apply(fun);
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline matrix<W,H,T> &matrix<W,H,T>::apply(fm::Delegate<T,T,vec2i> fun)
	{
		Cx(W)Cy(H)
			m_data[x][y] = fun(m_data[x][y],vec2i(x,y));
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline T matrix<W,H,T>::fold(fm::Delegate<T,T,T> ffun,T init) const
	{
		Cx(W)Cy(H)
			init = ffun(init,m_data[x][y]);
		
		return init;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	matrix<W,H,T> matrix<W,H,T>::identity;


	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline auto operator*(const T &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left*right[0][0])>
	{
		matrix<W,H,decltype(left*right[0][0])> ret;
		
		Cx(W)Cy(H)
			ret[x][y] = left * right[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline auto operator*(const matrix<W,H,T> &left,const T2 &right) -> matrix<W,H,decltype(left[0][0]*right)>
	{
		matrix<W,H,decltype(left[0][0]*right)> ret;
		
		Cx(W)Cy(H)
			ret[x][y] = left[x][y] * right;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline auto operator/(const matrix<W,H,T> &left,const T2 &right) -> matrix<W,H,decltype(left[0][0]/right)>
	{
		matrix<W,H,decltype(left[0][0]/right)> ret;
		
		Cx(W)Cy(H)
			ret[x][y] = left[x][y] / right;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,Size H2,class T2>
	inline auto operator*(const matrix<W,H,T> &left,const matrix<H,H2,T2> &right) -> matrix<W,H2,decltype(left[0][0]*right[0][0])>
	{
		typedef decltype(left[0][0]*right[0][0]) TR;
		
		matrix<W,H2,TR> ret = matrix<W,H2,TR>(TR());
		
		Cx(W)
			Cy(H2)
				C(H)
					ret[x][y] += left[x][i] * right[i][y];

		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline auto operator+(const matrix<W,H,T> &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left[0][0]+right[0][0])>
	{
		matrix<W,H,decltype(left[0][0]+right[0][0])> ret = left;
		
		Cx(W)Cy(H)
			ret[x][y] = ret[x][y] + right[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline auto operator-(const matrix<W,H,T> &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left[0][0]-right[0][0])>
	{
		matrix<W,H,decltype(left[0][0]-right[0][0])> ret = left;
		
		Cx(W)Cy(H)
			ret[x][y] = ret[x][y] - right[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T>
	inline auto operator-(const matrix<W,H,T> &mat) -> matrix<W,H,decltype(-mat[0][0])>
	{
		matrix<W,H,decltype(-mat[0][0])> ret = mat;
		
		Cx(W)Cy(H)
			ret[x][y] = -ret[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline matrix<W,H,T> &operator+=(matrix<W,H,T> &left,const matrix<W,H,T2> &right)
	{
		return left = left + right;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline matrix<W,H,T> &operator-=(matrix<W,H,T> &left,const matrix<W,H,T2> &right)
	{
		return left = left - right;
	}

	/////////////////////////////////////////////////////////////
	template<Size W,Size H,class T,class T2>
	inline matrix<W,H,T> &operator*=(matrix<W,H,T> &left,const matrix<H,H,T2> &right)
	{
		return left = left * right;
	}

	/////////////////////////////////////////////////////////////
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class,class,class>
	inline bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		Cx(W)Cy(H)
			if (left[x][y] != right[x][y])
				return false;
		
		return true;
	}

	/////////////////////////////////////////////////////////////
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class,class>
	inline bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		(void)left;
		(void)right;
		return false;
	}

	/////////////////////////////////////////////////////////////
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class,class,class>
	inline bool operator!=(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		Cx(W)Cy(H)
			if (left[x][y] == right[x][y])
				return false;
		
		return true;
	}

	/////////////////////////////////////////////////////////////
	template<Size W ,Size H ,class T,
             Size W2,Size H2,class T2,class,class>
	inline bool operator!=(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		(void)left;
		(void)right;
		return false;
	}
	
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const matrix<2,2,T> &mat,const vector2<T2> &vec) -> vector2<decltype(mat[0][0]*vec[0])>
	{
		return vector2<decltype(mat[0][0]*vec[0])>(mat[0][0]*vec.x + mat[0][1]*vec.y,
												   mat[1][0]*vec.x + mat[1][1]*vec.y);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const matrix<3,3,T> &mat,const vector3<T2> &vec) -> vector3<decltype(mat[0][0]*vec[0])>
	{
		return vector3<decltype(mat[0][0]*vec[0])>(mat[0][0]*vec.x + mat[0][1]*vec.y + mat[0][2]*vec.z,
												   mat[1][0]*vec.x + mat[1][1]*vec.y + mat[1][2]*vec.z,
												   mat[2][0]*vec.x + mat[2][1]*vec.y + mat[2][2]*vec.z);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const matrix<4,4,T> &mat,const vector4<T2> &vec) -> vector4<decltype(mat[0][0]*vec[0])>
	{
		return vector4<decltype(mat[0][0]*vec[0])>(mat[0][0]*vec.x + mat[0][1]*vec.y + mat[0][2]*vec.z + mat[0][3]*vec.w,
												   mat[1][0]*vec.x + mat[1][1]*vec.y + mat[1][2]*vec.z + mat[1][3]*vec.w,
												   mat[2][0]*vec.x + mat[2][1]*vec.y + mat[2][2]*vec.z + mat[2][3]*vec.w,
												   mat[3][0]*vec.x + mat[3][1]*vec.y + mat[3][2]*vec.z + mat[3][3]*vec.w);
	}
	
	
	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector2<T2> &vec,const matrix<2,2,T> &mat) -> vector2<decltype(vec[0]*mat[0][0])>
	{
		return vector2<decltype(mat[0][0]*vec[0])>(mat[0][0]*vec.x + mat[0][1]*vec.x,
												   mat[1][0]*vec.y + mat[1][1]*vec.y);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector3<T2> &vec,const matrix<3,3,T> &mat) -> vector3<decltype(vec[0]*mat[0][0])>
	{
		return vector3<decltype(mat[0][0]*vec[0])>(mat[0][0]*vec.x + mat[0][1]*vec.x + mat[0][2]*vec.x,
												   mat[1][0]*vec.y + mat[1][1]*vec.y + mat[1][2]*vec.y,
												   mat[2][0]*vec.z + mat[2][1]*vec.z + mat[2][2]*vec.z);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class T2>
	auto operator*(const vector4<T2> &vec,const matrix<4,4,T> &mat) -> vector4<decltype(vec[0]*mat[0][0])>
	{
		return vector4<decltype(mat[0][0]*vec[0])>(mat[0][0]*vec.x + mat[0][1]*vec.x + mat[0][2]*vec.x + mat[0][3]*vec.x,
												   mat[1][0]*vec.y + mat[1][1]*vec.y + mat[1][2]*vec.y + mat[1][3]*vec.y,
												   mat[2][0]*vec.z + mat[2][1]*vec.z + mat[2][2]*vec.z + mat[2][3]*vec.z,
												   mat[3][0]*vec.w + mat[3][1]*vec.w + mat[3][2]*vec.w + mat[3][3]*vec.w);
	}
	
	/////////////////////////////////////////////////////////////
	template<Size W,Size H,Size H2,class T>
	inline matrix<W,H+H2,T> operator|(const matrix<W,H,T> &m1,const matrix<W,H2,T> &m2)
	{
		return MATRIX::concatH(m1,m2);
	}
	

	/// square matrix methods /////////////////////////////////////////////////////////
	namespace MATRIX
	{
		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(float x,float y,float z)
		{
			return matrix<4, 4, float>(1,0,0,x,
						               0,1,0,y,
						               0,0,1,z,
						               0,0,0,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(vec3 translate)
		{
			return translation(translate.x,translate.y,translate.z);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(float x,float y)
		{
			return translation(x,y,0);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(float x,float y,float z)
		{
			return matrix<4,4,float>(x,0,0,0,
									 0,y,0,0,
									 0,0,z,0,
									 0,0,0,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(vec2 scale)
		{
			return scaling(scale.x,scale.y,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(vec3 scale)
		{
			return scaling(scale.x,scale.y,scale.z);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(float x,float y)
		{
			return scaling(x,y,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> rotation(const Anglef &angle)
		{
			return matrix<4,4,float>(std::cos(angle),-std::sin(angle),0,0,
									 std::sin(angle), std::cos(angle),0,0,
											       0,               0,1,0,
												   0,               0,0,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> perspective(const Anglef &fieldOfView,float aspect,float Znear,float Zfar)
		{
			return matrix<4,4,float>(1.f/std::tan(fieldOfView/2.f)/aspect,                    0,                        0,                          0,
																0,1.f/std::tan(fieldOfView/2.f),                        0,                          0,
																0,                            0,(Zfar+Znear)/(Znear-Zfar),(2*Znear*Zfar)/(Znear-Zfar),
																0,                            0,                       -1,                          0);
		}

		/////////////////////////////////////////////////////////////
		inline matrix<4,4,float> ortho(float left,float bottom,float right,float top,float nearVal,float farVal)
		{
			return matrix<4,4,float>(2.f/(right-left),               0,                   0,        (right+left)/(left-right),
													0,2.f/(top-bottom),                   0,        (top+bottom)/(bottom-top),
													0,               0,2.f/(farVal-nearVal),(farVal+nearVal)/(nearVal-farVal),
													0,               0,                   0,                                1);
		}


		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> lookAt(vec3 cam_pos,vec3 target_pos,vec3 up_dir)
		{
			mat4 t = translation(-cam_pos);
			vec3 f = (target_pos - cam_pos).sgn();
			vec3 r = f.cross(up_dir).sgn();
			vec3 u = r.cross(f).sgn();
			
			return matrix<4,4,float>( r.x,  r.y,  r.z,  0,
									  u.x,  u.y,  u.z,  0,
									 -f.x, -f.y, -f.z,  0,
										0,    0,    0,  1)*t;
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> lookAt(vec3 cam_pos,vec3 target_pos)
		{
			return lookAt(cam_pos,target_pos,vec3(0,1,0));
		}
		
		/////////////////////////////////////////////////////////////
		template<Size W,Size H,Size H2,class T>
		inline matrix<W,H+H2,T> concatH(const matrix<W,H,T> &m1,const matrix<W,H2,T> &m2)
		{
			matrix<W,H+H2> ret;
			Cx(W)Cy(H)
				ret[x][y] = m1[x][y];
			Cx(W)Cy(H2)
				ret[x][H+y] = m2[x][y];
			
			return ret;
		}
		
		/////////////////////////////////////////////////////////////
		template<Size W,Size W2,Size H,class T>
		inline matrix<W+W2,H,T> concatW(const matrix<W,H,T> &m1,const matrix<W2,H,T> &m2)
		{
			matrix<W+W2,H> ret;
			Cx(W)Cy(H)
				ret[x][y] = m1[x][y];
			Cx(W2)Cy(H)
				ret[W+x][y] = m2[x][y];
			
			return ret;
		}
	}
}

#include <iosfwd>

////////////////////////////////////////////////////////////
template<fm::Size W,fm::Size H,class T,class CharT,class CharTraitT>
inline std::basic_ostream<CharT,CharTraitT> &operator<<(std::basic_ostream<CharT,CharTraitT> &out, const fm::matrix<W,H,T> &mat)
{
	Cx(W)
	{
		Cy(H)
            if (y+1==H && x+1<W)
                out<<mat[x][y]<<'\n';
			else
                out<<mat[x][y]<<' ';
	}
	return out;
}

////////////////////////////////////////////////////////////
template<fm::Size W,fm::Size H,class T,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::matrix<W,H,T> &mat)
{
	Cx(W) Cy(H)
		in>>mat[x][y];
	return in;
}

#endif
