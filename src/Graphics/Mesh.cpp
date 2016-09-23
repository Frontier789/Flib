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
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Polar3.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Swap.hpp>
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
	Mesh::reference Mesh::swap(Mesh &target)
	{
		pts    .swap(target.pts    );
		uvs    .swap(target.uvs    );
		norms  .swap(target.norms  );
		tans   .swap(target.tans   );
		bitans .swap(target.bitans );
		indices.swap(target.indices);
		
		fm::swap(primitive,target.primitive);
		
		return *this;
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
        ret.uvs.resize(ret.pts.size());
        ret.primitive = fg::Triangles;
        ret.indices.resize(W*H*2*3);

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
						ret.indices[indi++] = (x+offsets[i].x)*(H+1) + y+offsets[i].y;
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
		ret.primitive = fg::Triangles;
		ret.indices.resize(W*2*3 + W*(H-1)*2*3);

        fm::Size ptsi = 0;
        fm::Size uvsi = 0;
        fm::Size indi = 0;

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
                ret.indices[indi++] = 0;
                ret.indices[indi++] = i+1;
                ret.indices[indi++] = i+2;
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
                ret.indices[indi++] = W+2;
                ret.indices[indi++] = W+2+i+1;
                ret.indices[indi++] = W+2+i+2;
            }
		}
		
		Cxy(W+1,H)
		{
			float xp = x/float(W);
			float yp = y/float(H-1);
			
			float r = (rfunc ? rfunc(xp,yp) : 1.f);
			
			fm::vec2 p = fm::pol2(radius*r,fm::deg(xp*360));
			ret.pts[ptsi++] = fm::vec3(p.x,height*yp - height/2,p.y);
			
			fm::vec2s inds[] = {fm::vec2s(0,0),fm::vec2s(1,0),fm::vec2s(1,1),
								fm::vec2s(0,0),fm::vec2s(1,1),fm::vec2s(0,1)};
			
			if (x < W && y < H-1)
				C(6)
					ret.indices[indi++] = (W+1)*2+2 + (x+inds[i].x)*H + (y+inds[i].y);
		}
		
		return ret;
	}

	class Comparator
    {
        bool almostEqual(float a,float b)
        {
            return fm::math::abs(a-b) < epsilon;
        }

    public:
    	float epsilon;
    	Comparator(float ep) : epsilon(ep) {}

        bool operator()(const fm::vec3 &l,const fm::vec3 &r)
        {
        	if (almostEqual((l-r).length(),epsilon)) return false;
            if (l.x != r.x) return l.x < r.x;
            if (l.y != r.y) return l.y < r.y;
            if (l.z != r.z) return l.z < r.z;

			return false;
        }
    };
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

		float mnDst = -1;

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

			fm::vec3 v0 = pts[ind1]-pts[ind0], v1 = pts[ind2]-pts[ind0], v2 = pts[ind1]-pts[ind2];

			if (mnDst == -1) mnDst = v0.length();
			mnDst = fm::math::min(mnDst,v0.length());
			mnDst = fm::math::min(mnDst,v1.length());
			mnDst = fm::math::min(mnDst,v2.length());

			fm::vec3 N = (v0).cross(v1);
			norms[ind0] += N;
			norms[ind1] += N;
			norms[ind2] += N;
        }
        // fg::fg_log << "k " << mnDst << std::endl;
//        if (joinSamePts)
            {
                std::map<fm::vec3,std::set<fm::Size>,Comparator > pToInds(Comparator(mnDst*.5));

                C(pts.size())
                    pToInds[pts[i]].insert(i);

                for (std::map<fm::vec3,std::set<fm::Size>,Comparator >::const_iterator it = pToInds.begin();it != pToInds.end();++it)
                {
                    fm::vec3 n;
                    float db = 0;
                    for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
                        n += norms[*pit],
                        ++db;
                    /*
					if (it->second.size() > 1)
					{
						fg::fg_log << it->first << " : ";
       	             for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
							fg::fg_log << *pit << " / ";
						fg::fg_log << std::endl;
					}*/
                    n /= db;

                    for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
                        norms[*pit] = n;
                }
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
