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
#include <FRONTIER/GL/Is_GLDataType.hpp>
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
    Mesh::AssociationPoint operator+(const Mesh::AssociationPoint &pt,int delta)
    {
        return (Mesh::AssociationPoint)((int)pt + delta);
    }

	/////////////////////////////////////////////////////////////
    Mesh::AssociationPoint operator-(const Mesh::AssociationPoint &pt,int delta)
    {
        return (Mesh::AssociationPoint)((int)pt - delta);
    }

	/////////////////////////////////////////////////////////////
    Mesh::~Mesh()
    {
        reset();
    }

	/////////////////////////////////////////////////////////////
    void Mesh::reset()
    {
        C(attrs.size())
            delete attrs[i];

        C(indices.size())
            delete indices[i];

        attrs.clear();
        indices.clear();
    }


	/////////////////////////////////////////////////////////////
    Mesh::Attribute::Attribute(AssociationPoint type,
                               fm::Size components,
                               fm::Size stride,
                               fm::Size count,
                               unsigned long componentType,
                               const void *ptr,
                               fg::Buffer *buf,
                               bool ownBuffer) : type(type),
                                                 components(components),
                                                 stride(stride),
                                                 count(count),
                                                 componentType(componentType),
                                                 ptr(ptr),
                                                 buf(buf),
                                                 ownBuffer(ownBuffer)
    {

    }

	/////////////////////////////////////////////////////////////
    Mesh::Attribute::~Attribute()
    {
        if (ownBuffer)
            delete buf;
    }

	/////////////////////////////////////////////////////////////
	Mesh::IndexData::IndexData(const IndexArrayHolder &indices,fg::Primitive primitive,bool genBuf) : indexCount(indices.N),
																									  componentType(indices.use16bits ? (unsigned int)Is_GLDataType<fm::Uint16>::enumVal : (unsigned int)Is_GLDataType<fm::Uint32>::enumVal),
																									  ptr(fm::nullPtr),
																									  buf(fm::nullPtr),
																									  ownBuffer(genBuf),
																									  primitive(primitive)
	{
		if (genBuf)
            buf = new fg::Buffer(fg::IndexBuffer),
            ptr = fm::nullPtr,
			buf->setData(indices.ptr,indices.N * (indices.use16bits ? sizeof(fm::Uint16) : sizeof(fm::Uint32)));
        else
            ptr = indices.ptr,
            buf = fm::nullPtr;
	}

	/////////////////////////////////////////////////////////////
    Mesh::IndexData::~IndexData()
    {
        if (ownBuffer)
            delete buf;
    }

	/////////////////////////////////////////////////////////////
    Mesh::Attribute *Mesh::operator[](Mesh::AssociationPoint type)
    {
        C(attrs.size())
            if (attrs[i]->type == type)
                return attrs[i];

        return fm::nullPtr;
    }

	Mesh::IndexArrayHolder::IndexArrayHolder(fm::Size N,bool use16bits) : use16bits(use16bits),
																		  N(N)
	{
		if (use16bits)
			ptr = new fm::Uint16[N];
		else
			ptr = new fm::Uint32[N];
	}
	
	Mesh::IndexArrayHolder::IndexArrayHolder(fm::Size N,fm::Size maxIndex) : use16bits(maxIndex <= 65536),
																			 N(N)
	{
		if (use16bits)
			ptr = new fm::Uint16[N];
		else
			ptr = new fm::Uint32[N];
	}

	Mesh::IndexArrayHolder::~IndexArrayHolder()
	{
		if (use16bits)
			delete[] (fm::Uint16*)ptr;
		else
			delete[] (fm::Uint32*)ptr;
	}

	void Mesh::IndexArrayHolder::set(fm::Size i,fm::Uint32 val)
	{
		if (use16bits)
			((fm::Uint16*)ptr)[i] = val;
		else
			((fm::Uint32*)ptr)[i] = val;
	}

	fm::Uint32 Mesh::IndexArrayHolder::get(fm::Size i)
	{
		if (use16bits)
			return ((fm::Uint16*)ptr)[i];
		else
			return ((fm::Uint32*)ptr)[i];
	}

	/////////////////////////////////////////////////////////////
    Mesh &Mesh::getSphere(Mesh &output,float radius,fm::Size W,fm::Size H,float (*radiusModifier)(float &,float &))
    {
        fm::vec3 *pts = new fm::vec3[(W+1)*H];
        fm::vec2 *tpts = new fm::vec2[(W+1)*H];

		IndexArrayHolder inds(W*(2*H-2)*3, (W+1)*H);

        fm::Size ptsi = 0;
        fm::Size tpti = 0;
        fm::Size indi = 0;

        Cx(W+1)
        {
			Cy(H)
			{
				float xpO = float(x)/(W);
				float ypO = float(y)/(H-1);
				float xp = xpO;
				float yp = ypO;
				float r = (radiusModifier ? radiusModifier(xp,yp) : 1.f);

				fm::vec3 p = fm::pol3(radius*r,fm::deg(xp*360),fm::deg(90-yp*180));

				pts[ptsi++]  = p;
				tpts[tpti++] = fm::vec2(xpO,ypO);

                if (x<W && y>0)
                    inds.set(indi++, (x+0)*H+y-1),
                    inds.set(indi++, (x+0)*H+y+0),
                    inds.set(indi++, (x+1)*H+y+0);

                if (x<W && y<H-1)
                    inds.set(indi++, (x+1)*H+y+0),
                    inds.set(indi++, (x+0)*H+y+0),
                    inds.set(indi++, (x+1)*H+y+1);
			}
		}

        output.attrs.push_back(new Mesh::Attribute(Mesh::Position,pts,(W+1)*H,true));
        output.attrs.push_back(new Mesh::Attribute(Mesh::TextureUV,tpts,(W+1)*H,true));

        if (inds.use16bits)
            output.indices.push_back(new Mesh::IndexData((fm::Uint16*)inds.ptr,inds.N,fg::Triangles,true));
        else
            output.indices.push_back(new Mesh::IndexData((fm::Uint32*)inds.ptr,inds.N,fg::Triangles,true));

		delete[] pts;
		delete[] tpts;

        return output;
    }

	/////////////////////////////////////////////////////////////
    Mesh &Mesh::getTorus(Mesh &output,float majorR,float minorR,fm::Size W,fm::Size H,float (*radiusModifier)(float &,float &))
    {
		fm::vec3 *pts = new fm::vec3[(W+1)*(H+1)];
        fm::vec2 *tpts = new fm::vec2[(W+1)*(H+1)];

		IndexArrayHolder inds(W*(H+1)*2*3, (W+1)*(H+1));

        fm::Size ptsi = 0;
        fm::Size tpti = 0;
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
				float r = (radiusModifier ? radiusModifier(xp,yp) : 1.f);

                fm::vec3 majorP = fm::pol3(majorR,fm::deg(0),fm::deg(xp*360));
                fm::vec3 f = majorP.sgn();

				fm::vec2 minorP = fm::pol2(minorR*r,fm::deg(yp*360));

				fm::vec3 p = majorP + f*minorP.x + u*minorP.y;

				pts[ptsi++]  = p;
				tpts[tpti++] = fm::vec2(xpO,ypO);

				fm::vec2i offsets[] = {fm::vec2i(0,0),fm::vec2i(1,0),fm::vec2i(1,1),
									   fm::vec2i(0,0),fm::vec2i(1,1),fm::vec2i(0,1)};

				if (x < W)
					C(6)
						inds.set(indi++,  (x+offsets[i].x)*(H+1) + (y+offsets[i].y)%(H+1)  );
			}
		}

        output.attrs.push_back(new Mesh::Attribute(Mesh::Position,pts,(W+1)*(H+1),true));
        output.attrs.push_back(new Mesh::Attribute(Mesh::TextureUV,tpts,(W+1)*(H+1),true));

        if (inds.use16bits)
            output.indices.push_back(new Mesh::IndexData((fm::Uint16*)inds.ptr,inds.N,fg::Triangles,true));
        else
            output.indices.push_back(new Mesh::IndexData((fm::Uint32*)inds.ptr,inds.N,fg::Triangles,true));

		delete[] pts;
		delete[] tpts;

        return output;
    }
/*
	/////////////////////////////////////////////////////////////
    Mesh &Mesh::getCube(Mesh &output,float size,fm::Size N,float (*radiusModifier)(float,float))
    Mesh &Mesh::getCylinder(Mesh &output,float radius,float height,fm::Size W,fm::Size H,float (*radiusModifier)(float,float))
    Mesh &Mesh::getCone(Mesh &output,float radius,float height,fm::Size N,float (*radiusModifier)(float,float))*/

	/////////////////////////////////////////////////////////////
	fm::Uint32 getIndex(const void *ptr,fm::Size index,bool use16bits)
	{
		if (use16bits) return ((const fm::Uint16*)ptr)[index];
		return ((const fm::Uint32*)ptr)[index];
	}

    class Comparator
    {
        bool almostEqual(float a,float b,float epsilon = .000001)
        {
            return fm::math::abs(a-b) < epsilon;
        }

    public:
        bool operator()(const fm::vec3 &l,const fm::vec3 &r)
        {
            if (!almostEqual(l.x,r.x)) return l.x < r.x;
            if (!almostEqual(l.y,r.y)) return l.y < r.y;
            if (!almostEqual(l.z,r.z)) return l.z < r.z;

			return false;
        }
    };

	/////////////////////////////////////////////////////////////
    Mesh &Mesh::calcNormals(Mesh &mesh,bool joinSamePts)
    {
    	Mesh::Attribute *attrp = mesh[Mesh::Position];

		if (attrp && attrp->components == 3 && attrp->componentType == Is_GLDataType<float>::enumVal)
		{
			const fm::vec3 *datap = (const fm::vec3 *)attrp->buf->map(true,false);
			fm::Size N = attrp->count;

			fm::vec3 *norms = new fm::vec3[N];

			C(mesh.indices.size())
			{
				fg::Buffer *bufi = mesh.indices[i]->buf;
				const void *datai = (const void *)bufi->map(true,false);
				bool use16bits = (mesh.indices[i]->componentType == Is_GLDataType<fm::Uint16>::enumVal);

				fm::Uint32 ind0;
				fm::Uint32 ind1;
				fm::Uint32 ind2;

				Cx(mesh.indices[i]->indexCount-2)
				{
					if (mesh.indices[i]->primitive == fg::Triangles)
					{
						ind0 = getIndex(datai,x,use16bits),
						ind1 = getIndex(datai,x+1,use16bits),
						ind2 = getIndex(datai,x+2,use16bits),
						x += 2;
					}
					else if (mesh.indices[i]->primitive == fg::TriangleStrip)
					{
						ind0 = getIndex(datai,x,use16bits),
						ind1 = getIndex(datai,x+1,use16bits),
						ind2 = getIndex(datai,x+2,use16bits);
					}
					else if (mesh.indices[i]->primitive == fg::TriangleFan)
					{
						ind0 = getIndex(datai,0,use16bits),
						ind1 = getIndex(datai,x+1,use16bits),
						ind2 = getIndex(datai,x+2,use16bits);
					}
					else break;

					fm::vec3 p0 = datap[ind0];

					fm::vec3 v1 = datap[ind1]-p0;
					fm::vec3 v2 = datap[ind2]-p0;

					fm::vec3 n = v1.cross(v2); /// don't normalize (magnitude = 2*area)

					norms[ind0] += n;
					norms[ind1] += n;
					norms[ind2] += n;
				}

				bufi->unMap();
			}

            if (joinSamePts)
            {
                std::map<fm::vec3,std::set<fm::Size>,Comparator > pToInds;

                C(N)
                    pToInds[datap[i]].insert(i);

                for (std::map<fm::vec3,std::set<fm::Size>,Comparator >::const_iterator it = pToInds.begin();it != pToInds.end();++it)
                {
                    fm::vec3 n;
                    float db = 0;
                    for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
                        n += norms[*pit],
                        ++db;

                    n /= db;

                    for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
                        norms[*pit] = n;
                }
            }

            C(N)
                norms[i] = norms[i].sgn();

			mesh.attrs.push_back(new Mesh::Attribute(Mesh::Normal,norms,N,true));

			delete[] norms;

			attrp->buf->unMap();
		}

		return mesh;
    }

	/////////////////////////////////////////////////////////////
    Mesh &Mesh::calcTangents(Mesh &mesh,bool joinSamePts)
    {
    	Mesh::Attribute *attrp = mesh[Mesh::Position];
    	Mesh::Attribute *attrt = mesh[Mesh::TextureUV];

		if (attrp && attrp->components == 3 && attrp->componentType == Is_GLDataType<float>::enumVal &&
            attrt && attrt->components == 2 && attrt->componentType == Is_GLDataType<float>::enumVal)
		{
			const fm::vec3 *datap = (const fm::vec3 *)attrp->buf->map(true,false);
			const fm::vec2 *datat = (const fm::vec2 *)attrt->buf->map(true,false);
			fm::Size N = attrp->count;

			fm::vec3 *tans = new fm::vec3[N];
			fm::vec3 *bitans = new fm::vec3[N];

			C(mesh.indices.size())
			{
                fg::Buffer *bufi = mesh.indices[i]->buf;
				const void *datai = (const void *)bufi->map(true,false);
				bool use16bits = (mesh.indices[i]->componentType == Is_GLDataType<fm::Uint16>::enumVal);

				fm::Uint32 ind0;
				fm::Uint32 ind1;
				fm::Uint32 ind2;

				Cx(mesh.indices[i]->indexCount-2)
				{
					if (mesh.indices[i]->primitive == fg::Triangles)
					{
						ind0 = getIndex(datai,x,use16bits),
						ind1 = getIndex(datai,x+1,use16bits),
						ind2 = getIndex(datai,x+2,use16bits),
						x += 2;
					}
					else if (mesh.indices[i]->primitive == fg::TriangleStrip)
					{
						ind0 = getIndex(datai,x,use16bits),
						ind1 = getIndex(datai,x+1,use16bits),
						ind2 = getIndex(datai,x+2,use16bits);
					}
					else if (mesh.indices[i]->primitive == fg::TriangleFan)
					{
						ind0 = getIndex(datai,0,use16bits),
						ind1 = getIndex(datai,x+1,use16bits),
						ind2 = getIndex(datai,x+2,use16bits);
					}
					else break;

					fm::vec3 p0 = datap[ind0];

					fm::vec3 v1 = datap[ind1]-p0;
					fm::vec3 v2 = datap[ind2]-p0;

					fm::vec2 t0 = datat[ind0];

					fm::vec2 s1 = datat[ind1]-t0;
					fm::vec2 s2 = datat[ind2]-t0;

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

				bufi->unMap();
			}

            if (joinSamePts)
            {
                std::map<fm::vec3,std::set<fm::Size>,Comparator > pToInds;

                C(N)
                    pToInds[datap[i]].insert(i);

                for (std::map<fm::vec3,std::set<fm::Size>,Comparator >::const_iterator it = pToInds.begin();it != pToInds.end();++it)
                {
                    fm::vec3 ta,bita;
                    float db = 0;
                    for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
                        ta += tans[*pit],
                        bita += bitans[*pit],
                        ++db;

                    ta /= db;
                    bita /= db;

                    for (std::set<fm::Size>::const_iterator pit = it->second.begin();pit != it->second.end();++pit)
                        tans[*pit] = ta,
                        bitans[*pit] = bita;
                }
            }

			C(N)
                tans[i] = tans[i].sgn(),
                bitans[i] = bitans[i].sgn();

			mesh.attrs.push_back(new Mesh::Attribute(Mesh::Tangent,tans,N,true));
			mesh.attrs.push_back(new Mesh::Attribute(Mesh::Bitangent,bitans,N,true));

			delete[] tans;
			delete[] bitans;

			attrp->buf->unMap();
			attrt->buf->unMap();
		}

		return mesh;
    }
}
