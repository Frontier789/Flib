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
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Polar3.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <set>
#include <map>

namespace fg
{
	/////////////////////////////////////////////////////////////
	Mesh::Mesh() : primitive(fg::Triangles)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Mesh Mesh::copy()
	{
	    Mesh ret;
	    ret.pts.assign(pts.begin(),pts.end());
	    ret.uvs.assign(uvs.begin(),uvs.end());
	    ret.norms.assign(norms.begin(),norms.end());
	    ret.tans.assign(tans.begin(),tans.end());
	    ret.bitans.assign(bitans.begin(),bitans.end());
	    ret.indices.assign(indices.begin(),indices.end());
		ret.primitive = primitive;

	    return ret;
	}

	/////////////////////////////////////////////////////////////
    void Mesh::swap(Mesh &target)
    {
        target.pts.swap(pts);
	    target.uvs.swap(uvs);
	    target.norms.swap(norms);
	    target.tans.swap(tans);
	    target.bitans.swap(bitans);
	    target.indices.swap(indices);
	    std::swap(primitive,target.primitive);
    }

	/////////////////////////////////////////////////////////////
    Mesh Mesh::getSphere(float radius,fm::Size W,fm::Size H,const fm::Delegate<float,float &,float &> &rfunc)
    {
        Mesh ret;
        ret.pts.resize((W+1)*H);
        ret.uvs.resize((W+1)*H);
        ret.primitive = fg::Triangles;
        ret.indices.resize(W*(2*H-2)*3);
        std::vector<fm::Uint32> &inds = ret.indices;

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
        ret.uvs.resize((W+1)*(H+1));
        ret.primitive = fg::Triangles;
        ret.indices.resize(W*(H+1)*2*3);

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

				if (x < W)
					C(6)
						ret.indices[indi++] = (x+offsets[i].x)*(H+1) + (y+offsets[i].y)%(H+1);
			}
		}

        return ret;
    }

	/////////////////////////////////////////////////////////////
    void Mesh::calcNormals()
    {
		if (pts.size() < 3)
            return;
           
		if (primitive != fg::Triangles && 
			primitive != fg::TriangleStrip && 
			primitive != fg::TriangleFan)
			return;
		
		bool useInds = (indices.size()!=0);
		
		norms.resize(pts.size(),fm::vec3());
		
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
			
			fm::vec3 N = (pts[ind1]-pts[ind0]).cross(pts[ind2]-pts[ind0]);
			norms[ind0] += N;
			norms[ind1] += N;
			norms[ind2] += N;
        }
        
        C(norms.size())
			norms[i] = norms[i].sgn();
    }

	/////////////////////////////////////////////////////////////
	void Mesh::calcTangents()
    {
		if (pts.size() < 3 || uvs.size() < 3)
            return;
           
		if (primitive != fg::Triangles && 
			primitive != fg::TriangleStrip && 
			primitive != fg::TriangleFan)
			return;
		
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

			fm::matrix<2,3> btmat = fm::MATRIX::inverse(fm::mat2(tmat))*fm::matrix<2,3>(vmat);

			fm::vec3 T = -fm::vec3(btmat[0][0],btmat[0][1],btmat[0][2]);
			fm::vec3 B = -fm::vec3(btmat[1][0],btmat[1][1],btmat[1][2]);

			tans[ind0] += T;
			tans[ind1] += T;
			tans[ind2] += T;

			bitans[ind0] += B;
			bitans[ind1] += B;
			bitans[ind2] += B;
        }
        
        C(tans.size())
			tans[i] = tans[i].sgn();
			
        C(bitans.size())
			bitans[i] = bitans[i].sgn();
    }
}
