#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <type_traits>
#include <iostream>
#include <future>
#include <thread>

using namespace std;

namespace teapot
{
	#include "utah.teapot.txt"
}

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
		typedef T value_type;
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		
		T &operator[](fm::Size index);
		const T &operator[](fm::Size index) const;
		
		auto eval(fm::vec2d p) const -> decltype(fm::priv::eval_bezier(*this,p)) {return fm::priv::eval_bezier(*this,p);}
		
		T eval(double t) const;
		
		fm::Size size() const;
		
		BezierCurve(fm::Size n = 4,T val = T());
		
		iterator begin() {return m_controlPts.begin();}
		const_iterator begin() const {return m_controlPts.begin();}
		
		iterator end() {return m_controlPts.end();}
		const_iterator end() const {return m_controlPts.end();}
		
		T operator()(double t) const {return eval(t);}
		auto operator()(fm::vec2d t) const -> decltype(eval(fm::vec2d())) const {return eval(t);}
		
	private:
		std::vector<T> m_controlPts;
		
		T deCasteljou(double t) const;
	};

	template<class T>
	inline fm::Size BezierCurve<T>::size() const
	{
		return m_controlPts.size();
	}

	template<class T>
	inline BezierCurve<T>::BezierCurve(fm::Size n,T val) :
		m_controlPts(n,val)
	{
		
	}
		
	template<class T>
	inline T &BezierCurve<T>::operator[](fm::Size index)
	{
		return m_controlPts[index];
	}

	template<class T>
	inline const T &BezierCurve<T>::operator[](fm::Size index) const
	{
		return m_controlPts[index];
	}

	template<class T>
	inline T BezierCurve<T>::deCasteljou(double t) const
	{
		auto pts = m_controlPts;
		
		for (fm::Size lvl=0;lvl<size();++lvl)
			for (fm::Size i=1;i<size()-lvl;++i)
				pts[i-1] = pts[i-1] * (1-t) + pts[i] * t;
		
		return std::move(pts[0]);
	}

	template<class T>
	inline T BezierCurve<T>::eval(double t) const
	{
		return deCasteljou(t);
	}
		
	template<class T>
	inline BezierCurve<T> operator*(const BezierCurve<T> &curve,double t)
	{
		BezierCurve<T> cpy(curve.size());
		
		C(curve.size()) cpy[i] = curve[i] * t;
		
		return cpy;
	}
		
	template<class T>
	inline BezierCurve<T> operator+(const BezierCurve<T> &left,const BezierCurve<T> &right)
	{
		auto s = std::min(left.size(),right.size());
		
		BezierCurve<T> ret(s);
		
		C(s) ret[i] = left[i] + right[i];
		
		return ret;
	}
		
	template<class T>
	inline BezierCurve<T> operator-(const BezierCurve<T> &left,const BezierCurve<T> &right)
	{
		auto s = std::min(left.size(),right.size());
		
		BezierCurve<T> ret(s);
		
		C(s) ret[i] = left[i] - right[i];
		
		return ret;
	}
}

int main(int argc,char *argv[])
{
	fm::Size n = argc==1 ? 10 : atoi(argv[1]);
	Mesh m;
	m.faces.push_back(Mesh::Face(Triangles,0));
	auto &inds = m.faces.back().indices;
	
	for (fm::Size k=0;k<teapot::nPatches;++k) {
		BezierCurve<BezierCurve<vec3>> bezier(4);
		Cx(4) {
			bezier[x] = std::move(BezierCurve<vec3>(4));
			Cy(4)
				bezier[x][y] = teapot::vertices[teapot::patches[k][x*4+y]-1];
		}
		
		m.pts.reserve(m.pts.size() + n*n);
		m.uvs.reserve(m.uvs.size() + n*n);
		
		fm::Size pbase = m.pts.size();
		fm::Size ibase = inds.size();
		
		inds.resize(inds.size() + (n-1)*(n-1)*6);
		
		Cxy(n,n) {
			vec2d uv = {x/double(n-1),y/double(n-1)};
			auto p = bezier(uv);
			m.pts.push_back(p);
			m.uvs.push_back(uv);
			
			if (x != n-1 && y != n-1) {
				inds[(x*(n-1)+y)*6+0+ibase] = pbase+x*n+y;
				inds[(x*(n-1)+y)*6+1+ibase] = pbase+(x+1)*n+y;
				inds[(x*(n-1)+y)*6+2+ibase] = pbase+(x+1)*n+(y+1);
				inds[(x*(n-1)+y)*6+3+ibase] = pbase+x*n+y;
				inds[(x*(n-1)+y)*6+4+ibase] = pbase+(x+1)*n+(y+1);
				inds[(x*(n-1)+y)*6+5+ibase] = pbase+x*n+(y+1);
			}
		}
	}
	
	m.calcNormals();
	m.clr.resize(m.pts.size());
	
	for (::size_t i=0;i<m.clr.size();++i)
	 	m.clr[i] = m.norms[i];
	
	fgui::plot(std::move(m)).wait();
}
