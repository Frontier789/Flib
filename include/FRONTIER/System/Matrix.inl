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
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T>::matrix()
	{
		reset();
	}
	
	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T>::matrix(const T &all)
	{
		reset(all);
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T>::matrix(const T (&data)[W*H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *(data+x*H+y);
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T>::matrix(const T (&data)[W][H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *((const T*)data+x*H+y);
	}
	
	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T>::matrix(const T &diagonal,const T &rest)
	{
		reset(diagonal,rest);
	}
	
	/// constructors /////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T>::matrix(const matrix<W,H,T> &mat)
	{
		Cx(W)Cy(H)
			m_data[x][y] = mat[x][y];
	}


	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline matrix<W,H,T>::matrix(const T &a00,const T &a01,
								 const T &a10,const T &a11) : m_data{{a00,a01},
																	 {a10,a11}}
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline matrix<W,H,T>::matrix(const T &a00,const T &a01,const T &a02,
								 const T &a10,const T &a11,const T &a12,
								 const T &a20,const T &a21,const T &a22) : m_data{{a00,a01,a02},
																				  {a10,a11,a12},
																				  {a20,a21,a22}}
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline matrix<W,H,T>::matrix(const T &a00,const T &a01,const T &a02,const T &a03,
								 const T &a10,const T &a11,const T &a12,const T &a13,
								 const T &a20,const T &a21,const T &a22,const T &a23,
								 const T &a30,const T &a31,const T &a32,const T &a33) : m_data{{a00,a01,a02,a03},
																							   {a10,a11,a12,a13},
																							   {a20,a21,a22,a23},
																							   {a30,a31,a32,a33}}
	{
		
	}

	/// functions /////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline T matrix<W,H,T>::at(size_t x,size_t y) const
	{
		return (x<W && y<H && x>=0 && y>=0) ? m_data[x][y] : T();
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::set(size_t x,size_t y,const T &value)
	{
		if (x<W && y<H && x>=0 && y>=0)
			m_data[x][y] = value;
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset()
	{
		Cx(W)Cy(H)
			m_data[x][y] = T(int(x == y));
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset(const T &all)
	{
		Cx(W)Cy(H)
			m_data[x][y] = all;
			
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
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
	template<size_t W,size_t H,class T>
	inline matrix<H,W,T> matrix<W,H,T>::transpose() const
	{
		matrix<H,W,T> ret;
		Cx(W)Cy(H)
			ret[y][x] = m_data[x][y];
			
		return ret;
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::byComp(const matrix<W,H,T> &mat) const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = m_data[x][y] * mat[x][y];
			
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<size_t H2>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::preMul(const matrix<H,H2,T> &mat)
	{
		return (*this) = mat * (*this);
	}


	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<size_t W2,size_t H2,class T2>
	inline matrix<W2,H2,T2> matrix<W,H,T>::convert() const
	{
		matrix<W2,H2,T2> ret;
		Cx(W)Cy(H)
			ret[x][y] = (x<W2 && y<H2) ? T2(m_data[x][y]) : T2(int(x == y));
			
		return ret;
	}
	
	namespace priv
	{
		template<size_t N,class T>
		class getDet
		{
		public:
			static T getDeterminant(const matrix<N,N,T> &mat)
			{
				if (mat[0][0] == T())
				{
					size_t mxp = 0;

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
					for (size_t y = x;y<N;y++)
					{
						sum = T();
						for (size_t k = 0;k<x;k++)
							sum += L[y][k] * U[k][x];

						L[y][x] = mat[y][x] - sum;
					}


					for (size_t y = x;y<N;y++)
					{
						sum = T();
						for (size_t k = 0;k<x;k++)
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
		class getDet<0,U>
		{
		public:
			static U getDeterminant(const matrix<0,0,U> &mat)
			{
				(void)mat;
				return U(1);
			}
		};
		
		template<class U>
		class getDet<1,U>
		{
		public:
			static U getDeterminant(const matrix<1,1,U> &mat)
			{
				return mat[0][0];
			}
		};
		
		template<class U>
		class getDet<2,U>
		{
		public:
			static U getDeterminant(const matrix<2,2,U> &mat)
			{
				return mat[0][0]*mat[1][1] - mat[1][0]*mat[0][1];
			}
		};
		
		template<class U>
		class getDet<3,U>
		{
		public:
			static U getDeterminant(const matrix<3,3,U> &mat)
			{
				return mat[0][0]*mat[1][1]*mat[2][2] + mat[1][0]*mat[2][1]*mat[0][2] + mat[2][0]*mat[0][1]*mat[1][2] -
					   mat[2][0]*mat[1][1]*mat[0][2] - mat[1][0]*mat[0][1]*mat[2][2] - mat[0][0]*mat[2][1]*mat[1][2];
			}
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline T matrix<W,H,T>::det() const
	{
		return priv::getDet<W,T>::getDeterminant(*this);
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline matrix<W,H,T> matrix<W,H,T>::minors() const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
		{
			matrix<W-1u,H-1u,T> mx;
			
			for (size_t dx=0;dx<W;dx++)
				for (size_t dy=0;dy<H;dy++)
					if (dx!=x && dy!=y)
					{
						size_t px = dx;
						size_t py = dy;
						
						if (px > x) px--;
						if (py > y) py--;
						
						mx[px][py] = m_data[dx][dy];
					}
					
			ret[x][y] = mx.det();
		}
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline matrix<W,H,T> matrix<W,H,T>::adjugate() const
	{
		matrix<W,H,T> cofactors = minors();
		Cx(W)Cy(H)
			cofactors[x][y] = cofactors[x][y] * T((x+y)%2 ? -1.f : 1.f);
			
		return cofactors.transpose();
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline matrix<W,H,T> matrix<W,H,T>::inverse() const
	{
		return adjugate()/det();
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline T matrix<W,H,T>::trace() const
	{
		T ret = T();
		C(W)
			ret = ret + m_data[i][i];
		
		return ret;
	}

	/// operators /////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline T *matrix<W,H,T>::operator[](size_t index)
	{
		return m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline const T *matrix<W,H,T>::operator[](size_t index) const
	{
		return m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T (&data)[W][H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *((const T*)data+x*H+y);
		return *this;
	}

	////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T (&data)[W*H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *(data+x*H+y);
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T &a00,const T &a01,
																	   const T &a10,const T &a11)
	{
		return *this = matrix<W,H,T>(a00,a01,
									 a10,a11);
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T &a00,const T &a01,const T &a02,
																	   const T &a10,const T &a11,const T &a12,
																	   const T &a20,const T &a21,const T &a22)
	{
		return *this = matrix<W,H,T>(a00,a01,a02,
									 a10,a11,a12,
									 a20,a21,a22);
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	template<class,class>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T &a00,const T &a01,const T &a02,const T &a03,
																	   const T &a10,const T &a11,const T &a12,const T &a13,
																	   const T &a20,const T &a21,const T &a22,const T &a23,
																	   const T &a30,const T &a31,const T &a32,const T &a33)
	{
		return *this = matrix<W,H,T>(a00,a01,a02,a03,
									 a10,a11,a12,a13,
									 a20,a21,a22,a23,
									 a30,a31,a32,a33);
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	matrix<W,H,T> matrix<W,H,T>::identity;


	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline auto operator*(const T &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left*right[0][0])>
	{
		matrix<W,H,decltype(left*right[0][0])> ret;
		
		Cx(W)Cy(H)
			ret[x][y] = left * right[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline auto operator*(const matrix<W,H,T> &left,const T2 &right) -> matrix<W,H,decltype(left[0][0]*right)>
	{
		matrix<W,H,decltype(left[0][0]*right)> ret;
		
		Cx(W)Cy(H)
			ret[x][y] = left[x][y] * right;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline auto operator/(const matrix<W,H,T> &left,const T2 &right) -> matrix<W,H,decltype(left[0][0]/right)>
	{
		matrix<W,H,decltype(left[0][0]/right)> ret;
		
		Cx(W)Cy(H)
			ret[x][y] = left[x][y] / right;
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,size_t H2,class T2>
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
	template<size_t W,size_t H,class T,class T2>
	inline auto operator+(const matrix<W,H,T> &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left[0][0]+right[0][0])>
	{
		matrix<W,H,decltype(left[0][0]+right[0][0])> ret = left;
		
		Cx(W)Cy(H)
			ret[x][y] = ret[x][y] + right[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline auto operator-(const matrix<W,H,T> &left,const matrix<W,H,T2> &right) -> matrix<W,H,decltype(left[0][0]-right[0][0])>
	{
		matrix<W,H,decltype(left[0][0]-right[0][0])> ret = left;
		
		Cx(W)Cy(H)
			ret[x][y] = ret[x][y] - right[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T>
	inline auto operator-(const matrix<W,H,T> &mat) -> matrix<W,H,decltype(-mat[0][0])>
	{
		matrix<W,H,decltype(-mat[0][0])> ret = mat;
		
		Cx(W)Cy(H)
			ret[x][y] = -ret[x][y];
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline matrix<W,H,T> &operator+=(matrix<W,H,T> &left,const matrix<W,H,T2> &right)
	{
		return left = left + right;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline matrix<W,H,T> &operator-=(matrix<W,H,T> &left,const matrix<W,H,T2> &right)
	{
		return left = left - right;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W,size_t H,class T,class T2>
	inline matrix<W,H,T> &operator*=(matrix<W,H,T> &left,const matrix<H,H,T2> &right)
	{
		return left = left * right;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W ,size_t H ,class T,
             size_t W2,size_t H2,class T2,class,class,class>
	inline bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		Cx(W)Cy(H)
			if (left[x][y] != right[x][y])
				return false;
		
		return true;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W ,size_t H ,class T,
             size_t W2,size_t H2,class T2,class,class>
	inline bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		(void)left;
		(void)right;
		return false;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W ,size_t H ,class T,
             size_t W2,size_t H2,class T2,class,class,class>
	inline bool operator!=(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		Cx(W)Cy(H)
			if (left[x][y] == right[x][y])
				return false;
		
		return true;
	}

	/////////////////////////////////////////////////////////////
	template<size_t W ,size_t H ,class T,
             size_t W2,size_t H2,class T2,class,class>
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
	

	/// square matrix methods /////////////////////////////////////////////////////////
	namespace MATRIX
	{
		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(float x,float y,float z)
		{
			return matrix<4,4,float>(1,0,0,x,
									 0,1,0,y,
									 0,0,1,z,
									 0,0,0,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(const vec3 &translate)
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
		inline matrix<4,4,float> scaling(const vec2 &scale)
		{
			return scaling(scale.x,scale.y,1);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(const vec3 &scale)
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
		inline matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,const vec3 &up_dir)
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
		inline matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos)
		{
			return lookAt(cam_pos,target_pos,vec3(0,1,0));
		}
	}
}

#include <iosfwd>

////////////////////////////////////////////////////////////
template<size_t W,size_t H,class T,class CharT,class CharTraitT>
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
template<size_t W,size_t H,class T,class CharT,class CharTraitT>
inline std::basic_istream<CharT,CharTraitT> &operator>>(std::basic_istream<CharT,CharTraitT> &in, fm::matrix<W,H,T> &mat)
{
	Cx(W) Cy(H)
		in>>mat[x][y];
	return in;
}

#endif
