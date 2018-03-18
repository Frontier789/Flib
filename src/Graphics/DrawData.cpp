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
#include <FRONTIER/Graphics/IndexHolder.hpp>
#include <FRONTIER/Graphics/AssocPoint.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/DrawCall.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <cstring>

namespace fg
{
    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawData()
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawData(const DrawData &drawData)
	{
		(*this) = drawData;
	}
#endif

    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawData(DrawData &&drawData)
	{
		drawData.swap(*this);
	}
	
    //////////////////////////////////////////////////////////////////////////
	DrawData::~DrawData()
	{
		for (auto it : m_extraAttrs)
			delete it.second;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawData(const Mesh &m)
	{
		(*this) = m;
	}

    //////////////////////////////////////////////////////////////////////////
	Attribute &DrawData::operator[](AssocPoint type)
	{
		return getAttribute(type);
	}

    //////////////////////////////////////////////////////////////////////////
	const Attribute &DrawData::operator[](AssocPoint type) const
	{
		return getAttribute(type);
	}
	
	/////////////////////////////////////////////////////////////
	Attribute &DrawData::getAttribute(AssocPoint type)
	{
		if (type >= Assoc::Custom0)
		{
			Attribute *&ptr = m_extraAttrs[type];
			if (!ptr)
				ptr = new Attribute;
			
			return *ptr;
		}
		
		if (type == Assoc::Color)     return colors;
		if (type == Assoc::Normal)    return normals;
		if (type == Assoc::Tangent)   return tangents;
		if (type == Assoc::Position)  return positions;
		if (type == Assoc::Bitangent) return bitangents;
		if (type == Assoc::TextureUV) return texPositions;
		
		return m_unusedAttr;
	}

	/////////////////////////////////////////////////////////////
	const Attribute &DrawData::getAttribute(AssocPoint type) const
	{
		if (type >= Assoc::Custom0)
		{
			Attribute *&ptr = m_extraAttrs[type];
			if (!ptr)
				ptr = new Attribute;
			
			return *ptr;
		}
		
		if (type == Assoc::Color)     return colors;
		if (type == Assoc::Normal)    return normals;
		if (type == Assoc::Tangent)   return tangents;
		if (type == Assoc::Position)  return positions;
		if (type == Assoc::Bitangent) return bitangents;
		if (type == Assoc::TextureUV) return texPositions;
		
		return m_unusedAttr;
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::operator=(const DrawData &drawData)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		m_drawCalls  = drawData.m_drawCalls;
		m_extraAttrs = drawData.m_extraAttrs;
		colors       = drawData.colors;
		normals      = drawData.normals;
		tangents     = drawData.tangents;
		positions    = drawData.positions;
		bitangents   = drawData.bitangents;
		texPositions = drawData.texPositions;
		
		return *this;
	}
#endif

    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::operator=(DrawData &&drawData)
	{
		return this->swap(drawData);
	}
	
    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::operator=(const Mesh &mesh)
	{
		reset();

		if (mesh.pts.size())
			positions.set(&mesh.pts[0],mesh.pts.size());

		if (mesh.clr.size())
			colors.set(&mesh.clr[0],mesh.clr.size());

		if (mesh.uvs.size())
			texPositions.set(&mesh.uvs[0],mesh.uvs.size());

		if (mesh.norms.size())
			normals.set(&mesh.norms[0],mesh.norms.size());

		if (mesh.tans.size())
			tangents.set(&mesh.tans[0],mesh.tans.size());

		if (mesh.bitans.size())
			bitangents.set(&mesh.bitans[0],mesh.bitans.size());
		
		if (mesh.extras.size())
		{
			int index = 0;
			for (const auto &extra : mesh.extras)
			{
				getAttribute(Assoc::Custom0 + index).set(&extra[0],extra.size());
				++index;
			}
		}
		
		if (mesh.faces.size())
		{
			fm::Size counts[]  = {0,0,0,0,0,0,0};
			fm::Size offsets[] = {0,0,0,0,0,0,0};
			
			C(mesh.faces.size())
			{
				counts[int(mesh.faces[i].primitive)] += mesh.faces[i].indices.size();
			}
			
			std::vector<fm::Uint32> drawIndices[7];
			
			C(sizeof(counts)/sizeof(*counts)) drawIndices[i].resize(counts[i]);
			
			C(mesh.faces.size())
			{
				int id = int(mesh.faces[i].primitive);
				
				std::memcpy(&drawIndices[id][offsets[id]],&mesh.faces[i].indices[0],sizeof(mesh.faces[i].indices[0])*mesh.faces[i].indices.size());
				
				offsets[id] += mesh.faces[i].indices.size();
			}
			
			C(sizeof(counts)/sizeof(*counts))
				if (counts[i])
				{
					m_drawCalls.push_back(DrawCall());
					m_drawCalls.back().set(&drawIndices[i][0],drawIndices[i].size(),fg::Primitive(i));				
				}
			
		}
		else
		{
			m_drawCalls.push_back(DrawCall());
			m_drawCalls.back().set(0,mesh.pts.size(),fg::Triangles);
		}

		return *this;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::remAttr(AssocPoint type)
	{
		getAttribute(type) = Attribute();
		
		return *this;
	}

    //////////////////////////////////////////////////////////////////////////
	bool DrawData::hasAttr(AssocPoint type) const
	{
		return getAttribute(type).buf != nullptr;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawCall &DrawData::getDraw(fm::Size index)
	{
		return m_drawCalls[index];
	}

    //////////////////////////////////////////////////////////////////////////
	const DrawCall &DrawData::getDraw(fm::Size index) const
	{
		return m_drawCalls[index];
	}

    //////////////////////////////////////////////////////////////////////////
	DrawCall &DrawData::addDraw(fg::Primitive primitive)
	{
		m_drawCalls.push_back(DrawCall());
		m_drawCalls.back().primitive = primitive;

		return m_drawCalls.back();
	}

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawData::addDraw(const fg::IndexHolder &inds,fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(inds,primitive);

        return m_drawCalls.back();
    }

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawData::addDraw(fm::Size beg,fm::Size len,fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(beg,len,primitive);

        return m_drawCalls.back();
    }

    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::clearDraws()
	{
		m_drawCalls.clear();
		
		return *this;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::reset()
	{
		return *this = DrawData();
	}

    //////////////////////////////////////////////////////////////////////////
	fm::Size DrawData::getDrawCount() const
	{
		return m_drawCalls.size();
	}
		
	/////////////////////////////////////////////////////////////
	DrawData::reference DrawData::swap(DrawData &drawData)
	{
		m_extraAttrs.swap(drawData.m_extraAttrs);
		m_drawCalls.swap(drawData.m_drawCalls);
		positions.swap(drawData.positions);
		colors.swap(drawData.colors);
		normals.swap(drawData.normals);
		tangents.swap(drawData.tangents);
		bitangents.swap(drawData.bitangents);
		texPositions.swap(drawData.texPositions);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	void DrawData::forEachAttr(fm::Delegate<void,fg::AssocPoint,const Attribute &> func) const
	{
		for (AssocPoint pt = Assoc::Position;pt != AssocPoint::Custom;pt = pt+1)
		{
			auto &attr = getAttribute(pt);
			
			if (attr.buf)
				func(pt,attr);
		}
		
		for (auto &it : m_extraAttrs)
			if (it.second && it.second->buf)
				func(it.first,*it.second);
	}
}
