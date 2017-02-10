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
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Polar3.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <map>
#include <set>

namespace fg
{
	/////////////////////////////////////////////////////////////
	Mesh::Face::Face(fg::Primitive primitive ,fm::Size indexCount) : indices(indexCount,0),
																	 primitive(primitive)
	{
		
	}
	
#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Mesh::Mesh(const Mesh &copy)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		(*this) = copy;
	}
#endif
	
	/////////////////////////////////////////////////////////////
	Mesh::Mesh(Mesh &&move)
	{
		move.swap(*this);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::operator=(const Mesh &copy)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		pts    .assign(copy.pts.begin(),   copy.pts.end());
		uvs    .assign(copy.uvs.begin(),   copy.uvs.end());
		norms  .assign(copy.norms.begin(), copy.norms.end());
		tans   .assign(copy.tans.begin(),  copy.tans.end());
		bitans .assign(copy.bitans.begin(),copy.bitans.end());
	    
		faces.assign(copy.faces.begin(),copy.faces.end());

	    return *this;
	}
#endif
	
	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::operator=(Mesh &&move)
	{
		move.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::swap(Mesh &target)
	{
		pts    .swap(target.pts    );
		uvs    .swap(target.uvs    );
		norms  .swap(target.norms  );
		tans   .swap(target.tans   );
		bitans .swap(target.bitans );
		
		faces  .swap(target.faces);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
    Mesh Mesh::getSphere(float radius,fm::Size W,fm::Size H,const fm::Delegate<float,float &,float &> &rfunc)
    {
        Mesh ret;
        ret.pts.resize((W+1)*H);
        ret.uvs.resize((W+1)*H);
      
        ret.faces.push_back(Mesh::Face(fg::Triangles,W*(2*H-2)*3));
        
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

				ret.pts[ptsi++]  = p;
				ret.uvs[uvsi++] = fm::vec2(xpO,ypO);

                if (x<W && y>0)
                    inds[indi++] = (x+0)*H+y-1,
                    inds[indi++] = (x+0)*H+y+0,
                    inds[indi++] = (x+1)*H+y+0;

                if (x<W && y<H-1)
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

		fm::vec3 u = fm::vec3(fm::pol3(1,fm::deg(0),fm::deg(0))).cross(fm::vec3(fm::pol3(1,fm::deg(0),fm::deg(90))));

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

				fm::vec2 minorP = fm::pol2(minorR*r,fm::deg(yp*360));

				fm::vec3 p = majorP + f*minorP.x + u*minorP.y;

				ret.pts[ptsi++] = p;
				ret.uvs[uvsi++] = fm::vec2(xpO,ypO);

				fm::vec2i offsets[] = {fm::vec2i(0,0),fm::vec2i(1,0),fm::vec2i(1,1),
									   fm::vec2i(0,0),fm::vec2i(1,1),fm::vec2i(0,1)};

				if (x < W && y < H)
					C(6)
						ret.faces[0].indices[indi++] = (x+offsets[i].x)*(H+1) + y+offsets[i].y;
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

		    fm::vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
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

		    fm::vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
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
			
			fm::vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
			ret.pts[ptsi++] = fm::vec3(p.x,height*yp - height/2,p.y);
			
			fm::vec2s inds[] = {fm::vec2s(0,0),fm::vec2s(1,1),fm::vec2s(1,0),
								fm::vec2s(0,0),fm::vec2s(0,1),fm::vec2s(1,1)};
			
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
        
		fm::vec2 texps[] = {fm::vec2(1,2),fm::vec2(1,0),fm::vec2(1,2),
							fm::vec2(2,2),fm::vec2(1,3),fm::vec2(4,2)};
        
		fm::vec2 texpm[] = {fm::vec2(-1,-1),fm::vec2(1,1),fm::vec2(1,-1),
							fm::vec2(1,-1),fm::vec2(1,-1),fm::vec2(-1,-1)};
        
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
				
				fm::vec2 texp = (i%3==2 ? fm::vec2(yp,xp) : fm::vec2(xp,yp));
				
				ret.uvs[uvsi++] = (texps[i] + texp*texpm[i])/fm::vec2(4,3);
				
				if (x+1 < N && y+1 < N)
				{
					fm::vec2s inds[] = {fm::vec2s(0,0),fm::vec2s(1,1),fm::vec2s(1,0),
										fm::vec2s(0,0),fm::vec2s(0,1),fm::vec2s(1,1)};
					
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
	class Comparator
    {
    public:
    	float epsilon;
    	Comparator(float ep) : epsilon(ep) {}

        bool operator()(const fm::vec3 &l,const fm::vec3 &r)
        {
			if ((l-r).length() < epsilon) return false;
            if (l.x != r.x) return l.x < r.x;
            if (l.y != r.y) return l.y < r.y;
            if (l.z != r.z) return l.z < r.z;

			return false;
        }
	};
	
	/////////////////////////////////////////////////////////////
    Mesh::reference Mesh::calcNormals(bool joinIdenticalVertices)
    {
		if (pts.size() < 3)
			return *this;
            
		norms.resize(pts.size(),fm::vec3());
		
		float mnDst = -1;
		
		for (fm::Size faceIndex = 0;faceIndex < faces.size();++faceIndex)
		{
			fg::Primitive &primitive = faces[faceIndex].primitive;
			
			std::vector<fm::Uint32> &indices = faces[faceIndex].indices;
		
			if (primitive != fg::Triangles &&
				primitive != fg::TriangleStrip &&
				primitive != fg::TriangleFan)
				continue;

			bool useInds = (indices.size()!=0);

			C((useInds ? indices.size() : pts.size()) - 2)
			{
				fm::Uint32 ind0 = useInds ? indices[i+0] : i+0;
				fm::Uint32 ind1 = useInds ? indices[i+1] : i+1;
				fm::Uint32 ind2 = useInds ? indices[i+2] : i+2;

				if (primitive == fg::Triangles)
				{
					i += 2;
				}
				else if (primitive == fg::TriangleFan)
				{
					ind0 = 0;
				}

				fm::vec3 v0 = pts[ind1]-pts[ind0], v1 = pts[ind2]-pts[ind0], v2 = pts[ind2]-pts[ind1];
				
				if (mnDst == -1) mnDst = v0.length();
				
				mnDst = std::min(mnDst,v0.length());
				mnDst = std::min(mnDst,v1.length());
				mnDst = std::min(mnDst,v2.length());

				fm::vec3 N = (v0).cross(v1);
				norms[ind0] += N;
				norms[ind1] += N;
				norms[ind2] += N;
			}
		}
		
		if (joinIdenticalVertices)
		{
			for (fm::Size faceIndex = 0;faceIndex < faces.size();++faceIndex)
			{
				std::map<fm::vec3,std::set<fm::Size>,Comparator > pToInds(Comparator(mnDst*.9));
				
				C(faces[faceIndex].indices.size())
					pToInds[pts[faces[faceIndex].indices[i]]].insert(faces[faceIndex].indices[i]);

				for (std::map<fm::vec3,std::set<fm::Size>,Comparator >::const_iterator it = pToInds.begin();it != pToInds.end();++it)
				{
					fm::vec3 n;
					
					for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
						n += norms[*pit];
					
					for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
						norms[*pit] = n;
				}
			}
		}

		C(norms.size())
			norms[i] = norms[i].sgn();
		
		return *this;
    }

	/////////////////////////////////////////////////////////////
	Mesh::reference Mesh::calcTangents()
    {
		if (pts.size() < 3 || uvs.size() < 3)
			return *this;

		for (fm::Size faceIndex = 0;faceIndex < faces.size();++faceIndex)
		{
			fg::Primitive &primitive = faces[faceIndex].primitive;
			
			std::vector<fm::Uint32> &indices = faces[faceIndex].indices;
		
			if (primitive != fg::Triangles &&
				primitive != fg::TriangleStrip &&
				primitive != fg::TriangleFan)
				continue;
			
			bool useInds = (indices.size()!=0);

			tans.resize(pts.size(),fm::vec3());
			bitans.resize(pts.size(),fm::vec3());

			C((useInds ? indices.size() : pts.size()) - 2)
			{
				fm::Uint32 ind0 = useInds ? indices[i+0] : i+0;
				fm::Uint32 ind1 = useInds ? indices[i+1] : i+1;
				fm::Uint32 ind2 = useInds ? indices[i+2] : i+2;

				if (primitive == fg::Triangles)
				{
					i += 2;
				}
				else if (primitive == fg::TriangleFan)
				{
					ind0 = 0;
				}

				fm::vec3 v1 = pts[ind1] - pts[ind0];
				fm::vec3 v2 = pts[ind2] - pts[ind0];

				fm::vec2 s1 = uvs[ind1] - uvs[ind0];
				fm::vec2 s2 = uvs[ind2] - uvs[ind0];

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
			}
        }

        C(tans.size())
			tans[i] = tans[i].sgn();

        C(bitans.size())
			bitans[i] = bitans[i].sgn();
		
		return *this;
    }
}
