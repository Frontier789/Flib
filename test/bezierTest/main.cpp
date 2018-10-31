#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <future>
#include <thread>

using namespace std;

namespace teapot
{
	#include "utah.teapot.txt"
}

template<class T>
struct BezierCurve
{
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	
	T &operator[](fm::Size index);
	const T &operator[](fm::Size index) const;
	
	T eval(double t) const;
	
	fm::Size size() const;
	
	BezierCurve(fm::Size n = 4,T val = T());
	
	iterator begin() {return m_controlPts.begin();}
	const_iterator begin() const {return m_controlPts.begin();}
	
	iterator end() {return m_controlPts.end();}
	const_iterator end() const {return m_controlPts.end();}
	
	T operator()(double t) const {return eval(t);}
	
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
	
	return pts[0];
}

template<class T>
inline T BezierCurve<T>::eval(double t) const
{
	return deCasteljou(t);
}
	
template<class T>
BezierCurve<T> operator*(const BezierCurve<T> &curve,double t)
{
	auto cpy = curve;
	
	for (auto &p : cpy) p = p*t;
	
	return cpy;
}
	
template<class T>
BezierCurve<T> operator+(const BezierCurve<T> &left,const BezierCurve<T> &right)
{
	auto s = std::min(left.size(),right.size());
	
	BezierCurve<T> ret(s);
	
	C(s) ret[i] = left[i] + right[i];
	
	return ret;
}


int main(int argc,char *argv[])
{
	BezierCurve<BezierCurve<vec3>> bezier(4,BezierCurve<vec3>(4));
	fm::Size n = argc==1 ? 10 : atoi(argv[1]);
	Mesh m;
	m.faces.push_back(Mesh::Face(Triangles,0));
	auto &inds = m.faces.back().indices;
	
	for (fm::Size k=0;k<teapot::nPatches;++k) {
		Cxy(4,4)
			bezier[x][y] = teapot::vertices[teapot::patches[k][x*4+y]-1];
		
		m.pts.reserve(m.pts.size() + n*n);
		m.uvs.reserve(m.uvs.size() + n*n);
		
		fm::Size pbase = m.pts.size();
		fm::Size ibase = inds.size();
		
		inds.resize(inds.size() + (n-1)*(n-1)*6);
		
		Cxy(n,n) {
			vec2d uv = {x/double(n-1),y/double(n-1)};
			auto p = bezier.eval(uv.x).eval(uv.y);
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
	
	m = Mesh::getTorus(1,.2,30,30);
	
	m.calcNormals(false);
	m.clr.resize(m.pts.size());
	
	for (::size_t i=0;i<m.clr.size();++i)
	 	m.clr[i] = m.norms[i];
	
	fgui::plot(std::move(m)).wait();
}
