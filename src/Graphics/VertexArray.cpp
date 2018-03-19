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
#include <FRONTIER/Graphics/VertexArray.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/GL/GLBindKeeper.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	fm::Result VertexArray::create()
	{
		if (!getGlId())
			return glCheck(glGenVertexArrays(1,&getGlId()));
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	VertexArray::VertexArray(VertexArray &&mv)
	{
		mv.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	VertexArray &VertexArray::swap(VertexArray &state)
	{
		m_attrsEnabled.swap(state.m_attrsEnabled);
		std::swap(getGlId(),state.getGlId());
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexArray::clearData()
	{
		fm::Result res;
		
		if (getGlId())
		{
			for (fm::Size i=0;i<m_attrsEnabled.size();++i)
			{
				if (m_attrsEnabled[i])
					res += glCheck(glDisableVertexAttribArray(i));
			}
			
			m_attrsEnabled.clear();
		}
		
		return res;
	}

	////////////////////////////////////////////////////////////
	VertexArray::~VertexArray()
	{
		if (getGlId())
			glCheck(glDeleteVertexArrays(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	fm::Result VertexArray::bind() const
	{
		return glCheck(glBindVertexArray(getGlId()));
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexArray::setAttributeBound(fm::Size attrId,const Attribute &attr)
	{
		fm::Result res = fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
		
		if (attr.buf)
		{
			if (m_attrsEnabled.size() <= attrId) m_attrsEnabled.resize(attrId+1,false);
			
			if (!m_attrsEnabled[attrId])
				res += glCheck(glEnableVertexAttribArray(attrId));
			
			res += glCheck(glVertexAttribPointer(attrId, attr.compCount, attr.compType, GL_FALSE, attr.stride, (void*)attr.offset));
			res += glCheck(glVertexAttribDivisor(attrId, attr.instancesPerUpdate));
			
			m_attrsEnabled[attrId] = true;
		}
		else if (m_attrsEnabled.size() > attrId && m_attrsEnabled[attrId])
		{
			m_attrsEnabled[attrId] = false;
			res += glCheck(glDisableVertexAttribArray(attrId));
		}
		
		return res;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexArray::setAttribute(fm::Size attrId,const Attribute &attr)
	{
		GLBindKeeper guard(glBindVertexArray,GL_VERTEX_ARRAY_BINDING,getGlId());
		
		return setAttributeBound(attrId,attr);
	}

	////////////////////////////////////////////////////////////
	fm::Result VertexArray::bind(fm::Ref<const VertexArray> vao)
	{
		if (vao)
			return vao->bind();
		else
			return glCheck(glBindVertexArray(0));
	}

	////////////////////////////////////////////////////////////
	bool VertexArray::isAvailable()
	{
		return ::priv::so_loader.getProcAddr("glGenVertexArrays") != nullptr;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexArray::setAttributes(const DrawData &drawData,fm::Delegate<fm::Size,fg::AssocPoint> assocToAttrId)
	{
		GLBindKeeper guard(glBindVertexArray,GL_VERTEX_ARRAY_BINDING,getGlId());
		
		fm::Result res;
		std::vector<bool> attrsEnabled;
		
		drawData.forEachAttr([&](fg::AssocPoint pt,const Attribute &attr) {
			
			fm::Size id = assocToAttrId(pt);
			
			if (id == fm::Size(-1))
				return;
			
			if (attrsEnabled.size() <= id) attrsEnabled.resize(id+1,false);
			attrsEnabled[id] = true;
			
			res += setAttributeBound(id,attr);
		});
		
		for (fm::Size i=0;i<m_attrsEnabled.size();++i)
		{
			bool prev = m_attrsEnabled[i];
			bool cur  = (i < attrsEnabled.size() ? attrsEnabled[i] : false);
			
			if (prev && !cur)
				res += setAttributeBound(i,nullptr);
		}
		
		m_attrsEnabled.swap(attrsEnabled);
		
		return res;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result VertexArray::setAttribute(fm::Size attrId,fm::Size compCount,fm::Size compType,const void *ptr)
	{
		GLBindKeeper guard(glBindVertexArray,GL_VERTEX_ARRAY_BINDING,getGlId());
		
		if (m_attrsEnabled.size() > attrId && m_attrsEnabled[attrId])
		{
			m_attrsEnabled[attrId] = false;
			fm::Result res = glCheck(glDisableVertexAttribArray(attrId));
			
			if (!res) return res;
		}
		
		
		#ifndef FRONTIER_VERTEX_ARR_SETATTR_GEN
			#define FRONTIER_VERTEX_ARR_SETATTR(n,Q,q,type) if (compCount == n) return glCheck(glVertex##Q##n##q##v(attrId,(const GL##type*)ptr))
			#define FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_TYPE,Q,q,type) \
			if (compType == GL_TYPE)                                  \
			{                                                         \
				FRONTIER_VERTEX_ARR_SETATTR(1,Q,q,type);              \
				FRONTIER_VERTEX_ARR_SETATTR(2,Q,q,type);              \
				FRONTIER_VERTEX_ARR_SETATTR(3,Q,q,type);              \
				FRONTIER_VERTEX_ARR_SETATTR(4,Q,q,type);              \
			}
		#endif
		
		int t[4] = {0,0,0,1};
		
		if (compType == GL_BYTE || compType == GL_UNSIGNED_BYTE)
		{
			for (fm::Size i=0;i<compCount;++i) t[i] = ((char*)ptr)[i];
			compType = GL_INT;
			ptr = (void*)t;
		}
		
		FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_FLOAT,Attrib,f,float)
		FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_SHORT,Attrib,s,short)
		FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_UNSIGNED_SHORT,Attrib,s,short)
		FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_DOUBLE,Attrib,d,double)
		FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_INT,AttribI,i,int)
		FRONTIER_VERTEX_ARR_SETATTR_GEN(GL_UNSIGNED_INT,AttribI,ui,uint)
		
		return fm::Result();
	}
}
