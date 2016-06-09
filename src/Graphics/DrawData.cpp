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
	    return (Assoc::Point)((int)pt + delta);
	}

    //////////////////////////////////////////////////////////////////////////
	Assoc::Point operator-(const Assoc::Point &pt,int delta)
	{
	    return (Assoc::Point)((int)pt - delta);
	}

    //////////////////////////////////////////////////////////////////////////
    DrawData::Attribute::Attribute(fm::Size components,
                                   fm::Size stride,
                                   fm::Size count,
                                   fm::Size componentType,
                                   fg::Buffer *buffer,
                                   bool ownBuffer) : components(components),
                                                     stride(stride),
                                                     count(count),
                                                     componentType(componentType),
                                                     buf(buffer ? buffer : new fg::Buffer),
                                                     ownBuffer(buffer ? ownBuffer : true)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::Attribute::Attribute(const DrawData::Attribute &attr) : buf(fm::nullPtr),
																	  ownBuffer(false)
    {
        (*this) = attr;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::Attribute &DrawData::Attribute::operator=(const DrawData::Attribute &attr)
    {
        components = attr.components;
        stride = attr.stride;
        count = attr.count;
        componentType = attr.componentType;

        if (attr.buf)
        {
            if (!ownBuffer || !buf) buf = new fg::Buffer;
            (*buf) = (*attr.buf);
            ownBuffer = true;
        }
        else
        {
            if (ownBuffer) delete buf;
            buf = fm::nullPtr;
            ownBuffer = false;
        }

        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::Attribute &DrawData::Attribute::set(fm::Size components,
                                                  fm::Size stride,
                                                  fm::Size count,
                                                  fm::Size componentType,
                                                  const void *pointer,
                                                  fm::Size bytesToCopy)
    {
        this->components = components;
        this->stride = stride;
        this->count = count;
        this->componentType = componentType;
        if (!ownBuffer || !buf)
        {
            ownBuffer = true;
            buf = new fg::Buffer;
        }

        buf->setData(pointer,bytesToCopy);

        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::Attribute &DrawData::Attribute::set(fm::Size components,
                                                  fm::Size stride,
                                                  fm::Size count,
                                                  fm::Size componentType,
                                                  fg::Buffer *buf,
                                                  bool ownBuffer)
    {
        this->components = components;
        this->stride = stride;
        this->count = count;
        this->componentType = componentType;

        if (ownBuffer) delete buf;

        this->buf = buf;
        this->ownBuffer = ownBuffer;

        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::Attribute::~Attribute()
    {
        if (ownBuffer)
            delete buf;
    }


    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall::DrawCall(fg::Primitive primitive,
                                 fm::Size indexCount,
                                 fm::Size drawBeg,
                                 fm::Size drawLen,
                                 fm::Size compType,
                                 fg::Buffer *buffer,
                                 bool ownBuffer) : primitive(primitive),
                                                   indexCount(indexCount),
                                                   drawBeg(drawBeg),
                                                   drawLen(drawLen),
                                                   componentType(compType),
                                                   buf(buffer ? buffer : new fg::Buffer(fg::IndexBuffer)),
                                                   ownBuffer(buffer ? ownBuffer : true)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall::DrawCall(const DrawData::DrawCall &drawCall) : buf(fm::nullPtr),
                                                                       ownBuffer(false)
    {
        (*this) = drawCall;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::DrawCall::operator=(const DrawData::DrawCall &drawCall)
    {
        primitive = drawCall.primitive;
        indexCount = drawCall.indexCount;
        drawBeg = drawCall.drawBeg;
        drawLen = drawCall.drawLen;
        componentType = drawCall.componentType;

        if (drawCall.buf)
        {
            if (!ownBuffer || !buf) buf = new fg::Buffer(fg::IndexBuffer);
            (*buf) = (*drawCall.buf);
            ownBuffer = true;
        }
        else
        {
            if (ownBuffer) delete buf;
            buf = fm::nullPtr;
            ownBuffer = false;
        }

		return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::DrawCall::operator=(const IndexArrayHolder &indices)
    {
        return set(indices,fg::Triangles);
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::DrawCall::set(const void *ptr,fm::Size indCount,fm::Size compType,fm::Size bytesToCopy,fg::Primitive primitive)
    {
        this->primitive = primitive;
        indexCount = indCount;
        drawBeg = 0;
        drawLen = indCount;
        componentType = compType;
        if (!ownBuffer || !buf)
        {
            ownBuffer = true;
            buf = new fg::Buffer(fg::IndexBuffer);
        }

        buf->setData(ptr,bytesToCopy);

		return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::DrawCall::set(const IndexArrayHolder &indices,fg::Primitive primitive)
    {
        if (indices.doUse16bits())
            return set((fm::Uint16*)indices.getPtr(),indices.getSize(),primitive);

        return set((fm::Uint32*)indices.getPtr(),indices.getSize(),primitive);
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::DrawCall::set(fm::Size beg,fm::Size len,fg::Primitive primitive)
    {
        this->primitive = primitive;
        indexCount = 0;
        componentType = 0;

        if (ownBuffer) delete buf;
        buf = fm::nullPtr;
        ownBuffer = false;

        drawBeg = beg;
        drawLen = len;

		return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::DrawCall::setRange(fm::Size beg,fm::Size len)
    {
        drawBeg = beg;
        drawLen = len;

        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall::~DrawCall()
    {
        if (ownBuffer)
            delete buf;
    }

    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawData()
	{

	}

    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawData(const Mesh &m)
	{
		(*this) = m;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData::Attribute &DrawData::operator[](AssociationPoint type)
	{
		return m_attrs[type];
	}

    //////////////////////////////////////////////////////////////////////////
	const DrawData::Attribute &DrawData::operator[](AssociationPoint type) const
	{
		return m_attrs.find(type)->second;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::operator=(const Mesh &mesh)
	{
		reset();

		if (mesh.pts.size())
			m_attrs[Assoc::Position].set(&mesh.pts[0],mesh.pts.size());

		if (mesh.uvs.size())
			m_attrs[Assoc::TextureUV].set(&mesh.uvs[0],mesh.uvs.size());

		if (mesh.norms.size())
			m_attrs[Assoc::Normal].set(&mesh.norms[0],mesh.norms.size());

		if (mesh.tans.size())
			m_attrs[Assoc::Tangent].set(&mesh.tans[0],mesh.tans.size());

		if (mesh.bitans.size())
			m_attrs[Assoc::Bitangent].set(&mesh.bitans[0],mesh.bitans.size());

		if (mesh.indices.size())
		{
			m_drawCalls.push_back(DrawCall());
			m_drawCalls.back().set(&mesh.indices[0],mesh.indices.size(),mesh.primitive);
		}
		else
		{
			m_drawCalls.push_back(DrawCall());
			m_drawCalls.back().set(0,mesh.pts.size(),mesh.primitive);
		}

		return *this;
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData &DrawData::remAttr(AssociationPoint type)
	{
		m_attrs.erase(type);
		return *this;
	}

    //////////////////////////////////////////////////////////////////////////
	bool DrawData::hasAttr(AssociationPoint type) const
	{
		return m_attrs.find(type) != m_attrs.end();
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawCall &DrawData::getDraw(fm::Size index)
	{
		return m_drawCalls[index];
	}

    //////////////////////////////////////////////////////////////////////////
	const DrawData::DrawCall &DrawData::getDraw(fm::Size index) const
	{
		return m_drawCalls[index];
	}

    //////////////////////////////////////////////////////////////////////////
	DrawData::DrawCall &DrawData::addDraw(fg::Primitive primitive)
	{
		m_drawCalls.push_back(DrawCall());
		m_drawCalls.back().primitive = primitive;

		return m_drawCalls.back();
	}

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::addDraw(const fg::IndexArrayHolder &inds,fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(inds,primitive);

        return m_drawCalls.back();
    }

    //////////////////////////////////////////////////////////////////////////
    DrawData::DrawCall &DrawData::addDraw(fm::Size beg,fm::Size len,fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(beg,len,primitive);

        return m_drawCalls.back();
    }

    //////////////////////////////////////////////////////////////////////////
	void DrawData::reset()
	{
		m_attrs.clear();
		m_drawCalls.clear();
	}

    //////////////////////////////////////////////////////////////////////////
	fm::Size DrawData::getDrawCount() const
	{
		return m_drawCalls.size();
	}
}
