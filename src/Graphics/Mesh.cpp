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
#include <FRONTIER/System/BezierCurve.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Polar3.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
using namespace std;

namespace fg
{
	using fm::vec2;
	using fm::vec3;
	using fm::vec2i;
	using fm::vec2s;
	
	/////////////////////////////////////////////////////////////
	Mesh::Face::Face(fg::Primitive primitive) :
		primitive(primitive),
		beg(0),
		len(fm::Size(-1))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::Face::Face(fg::Primitive primitive,fm::Size indexCount) :
		indices(indexCount,0),
		primitive(primitive),
		beg(0),
		len(fm::Size(-1))
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::Face::Face(fg::Primitive primitive,fm::Size beg,fm::Size len) : 
		primitive(primitive),
		beg(beg),
		len(len)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	bool Mesh::Face::useIndices() const
	{
		return indices.size();
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::Mesh(const Mesh &copy)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		pts    .assign(copy.pts.begin(),   copy.pts.end());
		uvs    .assign(copy.uvs.begin(),   copy.uvs.end());
		clr    .assign(copy.clr.begin(),   copy.clr.end());
		norms  .assign(copy.norms.begin(), copy.norms.end());
		tans   .assign(copy.tans.begin(),  copy.tans.end());
		bitans .assign(copy.bitans.begin(),copy.bitans.end());
		faces  .assign(copy.faces.begin(), copy.faces.end());
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::Mesh(Mesh &&move)
	{
		move.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::operator=(Mesh &&move)
	{
		return this->swap(move);
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::swap(Mesh &target)
	{
		pts    .swap(target.pts    );
		uvs    .swap(target.uvs    );
		clr    .swap(target.clr    );
		norms  .swap(target.norms  );
		tans   .swap(target.tans   );
		bitans .swap(target.bitans );
		
		faces  .swap(target.faces);
		
		return *this;
	}
		
	/////////////////////////////////////////////////////////////
	fm::box3f Mesh::AABB() const
	{
		fm::box3f aabb;
		
		if (pts.empty()) return aabb;
		
		aabb.pos = pts[0];
		for (auto p : pts) aabb.expand(p);
		
		return aabb;
	}

	/////////////////////////////////////////////////////////////
    Mesh Mesh::getSphere(float radius,fm::Size W,fm::Size H,const fm::Delegate<float,float &,float &> &rfunc)
    {
        Mesh ret;
        ret.pts.resize((W+1)*H);
        ret.uvs.resize((W+1)*H);
      
        ret.faces.push_back(Mesh::Face(fg::Triangles,W*(H-3)*6 + W*2*3));
        
        std::vector<fm::Uint32> &inds = ret.faces[0].indices;

        fm::Size ptsi = 0;
        fm::Size uvsi = 0;
        fm::Size indi = 0;

        Cx(W+1)
        {
			Cy(H)
			{
				float xpO = float(x)/(W);
				float ypO = float(y)/(H-1);
				float xp = xpO;
				float yp = ypO;
				float r = (rfunc ? rfunc(xp,yp) : 1.f);

				fm::vec3 p = fm::pol3(radius*r,fm::deg(xp*360),fm::deg(90-yp*180));

				ret.pts[ptsi++] = p;
				ret.uvs[uvsi++] = vec2(xpO,ypO);

                if (x<W && y>0 && y<H-1)
                    inds[indi++] = (x+0)*H+y-1,
                    inds[indi++] = (x+0)*H+y+0,
                    inds[indi++] = (x+1)*H+y+0;

                if (x<W && y<H-1 && y>0)
                    inds[indi++] = (x+1)*H+y+0,
                    inds[indi++] = (x+0)*H+y+0,
                    inds[indi++] = (x+1)*H+y+1;
			}
		}

        return ret;
    }
    
	/////////////////////////////////////////////////////////////
    Mesh Mesh::getTorus(float majorR,float minorR,fm::Size W,fm::Size H,const fm::Delegate<float,float &,float &> &rfunc)
    {
        Mesh ret;
        ret.pts.resize((W+1)*(H+1));
        ret.uvs.resize(ret.pts.size());
        
        ret.faces.push_back(Mesh::Face(fg::Triangles,W*H*2*3));

        fm::Size ptsi = 0;
        fm::Size uvsi = 0;
        fm::Size indi = 0;

		fm::vec3 u(0,-1,0);

        Cx(W+1)
        {
			Cy(H+1)
			{
                float xpO = float(x)/(W);
				float ypO = float(y)/(H);
                float xp = xpO;
				float yp = ypO;
				float r = (rfunc ? rfunc(xp,yp) : 1.f);

                fm::vec3 majorP = fm::pol3(majorR,fm::deg(0),fm::deg(xp*360));
                fm::vec3 f = majorP.sgn();

				vec2 minorP = fm::pol2(minorR*r,fm::deg(yp*360));

				fm::vec3 p = majorP + f*minorP.x + u*minorP.y;

				ret.pts[ptsi++] = p;
				ret.uvs[uvsi++] = vec2(xpO,ypO);

				if (x < W && y < H)
					for (auto o : {vec2i(0,0),vec2i(1,0),vec2i(1,1),
								   vec2i(0,0),vec2i(1,1),vec2i(0,1)})
						ret.faces[0].indices[indi++] = (x+o.x)*(H+1) + y+o.y;
			}
		}

        return ret;
    }

	/////////////////////////////////////////////////////////////
    Mesh Mesh::getCylinder(float radius,float height, fm::Size W,fm::Size H, const fm::Delegate<float,float &,float &> &rfunc)
	{
		Mesh ret;
		ret.pts.resize((W+1)*2 + 2 + H*(W+1));
		ret.uvs.resize(ret.pts.size());

        fm::Size ptsi = 0;
        // fm::Size uvsi = 0;
        fm::Size indi = 0;
        
        ret.faces.push_back(Mesh::Face(fg::Triangles,W*2*3));

        ret.pts[ptsi++] = fm::vec3(0,height/2,0);
		C(W+1)
		{
		    float xp = i/float(W);
		    float yp = 1;
		    float r = (rfunc ? rfunc(xp,yp) : 1.f);

		    vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
		    ret.pts[ptsi++] = fm::vec3(p.x,height/2,p.y);
		    if (i < W)
            {
                ret.faces.back().indices[indi++] = 0;
                ret.faces.back().indices[indi++] = i+2;
                ret.faces.back().indices[indi++] = i+1;
            }
		}

        ret.pts[ptsi++] = fm::vec3(0,-height/2,0);
		C(W+1)
		{
		    float xp = i/float(W);
		    float yp = 0;
		    float r = (rfunc ? rfunc(xp,yp) : 1.f);

		    vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
		    ret.pts[ptsi++] = fm::vec3(p.x,-height/2,p.y);
		    if (i < W)
            {
                ret.faces.back().indices[indi++] = W+2;
                ret.faces.back().indices[indi++] = W+2+i+1;
                ret.faces.back().indices[indi++] = W+2+i+2;
            }
		}
		
		ret.faces.push_back(Mesh::Face(fg::Triangles,W*(H-1)*2*3));
		indi = 0;
		
		Cxy(W+1,H)
		{
			float xp = x/float(W);
			float yp = y/float(H-1);
			
			float r = (rfunc ? rfunc(xp,yp) : 1.f);
			
			vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
			ret.pts[ptsi++] = fm::vec3(p.x,height*yp - height/2,p.y);
			
			vec2s inds[] = {vec2s(0,0),vec2s(1,1),vec2s(1,0),
								vec2s(0,0),vec2s(0,1),vec2s(1,1)};
			
			if (x < W && y < H-1)
				C(6)
					ret.faces.back().indices[indi++] = (W+1)*2+2 + (x+inds[i].x)*H + (y+inds[i].y);
		}
		
		return ret;
	}
    
	/////////////////////////////////////////////////////////////
    Mesh Mesh::getCube(float size,fm::Size N,const fm::Delegate<float,float &,float &> &rfunc)
    {
    	Mesh ret;
		ret.pts.resize(N*N*6);
		ret.uvs.resize(ret.pts.size());

        fm::Size ptsi = 0;
        fm::Size uvsi = 0;
        fm::Size indi = 0;
        
		vec2 texps[] = {vec2(1,2),vec2(1,0),vec2(1,2),
							vec2(2,2),vec2(1,3),vec2(4,2)};
        
		vec2 texpm[] = {vec2(-1,-1),vec2(1,1),vec2(1,-1),
							vec2(1,-1),vec2(1,-1),vec2(-1,-1)};
        
        C(6)
        {
			// (1,0,0),(0,1,0),(0,0,1),(-1,0,0),(0,-1,0),(0,0,-1),
			fm::vec3 n(i%3==0,i%3==1,i%3==2);
			fm::vec3 r(i%3==1,i%3==2,i%3==0);
			fm::vec3 u(i%3==2,i%3==0,i%3==1);
			
			if (i > 2)
				n *= -1;
			
			indi = 0;
			ret.faces.push_back(Mesh::Face(fg::Triangles,(N-1)*(N-1)*2*3));
			
			Cxy(N,N)
			{
				float xp = x/float(N-1);
				float yp = y/float(N-1);
				float d  = (rfunc ? rfunc(xp,yp) : 1.0f);
				
				ret.pts[ptsi++] = d/2*n*size + (xp-0.5)*r*size*d + (yp-0.5)*u*size*d;
				
				vec2 texp = (i%3==2 ? vec2(yp,xp) : vec2(xp,yp));
				
				ret.uvs[uvsi++] = (texps[i] + texp*texpm[i])/vec2(4,3);
				
				if (x+1 < N && y+1 < N)
				{
					vec2s inds[] = {vec2s(0,0),vec2s(1,1),vec2s(1,0),
										vec2s(0,0),vec2s(0,1),vec2s(1,1)};
					
					if (i > 2)
					{
						std::swap(inds[1].x,inds[1].y);
						std::swap(inds[2].x,inds[2].y);
						std::swap(inds[4].x,inds[4].y);
						std::swap(inds[5].x,inds[5].y);
					}
					
					Cz(6)
						ret.faces.back().indices[indi++] = i*N*N + (x+inds[z].x)*N + (y+inds[z].y);
				}
			}
        }
        
		return ret;
    }
	
	/////////////////////////////////////////////////////////////
	struct Pind
    {
    	fm::vec3 p;
		fm::Size i;
		
		bool far(const Pind &pind,fm::Size dim,double d) const {
			return std::abs(double(p[dim]) - pind.p[dim]) >= d;
		}
		
		bool operator<(const Pind &p) const {
			return i < p.i;
		}
		
		bool operator==(const Pind &p) const {
			return i == p.i;
		}
	};
	
	struct PindSorter {
		PindSorter(fm::Size i) : i(i) {}
		fm::Size i;
		
		bool operator()(const Pind &a,const Pind &b) const {
			return a.p[i] < b.p[i];
		}
	};
	
	template<class It>
	void forEachPindGroup(It beg,It end,double mnDst,fm::Delegate<void,It,It> cb,fm::Size dim = 0)
	{
		if (dim == 3) cb(beg, end);
		else {
			std::sort(beg,end,PindSorter(dim));
			
			It gbeg = beg;
			for (It cur = beg;cur != end;)
			{
				++cur;
				
				if (cur == end || gbeg->far(*cur,dim,mnDst)) {
					forEachPindGroup(gbeg, cur, mnDst, cb, dim+1);
					gbeg = cur;
				}
			}
		}
	}
	
	template<class T>
	T minButNot0(T a,T b)
	{
		if (b != T()) {
			if (a < T()) return b;
			return std::min(a,b);
		}
		return a;
	}
	
	/////////////////////////////////////////////////////////////
	void Mesh::forEachTriangle(const Face &face,fm::Delegate<void,fm::Uint32,fm::Uint32,fm::Uint32> cb) const
	{
		fg::Primitive primitive = face.primitive;
	
		if (primitive != fg::Triangles &&
			primitive != fg::TriangleStrip &&
			primitive != fg::TriangleFan)
			return;
		
		auto &indices = face.indices;

		for (fm::Size i=face.beg;i<face.beg + faceSize(face);++i)
		{
			fm::Uint32 ind0 = face.useIndices() ? indices[i+0] : i+0;
			fm::Uint32 ind1 = face.useIndices() ? indices[i+1] : i+1;
			fm::Uint32 ind2 = face.useIndices() ? indices[i+2] : i+2;

			if (primitive == fg::Triangles) i += 2;
			else if (primitive == fg::TriangleFan) ind0 = 0;

			cb(ind0,ind1,ind2);
		}
	}
	
	/////////////////////////////////////////////////////////////
	double Mesh::getEpsilon() const
	{
		double mnDst = -1;
		
		for (auto &face : faces)
		{
			forEachTriangle(face,[&](fm::Uint32 ind0,fm::Uint32 ind1,fm::Uint32 ind2){
				
				fm::vec3 v0 = pts[ind1]-pts[ind0], v1 = pts[ind2]-pts[ind0], v2 = pts[ind2]-pts[ind1];
				
				mnDst = minButNot0(mnDst,fm::vec3d(v0).LENGTH());
				mnDst = minButNot0(mnDst,fm::vec3d(v1).LENGTH());
				mnDst = minButNot0(mnDst,fm::vec3d(v2).LENGTH());
			});
		}
		
		if (mnDst == -1) return 0;
		
		return std::sqrt(mnDst) / 2;
	}
	
	/////////////////////////////////////////////////////////////
	void Mesh::joinNormals()
	{
		double mnDst = getEpsilon();
			
		for (auto &face : faces)
		{
			std::vector<Pind> points;
			points.reserve(faceSize(face));
			
			for (fm::Size i=face.beg;i<face.beg + faceSize(face);++i)
			{
				fm::Uint32 ind = face.useIndices() ? face.indices[i] : i;
				points.push_back(Pind{pts[ind],ind});
			}
			
			std::sort(points.begin(),points.end());
			points.erase(std::unique(points.begin(),points.end()),points.end());
			
			typedef std::vector<Pind>::iterator It;
			forEachPindGroup<It>(points.begin(), points.end(), mnDst, [&](It beg,It end){
				if (beg == end || beg+1 == end) return;
				
				fm::vec3 n;
				double cnt = 0;
				
				for (auto it = beg;it != end;++it) {
					n += norms[it->i];
					++cnt;
				}
				n /= cnt;
				
				for (auto it = beg;it != end;++it)
					norms[it->i] = n;
			});
		}
	}
	
	/////////////////////////////////////////////////////////////
    Mesh::reference Mesh::calcNormals(bool joinIdenticalVertices)
    {
		if (pts.size() < 3)
			return *this;
            
		norms.resize(pts.size(),fm::vec3());
		
		for (auto &face : faces)
		{
			forEachTriangle(face,[&](fm::Uint32 ind0,fm::Uint32 ind1,fm::Uint32 ind2){
				
				fm::vec3 v0 = pts[ind1]-pts[ind0], v1 = pts[ind2]-pts[ind0];
				
				fm::vec3 N = (v0).cross(v1);
				norms[ind0] += N;
				norms[ind1] += N;
				norms[ind2] += N;
			});
		}
		
		if (joinIdenticalVertices)
			joinNormals();
		
		for (auto &n : norms)
			n = n.sgn();
		
		return *this;
    }

	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::calcTangents(bool joinIdenticalVertices)
    {
		if (pts.size() < 3 || uvs.size() < 3)
			return *this;

		tans.resize(pts.size(),fm::vec3());
		bitans.resize(pts.size(),fm::vec3());
		
		for (auto &face : faces)
		{
			forEachTriangle(face,[&](fm::Uint32 ind0,fm::Uint32 ind1,fm::Uint32 ind2){
				
				fm::vec3 v1 = pts[ind1] - pts[ind0];
				fm::vec3 v2 = pts[ind2] - pts[ind0];

				vec2 s1 = uvs[ind1] - uvs[ind0];
				vec2 s2 = uvs[ind2] - uvs[ind0];

				float tmat[] = {s1.x,s1.y,
								s2.x,s2.y};

				float vmat[] = {v1.x,v1.y,v1.z,
								v2.x,v2.y,v2.z};

				fm::matrix<2,3> btmat = fm::mat2(tmat).inverse()*fm::matrix<2,3>(vmat);

				fm::vec3 T = -fm::vec3(btmat[0][0],btmat[0][1],btmat[0][2]);
				fm::vec3 B = -fm::vec3(btmat[1][0],btmat[1][1],btmat[1][2]);

				tans[ind0] += T;
				tans[ind1] += T;
				tans[ind2] += T;

				bitans[ind0] += B;
				bitans[ind1] += B;
				bitans[ind2] += B;
			});
        }
		
		if (joinIdenticalVertices) {
			norms.swap(tans);
			
			joinNormals();
			
			norms.swap(bitans);
			joinNormals();
			norms.swap(bitans);
			
			norms.swap(tans);
		}

        C(tans.size())
			tans[i] = tans[i].sgn();

        C(bitans.size())
			bitans[i] = bitans[i].sgn();
		
		return *this;
    }
	
	
	// TODO: rounded joints
	/////////////////////////////////////////////////////////////
	Mesh Mesh::tesLineStrip(const vec2 *pts,fm::Size N,float width,bool distField)
	{
		Mesh m;
		if (N < 2) return m;
		
		m.pts.resize((N-1)*3*2);
		if (distField)
			m.extras.push_back(std::vector<fm::vec4>(m.pts.size()));
		
		width /= 2;
		
		vec2 v = pts[1] - pts[0];
		vec2 n = v.perp().sgn();
		vec2 A = pts[0] + n * width, B = pts[0] - n * width;
		
		fm::Size baseIndex = 0;
		
		double lengthSum = 0;
		for (fm::Size i=0;i+1<N;++i) lengthSum += (pts[i+1] - pts[i]).length();
		
		double curLen = 0;
		
		for (fm::Size i=1;i<N;++i)
		{
			vec2 v2 = pts[i+1] - pts[i];
			vec2 n2 = v2.perp().sgn();
			
			vec2 C,D;
			
			if (i+1 < N)
			{
				double cos2a = n.dot(n2);
				double cosa  = std::sqrt((cos2a + 1) / 2.0);
				
				vec2 b = (n + n2).sgn() * width / cosa;
				C = pts[i] + b;
				D = pts[i] - b;
			}
			else
			{
				n2 = (pts[N-1] - pts[N-2]).perp().sgn();
				C = pts[N-1] + n2 * width;
				D = pts[N-1] - n2 * width;
			}
			
			for (vec2 p : {A,B,C,C,D,B})
				m.pts[baseIndex++] = p;
				
			if (distField)
			{
				float lenRat    = curLen / lengthSum;
				float nxtLenRat = (curLen + v.length()) / lengthSum;
				baseIndex -= 6;
				for (vec2 p : {vec2(1,lenRat),vec2(0,lenRat),vec2(1,nxtLenRat),vec2(1,nxtLenRat),vec2(0,nxtLenRat),vec2(0,lenRat)})
					m.extras.back()[baseIndex++] = fm::vec4(p.x,1-p.x,p.y,1-p.y) * fm::vec4(width*2,width*2,lengthSum,lengthSum);
			}
			
			curLen += v.length();
			v = v2; n = n2;
			A = C;  B = D;
		}
		
		return m;
	}
    
	/////////////////////////////////////////////////////////////
	Mesh Mesh::getCircle(float radius,fm::Size N,const fm::Delegate<float,float &> &rfunc)
	{
		Mesh ret;
		ret.pts.resize(N);
		ret.uvs.resize(ret.pts.size());
		ret.faces.push_back(Mesh::Face(fg::TriangleFan,N));
		
		C(N)
		{
			float v = float(i) / N;
			float r = (rfunc ? rfunc(v) : 1.0);
			
			vec2 p = fm::pol2(r,fm::deg(360.0 * v));
			
			ret.pts[i] = p * radius;
			ret.uvs[i] = p/2 + vec2(.5,.5);
			
			ret.faces.back().indices[i] = i;
		}
        
		return ret;
	}
    
	/////////////////////////////////////////////////////////////
	Mesh Mesh::getRectangle(float width,float height,fm::Size W,fm::Size H,const fm::Delegate<float,float &,float &> &rfunc)
	{
		Mesh ret;
		ret.pts.resize(2*W + 2*H);
		ret.uvs.resize(ret.pts.size());
		ret.faces.push_back(Mesh::Face(fg::TriangleFan,2*W + 2*H));
		
		fm::Size index = 0;
		
		auto helper = [&](float onx,float ony) {
			
			float r = (rfunc ? rfunc(onx,ony) : 1.0);
			
			vec2 p(onx * r,ony * r);
			
			ret.pts[index] = p * vec2(width,height);
			ret.uvs[index] = p;
			
			ret.faces.back().indices[index] = index;
			++index;
		};
		
		C(W) helper(float(i)/(W-1),1);
		C(H) helper(1,1-float(i)/(H-1));
		C(W) helper(1-float(i)/(W-1),0);
		C(H) helper(0,float(i)/(H-1));
        
		return ret;
	}
	
	namespace {
		namespace teapot
		{
			#include "models/utah.teapot.txt"
		}
	}
	
	/////////////////////////////////////////////////////////////
	Mesh Mesh::getTeapot(fm::Size N,float size)
	{
		Mesh m;
		m.faces.push_back(Mesh::Face(Triangles,0));
		auto &inds = m.faces.back().indices;
		
		for (fm::Size k=0;k<teapot::nPatches;++k) {
			fm::BezierSurface<fm::vec3> bezier(4);
			Cxy(4,4)
				bezier[x][y] = teapot::vertices[teapot::patches[k][x*4+y]-1];
			
			m.pts.reserve(m.pts.size() + N*N);
			m.uvs.reserve(m.uvs.size() + N*N);
			
			fm::Size pbase = m.pts.size();
			fm::Size ibase = inds.size();
			
			inds.resize(inds.size() + (N-1)*(N-1)*6);
			
			Cxy(N,N) {
				fm::vec2d uv = {x/double(N-1),y/double(N-1)};
				auto p = bezier(uv);
				m.pts.push_back(p*size);
				m.uvs.push_back(uv);
				
				if (x != N-1 && y != N-1) {
					inds[(x*(N-1)+y)*6+0+ibase] = pbase+x*N+y;
					inds[(x*(N-1)+y)*6+1+ibase] = pbase+(x+1)*N+(y+1);
					inds[(x*(N-1)+y)*6+2+ibase] = pbase+(x+1)*N+y;
					inds[(x*(N-1)+y)*6+3+ibase] = pbase+x*N+y;
					inds[(x*(N-1)+y)*6+4+ibase] = pbase+x*N+(y+1);
					inds[(x*(N-1)+y)*6+5+ibase] = pbase+(x+1)*N+(y+1);
				}
			}
		}
		
		return m;
	}
	
	
	/////////////////////////////////////////////////////////////
	Mesh Mesh::getDiamond(fm::Size N,float size)
	{
		vector<vec2> cps{vec2(.1,0),vec2(.1,.1),vec2(1,.95),vec2(1.1,.96),vec2(1.4,.7),vec2(1.4,0)};
		fm::Size s = cps.size();
		
		Mesh m;
				
		for (fm::Size i=0;i<N;++i) // slice
		{
			fm::Anglef acur = fm::deg(360*i/float(N));
			fm::Anglef anxt = fm::deg(360*(i+1)/float(N));
			vec2 pcur = fm::pol2(1,acur);
			vec2 pnxt = fm::pol2(1,anxt);
			
			float ir = float(i)/N;
			float irn = float(i+1)/N;
			
			for (fm::Size j=0;j+1<s;++j) {
				vec2 pa0 = pcur * cps[j].y;
				vec2 pn0 = pnxt * cps[j].y;
				vec2 pa1 = pcur * cps[j+1].y;
				vec2 pn1 = pnxt * cps[j+1].y;
				
				float jr = float(j)/s;
				float jrn = float(j+1)/s;
				
				if (j+2 != s) {
					m.faces.push_back(Mesh::Face(fg::Triangles,m.pts.size(),3));
					m.pts.push_back(vec3(pa0.x,cps[j].x,pa0.y));
					m.pts.push_back(vec3(pa1.x,cps[j+1].x,pa1.y));
					m.pts.push_back(vec3(pn1.x,cps[j+1].x,pn1.y));
					
					m.uvs.push_back(vec2(ir,jr));
					m.uvs.push_back(vec2(ir,jrn));
					m.uvs.push_back(vec2(irn,jrn));
				}
				
				if (j) {
					m.faces.push_back(Mesh::Face(fg::Triangles,m.pts.size(),3));
					m.pts.push_back(vec3(pa0.x,cps[j].x,pa0.y));
					m.pts.push_back(vec3(pn1.x,cps[j+1].x,pn1.y));
					m.pts.push_back(vec3(pn0.x,cps[j].x,pn0.y));
					
					m.uvs.push_back(vec2(ir,jr));
					m.uvs.push_back(vec2(irn,jrn));
					m.uvs.push_back(vec2(irn,jr));
				}
			}
		}
		
		for (auto &p : m.pts) p *= size;
		
		return m;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size Mesh::faceSize(const Face &face) const
	{
		fm::Size s = (face.useIndices() ? face.indices.size() : pts.size());
		
		if (face.beg >= s) return 0;
		
		return std::min(face.len, s - face.beg);
	}
}
