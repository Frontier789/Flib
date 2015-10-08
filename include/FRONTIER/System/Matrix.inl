////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/Collector.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <cmath>
namespace fm
{
	/// constructors /////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T>::matrix(const matrix<W,H,T> &mat)
	{
		Cx(W)Cy(H)
			m_data[x][y] = mat[x][y];
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T>::matrix(const T (&data)[W][H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *((const T*)data+x*H+y);
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T>::matrix(const T (&data)[W*H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *(data+x*H+y);
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T>::matrix(const T &all)
	{
		reset(all);
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T>::matrix()
	{
		reset();
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	template<class T2,class Container>
	inline matrix<W,H,T>::matrix(const fm::Collector<T2,Container> &col)
	{
		reset(T());

		fm::Size count = col.size();
		typename Container::const_iterator it = col.begin();

		C(W*H < count ? W*H : count)
			m_data[i/H][i%H] = *it,
			++it;
	}


	/// functions /////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline T matrix<W,H,T>::at(fm::Size x,fm::Size y) const
	{
		return (x<W && y<H && x>=0 && y>=0) ? m_data[x][y] : T();
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::set(fm::Size x,fm::Size y,const T &value)
	{
		if (x<W && y<H && x>=0 && y>=0)
			m_data[x][y]=value;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset()
	{
		Cx(W)Cy(H)
			m_data[x][y] = T(x == y);
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::reset(T all)
	{
		Cx(W)Cy(H)
			m_data[x][y] = all;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<H,W,T> matrix<W,H,T>::transpose() const
	{
		matrix<H,W,T> ret;
		Cx(W)Cy(H)
			ret[y][x] = m_data[x][y];
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::byComp(const matrix<W,H,T> &mat) const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = m_data[x][y] * mat[x][y];
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	template<fm::Size W2,fm::Size H2,class T2>
	inline matrix<W2,H2,T2> matrix<W,H,T>::convert() const
	{
		matrix<W2,H2,T2> ret;
		Cx(W)Cy(H)
			ret[x][y] = (x<W2 && y<H2) ? T2(m_data[x][y]) : T2(x == y);
		return ret;
	}


	/// operators /////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline T *matrix<W,H,T>::operator[](fm::Size index)
	{
		return m_data[index];
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline const T *matrix<W,H,T>::operator[](fm::Size index) const
	{
		return m_data[index];
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T (&data)[W][H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *((const T*)data+x*H+y);
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator()(const T (&data)[W*H])
	{
		Cx(W)Cy(H)
			m_data[x][y] = *(data+x*H+y);
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::operator*(const T &value) const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = m_data[x][y] * value;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> matrix<W,H,T>::operator/(const T &value) const
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = m_data[x][y] / value;
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator*=(const T &value)
	{
		Cx(W)Cy(H)
			m_data[x][y] *= value;
		return *this;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline typename matrix<W,H,T>::reference matrix<W,H,T>::operator/=(const T &value)
	{
		Cx(W)Cy(H)
			m_data[x][y] /= value;
		return *this;
	}




	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,fm::Size H2>
	inline matrix<W,H2,T> operator*(const matrix<W,H,T> &left,const matrix<H,H2,T> &right)
	{
		matrix<W,H2,T> ret = matrix<W,H2,T>(T());
		Cx(W)
			Cy(H2)
				C(H)
					ret[x][y] += left[x][i]*right[i][y];

		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> operator+(const matrix<W,H,T> &left,const matrix<W,H,T> &right)
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = left[x][y] + right[x][y];
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> operator-(const matrix<W,H,T> &left,const matrix<W,H,T> &right)
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = left[x][y] - right[x][y];
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> operator-(const matrix<W,H,T> &mat)
	{
		matrix<W,H,T> ret;
		Cx(W)Cy(H)
			ret[x][y] = mat[x][y] * T(-1);
		return ret;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> &operator+=(matrix<W,H,T> &left,const matrix<W,H,T> &right)
	{
		Cx(W)Cy(H)
			left[x][y] += right[x][y];
		return left;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> &operator-=(matrix<W,H,T> &left,const matrix<W,H,T> &right)
	{
		Cx(W)Cy(H)
			left[x][y] -= right[x][y];
		return left;
	}

	////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline matrix<W,H,T> &operator*=(matrix<W,H,T> &left,const matrix<H,H,T> &right)
	{
		left = left * right;
		return left;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W ,fm::Size H ,class T,
			 fm::Size W2,fm::Size H2,class T2>
	inline bool operator==(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		if (W!=W2 || H!=H2)
			return false;
		Cx(W)Cy(H)
			if (   left[x][y]  != T(right[x][y]) &&
				T2(left[x][y]) !=   right[x][y])
				return false;
		return true;
	}


	////////////////////////////////////////////////////////////
	template<fm::Size W ,fm::Size H ,class T,
			 fm::Size W2,fm::Size H2,class T2>
	inline bool operator!=(const matrix<W,H,T> &left,const matrix<W2,H2,T2> &right)
	{
		if (W!=W2 || H!=H2)
			return true;
		Cx(W)Cy(H)
			if (   left[x][y]  != T(right[x][y]) &&
				T2(left[x][y]) !=   right[x][y])
				return true;
		return false;
	}
	template<fm::Size W,fm::Size H,class T> matrix<W,H,T> matrix<W,H,T>::identity = matrix<W,H,T>();



	/// square matrix methods /////////////////////////////////////////////////////////
	namespace MATRIX
	{
		////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		inline T det(const matrix<N,N,T> &mat)
		{
			T sum = T();

			fm::matrix<N,N,T> U(T(1));
			fm::matrix<N,N,T> L;

			Cx(N)
			{
				for (fm::Size y = x;y<N;y++)
				{
					sum = T();
					for (fm::Size k = 0;k<x;k++)
						sum += L[y][k] * U[k][x];

					L[y][x] = mat[y][x] - sum;
				}


				for (fm::Size y = x;y<N;y++)
				{
					sum = T();
					for (fm::Size k = 0;k<x;k++)
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

		////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		inline matrix<N,N,T> minorMat(const matrix<N,N,T> &mat)
		{
			matrix<N,N,T> ret;
			Cx(N)Cy(N)
			{
				matrix<N-1u,N-1u,T> mx;
				int d=0;
				for (fm::Size dx=0;dx<N;dx++)
					for (fm::Size dy=0;dy<N;dy++)
						if (dx!=x && dy!=y)
							mx[d%(N-1)][int(d/(N-1))] = mat[dx][dy],d++;
				ret[x][y] = det<N-1>(mx);
			}
			return ret;
		}



		////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		inline matrix<N,N,T> adj(const matrix<N,N,T> &mat)
		{
			matrix<N,N,T> ret = minorMat(mat);
			Cx(N)Cy(N)
				ret[x][y]*=((x+y)%2 ? -1.f : 1.f);
			return ret.transpose();
		}


		////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		inline matrix<N,N,T> inverse(const matrix<N,N,T> &mat)
		{
			return adj(mat)/det(mat);
		}


		////////////////////////////////////////////////////////////
		template<fm::Size W,fm::Size H,class T>
		inline matrix<H,W,T> transpose(const matrix<W,H,T> &mat)
		{
			return mat.transpose();
		}


		////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		inline T trace(const matrix<N,N,T> &mat)
		{
			T ret = 0;
			C(N)
				ret+=mat[i][i];
			return ret;
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(float x,float y,float z,StorageOrder storeOrder)
		{
			if (storeOrder==RowMajor)
			{
				float ret[]={1,0,0,x,
							 0,1,0,y,
							 0,0,1,z,
							 0,0,0,1};

				return matrix<4,4,float>(ret);
			}
			float ret[]={1,0,0,0,
						 0,1,0,0,
						 0,0,1,0,
						 x,y,z,1};

			return matrix<4,4,float>(ret);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(const vec3 &translate,StorageOrder storeOrder)
		{
			return translation(translate.x,translate.y,translate.z,storeOrder);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> translation(float x,float y,StorageOrder storeOrder)
		{
			return translation(x,y,0,storeOrder);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(float x,float y,float z,StorageOrder storeOrder)
		{
			// unused
			(void)storeOrder;

			float ret[]={x,0,0,0,
						 0,y,0,0,
						 0,0,z,0,
						 0,0,0,1};

			return matrix<4,4,float>(ret);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(const vec2 &scale,StorageOrder storeOrder)
		{
			return scaling(scale.x,scale.y,1,storeOrder);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(const vec3 &scale,StorageOrder storeOrder)
		{
			return scaling(scale.x,scale.y,scale.z,storeOrder);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> scaling(float x,float y,StorageOrder storeOrder)
		{
			return scaling(x,y,1,storeOrder);
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> rotation(const Angle &angle,StorageOrder storeOrder)
		{
			float z=*((float*)&angle);
			if (storeOrder==RowMajor)
			{
				float ret[]={std::cos(z),-std::sin(z),0,0,
							 std::sin(z), std::cos(z),0,0,
									   0,		    0,1,0,
									   0,		    0,0,1};

				return matrix<4,4,float>(ret);
			}
			float ret[]={std::cos(z), std::sin(z),0,0,
						-std::sin(z), std::cos(z),0,0,
								   0,		    0,1,0,
								   0,		    0,0,1};

			return matrix<4,4,float>(ret);

		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> perspective(const Angle &fieldOfView,float aspect,float Znear,float Zfar,StorageOrder storeOrder)
		{
			float fov=*((float*)&fieldOfView);
			if (storeOrder==RowMajor)
			{
				float n[4][4]={{1.f/std::tan(fov/2.f)/aspect,0,0,0},
							   {0,1.f/std::tan(fov/2.f),0,0},
							   {0,0,(Zfar+Znear)/(Znear-Zfar),(2*Znear*Zfar)/(Znear-Zfar)},
							   {0,0,-1,0}};
				return matrix<4,4,float>(n);
			}
			float n[4][4]={{1.f/std::tan(fov/2.f)/aspect,0,0,0},
						   {0,1.f/std::tan(fov/2.f),0,0},
						   {0,0,(Zfar+Znear)/(Znear-Zfar),-1},
						   {0,0,(2*Znear*Zfar)/(Znear-Zfar),0}};
			return matrix<4,4,float>(n);

		}

		/////////////////////////////////////////////////////////////
		inline matrix<4,4,float> ortho(float left,float bottom,float right,float top,float nearVal,float farVal,StorageOrder storeOrder)
		{
			if (storeOrder==RowMajor)
			{
				float n[4][4]={{2.f/(right-left),0,0,(right+left)/(left-right)},
							   {0,2.f/(top-bottom),0,(top+bottom)/(bottom-top)},
							   {0,0,2.f/(farVal-nearVal),(farVal+nearVal)/(nearVal-farVal)  },
							   {0,0,0,1}};
				return matrix<4,4,float>(n);
			}
			float n[4][4]={{2.f/(right-left),0,0,0},
						   {0,2.f/(top-bottom),0,0},
						   {0,0,2.f/(farVal-nearVal),0},
						   {(right+left)/(left-right),(top+bottom)/(bottom-top),(farVal + nearVal)/(nearVal - farVal),1}};
			return matrix<4,4,float>(n);
		}


		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,const vec3 &up_dir,StorageOrder storeOrder)
		{
			mat4 t = translation(-cam_pos,storeOrder);
			vec3 f = (target_pos - cam_pos).sgn();
			vec3 r = f.cross(up_dir).sgn();
			vec3 u = r.cross(f).sgn();
			if (storeOrder==RowMajor)
			{
				float ret[]={ r.x,  r.y,  r.z,  0,
							  u.x,  u.y,  u.z,  0,
							 -f.x, -f.y, -f.z,  0,
								0,    0,    0,  1};
				return matrix<4,4,float>(ret)*t;
			}
			float ret[]={ r.x,  u.x, -f.x,  0,
						  r.y,  u.y, -f.y,  0,
						  r.z,  u.z, -f.z,  0,
							0,    0,    0,  1};
			return matrix<4,4,float>(ret)*t;
		}

		////////////////////////////////////////////////////////////
		inline matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,StorageOrder storeOrder)
		{
			return lookAt(cam_pos,target_pos,vec3(0,1,0),storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos)
		{
			return lookAt(cam_pos,target_pos,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> lookAt(const vec3 &cam_pos,const vec3 &target_pos,const vec3 &up_dir)
		{
			return lookAt(cam_pos,target_pos,up_dir,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> scaling(const vec2 &scale)
		{
			return scaling(scale,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> scaling(const vec3 &scale)
		{
			return scaling(scale,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> scaling(float x,float y)
		{
			return scaling(x,y,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> scaling(float x,float y,float z)
		{
			return scaling(x,y,z,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> translation(const vec3 &translate)
		{
			return translation(translate,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> translation(float x,float y)
		{
			return translation(x,y,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> translation(float x,float y,float z)
		{
			return translation(x,y,z,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> rotation(const Angle &angle)
		{
			return rotation(angle,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> perspective(const Angle &fieldOfView,float aspect,float Znear,float Zfar)
		{
			return perspective(fieldOfView,aspect,Znear,Zfar,storeOrder);
		}

		////////////////////////////////////////////////////////////
		template<StorageOrder storeOrder>
		inline matrix<4,4,float> ortho(float left,float bottom,float right,float top,float nearVal,float farVal)
		{
			return ortho(left,bottom,right,top,nearVal,farVal,storeOrder);
		}
	}
}
namespace std
{
    template<typename,typename>
    class basic_ostream;
    template<typename,typename>
    class basic_istream;
    template<typename>
    class char_traits;
}

////////////////////////////////////////////////////////////
template<fm::Size W,fm::Size H,class T>
inline std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out, const fm::matrix<W,H,T> &mat)
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
template<fm::Size W,fm::Size H,class T>
inline std::basic_istream<char, std::char_traits<char> > &operator>>(std::basic_istream<char, std::char_traits<char> > &in, fm::matrix<W,H,T> &mat)
{
	Cx(W) Cy(H)
		in>>mat[x][y];
	return in;
}

namespace fm
{
	/// vector multiplication /////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> operator*(const matrix<2,2,T> &mat,const vector2<T> &vec)
	{
		return vector2<T>(mat[0][0]*vec.x + mat[0][1]*vec.y,
						  mat[1][0]*vec.x + mat[1][1]*vec.y);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> operator*(const matrix<3,3,T> &mat,const vector3<T> &vec)
	{
		return vector3<T>(mat[0][0]*vec.x + mat[0][1]*vec.y + mat[0][2]*vec.z,
						  mat[1][0]*vec.x + mat[1][1]*vec.y + mat[1][2]*vec.z,
						  mat[2][0]*vec.x + mat[2][1]*vec.y + mat[2][2]*vec.z);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> operator*(const matrix<4,4,T> &mat,const vector4<T> &vec)
	{
		return vector4<T>(mat[0][0]*vec.x + mat[0][1]*vec.y + mat[0][2]*vec.z + mat[0][3]*vec.w,
						  mat[1][0]*vec.x + mat[1][1]*vec.y + mat[1][2]*vec.z + mat[1][3]*vec.w,
						  mat[2][0]*vec.x + mat[2][1]*vec.y + mat[2][2]*vec.z + mat[2][3]*vec.w,
						  mat[3][0]*vec.x + mat[3][1]*vec.y + mat[3][2]*vec.z + mat[3][3]*vec.w);
	}



	////////////////////////////////////////////////////////////
	template<class T>
	inline vector2<T> operator*(const vector2<T> &vec,const matrix<2,2,T> &mat)
	{
		return vector2<T>(mat[0][0]*vec.x + mat[1][0]*vec.y,
						  mat[0][1]*vec.x + mat[1][1]*vec.y);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector3<T> operator*(const vector3<T> &vec,const matrix<3,3,T> &mat)
	{
		return vector3<T>(mat[0][0]*vec.x + mat[1][0]*vec.y + mat[2][0]*vec.z,
						  mat[0][1]*vec.x + mat[1][1]*vec.y + mat[2][1]*vec.z,
						  mat[0][2]*vec.x + mat[1][2]*vec.y + mat[2][2]*vec.z);
	}

	////////////////////////////////////////////////////////////
	template<class T>
	inline vector4<T> operator*(const vector4<T> &vec,const matrix<4,4,T> &mat)
	{
		return vector4<T>(mat[0][0]*vec.x + mat[1][0]*vec.y + mat[2][0]*vec.z + mat[3][0]*vec.w,
						  mat[0][1]*vec.x + mat[1][1]*vec.y + mat[2][1]*vec.z + mat[3][1]*vec.w,
						  mat[0][2]*vec.x + mat[1][2]*vec.y + mat[2][2]*vec.z + mat[3][2]*vec.w,
						  mat[0][3]*vec.x + mat[1][3]*vec.y + mat[2][3]*vec.z + mat[3][3]*vec.w);
	}
}
#endif
