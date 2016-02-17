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
#include <FRONTIER/Graphics/IndexArrayHolder.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>

namespace fg
{
	//////////////////////////////////////////////////////////////////////////
	Assoc::Point operator+(const Assoc::Point &pt,int delta)
	{
		return (Assoc::Point)(((int)pt)+delta);
	}
	
	//////////////////////////////////////////////////////////////////////////
	Assoc::Point operator-(const Assoc::Point &pt,int delta)
	{
		return (Assoc::Point)(((int)pt)-delta);
	}
	
	//////////////////////////////////////////////////////////////////////////
	DrawData::Attribute::Attribute(AssociationPoint type,
								   fm::Size components,
								   fm::Size stride,
								   fm::Size count,
								   unsigned long componentType,
								   fg::Buffer *buf) : type(type),
													  components(components),
													  stride(stride),
													  count(count),
													  componentType(componentType),
													  buf(buf)
	{
		
	}

	//////////////////////////////////////////////////////////////////////////
	DrawData::Attribute::~Attribute()
	{
		delete buf;
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	DrawData::DrawCall::DrawCall(const IndexArrayHolder &indices,
								 fg::Primitive primitive,
								 fm::Size drawBeg,
								 fm::Size drawLen) : primitive(primitive),
													 indexCount(indices.getSize()),
													 drawBeg(drawBeg),
													 drawLen(drawLen),
													 componentType(indices.doUse16bits() ? fg::Is_GLDataType<fm::Uint16>::enumVal : fg::Is_GLDataType<fm::Uint32>::enumVal),
													 buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(indices.getPtr(),(indices.doUse16bits() ? sizeof(fm::Uint16) : sizeof(fm::Uint32)) * indices.getSize());
	}
	
	//////////////////////////////////////////////////////////////////////////
	DrawData::DrawCall::DrawCall(fg::Primitive primitive,
								 fm::Size drawBeg,
								 fm::Size drawLen) : primitive(primitive),
													 indexCount(0),
													 drawBeg(drawBeg),
													 drawLen(drawLen),
													 componentType(0),
													 buf(fm::nullPtr)
	{
		
	}

	//////////////////////////////////////////////////////////////////////////
	DrawData::DrawCall::~DrawCall()
	{
		delete buf;
	}

	//////////////////////////////////////////////////////////////////////////
	DrawData::DrawData()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////
	DrawData::DrawData(const Mesh &m)
	{
		if (m.pts.size())
			attrs.push_back(new Attribute(Assoc::Position,&m.pts[0],m.pts.size()));
			
		if (m.uvs.size())
			attrs.push_back(new Attribute(Assoc::TextureUV,&m.uvs[0],m.uvs.size()));
			
		if (m.norms.size())
			attrs.push_back(new Attribute(Assoc::Normal,&m.norms[0],m.norms.size()));
			
		if (m.tans.size())
			attrs.push_back(new Attribute(Assoc::Tangent,&m.tans[0],m.tans.size()));
			
		if (m.bitans.size())
			attrs.push_back(new Attribute(Assoc::Bitangent,&m.bitans[0],m.bitans.size()));
		
		if (m.indices.size())
			drawCalls.push_back(new DrawCall(IndexArrayHolder(&m.indices[0],m.indices.size()),m.primitive));
		else
			drawCalls.push_back(new DrawCall(m.primitive,0,m.pts.size()));
	}

	//////////////////////////////////////////////////////////////////////////
	DrawData::~DrawData()
	{
		C(attrs.size())
            delete attrs[i];

        C(drawCalls.size())
            delete drawCalls[i];
	}

	//////////////////////////////////////////////////////////////////////////
	DrawData::Attribute *DrawData::operator[](AssociationPoint type)
	{
		C(attrs.size())
			if (attrs[i] && attrs[i]->type == type)
				return attrs[i];
		
		return fm::nullPtr;
	}
	
	//////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::setAttribute(DrawData::Attribute *ptr)
	{
		if (!ptr)
			return *this;
	
		bool found = false;
		
		C(attrs.size())
			if (attrs[i] && attrs[i]->type == ptr->type)
			{
				delete attrs[i];
				
				attrs[i] = ptr;
				
				found = true;
			}
		
		if (!found)
			attrs.push_back(ptr);
		
		return *this;
	}
}