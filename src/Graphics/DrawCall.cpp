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
#include <FRONTIER/Graphics/DrawCall.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/util/C.hpp>

namespace fg
{
    //////////////////////////////////////////////////////////////////////////
    DrawCall::DrawCall(fg::Primitive primitive,
                       fm::Size indexCount,
                       fm::Size drawBeg,
                       fm::Size drawLen,
                       fm::Size compType,
                       fg::Buffer *buffer,
                       bool ownBuffer,
                       fm::Size instances) : primitive(primitive),
                                             componentType(compType),
                                             indexCount(indexCount),
											 instances(instances),
                                             drawBeg(drawBeg),
                                             drawLen(drawLen),
                                             buf(buffer ? buffer : new fg::Buffer(fg::IndexBuffer)),
											 ownBuffer(buffer ? ownBuffer : true)
    {

    }

#ifndef FRONTIER_HEAVYCOPY_FORBID
    //////////////////////////////////////////////////////////////////////////
    DrawCall::DrawCall(const DrawCall &drawCall) : buf(nullptr),
                                                   ownBuffer(false)
    {
        (*this) = drawCall;
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    DrawCall::DrawCall(DrawCall &&drawCall) : buf(nullptr),
                                              ownBuffer(false)
    {
        drawCall.swap(*this);
    }

#ifndef FRONTIER_HEAVYCOPY_FORBID
    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::operator=(const DrawCall &drawCall)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
        componentType = drawCall.componentType;
        indexCount    = drawCall.indexCount;
		primitive     = drawCall.primitive;
		drawBeg       = drawCall.drawBeg;
		drawLen       = drawCall.drawLen;
		instances     = drawCall.instances; 
		
		if (ownBuffer) delete buf;
		
		ownBuffer = drawCall.ownBuffer;
		
        if (drawCall.buf)
        {
            if (ownBuffer) 
            {
            	buf = new fg::Buffer(fg::IndexBuffer);
				(*buf) = (*drawCall.buf);
            }
			else buf = drawCall.buf;
        }
		else buf = nullptr;

		return *this;
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::operator=(DrawCall &&drawCall)
	{
		return this->swap(drawCall);
    }
    
	/////////////////////////////////////////////////////////////
	DrawCall &DrawCall::swap(DrawCall &drawCall)
	{
        std::swap(componentType,drawCall.componentType);
        std::swap(indexCount,drawCall.indexCount);
		std::swap(primitive ,drawCall.primitive);
        std::swap(ownBuffer,drawCall.ownBuffer);
        std::swap(instances,drawCall.instances);
        std::swap(drawBeg,drawCall.drawBeg);
        std::swap(drawLen,drawCall.drawLen);
        std::swap(buf,drawCall.buf);
        
		return *this;
	}

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::operator=(const IndexHolder &indices)
    {
        return set(indices,fg::Triangles);
    }

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::set(const void *ptr,fm::Size indCount,fm::Size compType,fm::Size bytesToCopy,fg::Primitive primitive,fm::Size instances)
    {
        this->primitive = primitive;
		componentType   = compType;
		indexCount      = indCount;
		drawLen         = indCount;
		drawBeg         = 0;
		this->instances = instances;
		
        if (!ownBuffer || !buf)
        {
            ownBuffer = true;
            buf = new fg::Buffer(fg::IndexBuffer);
        }

        buf->setData(ptr,bytesToCopy);

		return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::set(const IndexHolder &indices,fg::Primitive primitive,fm::Size instances)
    {
        if (indices.doUse16bits())
            return set((fm::Uint16*)indices.getPtr(),indices.getSize(),primitive,instances);

        return set((fm::Uint32*)indices.getPtr(),indices.getSize(),primitive,instances);
    }

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::set(fm::Size beg,fm::Size len,fg::Primitive primitive,fm::Size instances)
    {
        this->primitive = primitive;
		componentType   = 0;
		indexCount      = 0;
		this->instances = instances;

        if (ownBuffer) delete buf;
        ownBuffer = false;
        buf = nullptr;

        drawBeg = beg;
        drawLen = len;

		return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawCall &DrawCall::setRange(fm::Size beg,fm::Size len)
    {
        drawBeg = beg;
        drawLen = len;

        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    DrawCall::~DrawCall()
    {
        if (ownBuffer)
            delete buf;
    }
}
