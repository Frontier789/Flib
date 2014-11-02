////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/TlsPtr.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	Drawable::~Drawable() 
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Attribute::Attribute() : m_ptr(NULL),
							 m_buffer(NULL),
							 m_components(0),
							 m_bytesPerComponent(0),
							 m_bytesPerVertex(0)
	{

	}
	
	/////////////////////////////////////////////////////////////
	bool Attribute::isUsed() const
	{
		return (m_components && m_bytesPerComponent && (m_buffer || m_ptr));
	}
	
	/////////////////////////////////////////////////////////////
	GLenum getSizeType(const Attribute &attr)
	{
		if (attr.m_bytesPerComponent == sizeof(GLfloat))
			return GL_FLOAT;
		
		if (attr.m_bytesPerComponent == sizeof(GLshort))
			return GL_SHORT;
		
		if (attr.m_bytesPerComponent == sizeof(GLint))
			return GL_INT;
		
		if (attr.m_bytesPerComponent == sizeof(GLdouble))
			return GL_DOUBLE;

		return 0;
	}
	
	/////////////////////////////////////////////////////////////
	GLenum getSizeType(const IndexPointer &indexptr)
	{
		if (indexptr.m_bytesPerIndex == sizeof(GLbyte))
			return GL_UNSIGNED_BYTE;
		
		if (indexptr.m_bytesPerIndex == sizeof(GLshort))
			return GL_UNSIGNED_SHORT;
		
		if (indexptr.m_bytesPerIndex == sizeof(GLuint))
			return GL_UNSIGNED_INT;

		return 0;
	}
		
	/////////////////////////////////////////////////////////////
	Attribute Attribute::Unused = Attribute();

	/////////////////////////////////////////////////////////////
	IndexPointer::IndexPointer() : m_bytesPerIndex(0),
								   m_buffer(NULL),
								   m_ptr(NULL)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	IndexPointer::IndexPointer(const unsigned char *ptr) : m_bytesPerIndex(sizeof(unsigned char)),
														   m_buffer(NULL),
														   m_ptr(ptr)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	IndexPointer::IndexPointer(const unsigned short *ptr) : m_bytesPerIndex(sizeof(unsigned short)),
															m_buffer(NULL),
															m_ptr(ptr)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	IndexPointer::IndexPointer(const unsigned int *ptr) : m_bytesPerIndex(sizeof(unsigned int)),
														  m_buffer(NULL),
														  m_ptr(ptr)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	IndexPointer::IndexPointer(const void *ptr,unsigned char bytesPerIndex,const fg::Buffer *buffer) : m_bytesPerIndex(bytesPerIndex),
																									   m_buffer(buffer),
																									   m_ptr(ptr)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void priv::defaultDraw(const Attribute &pos,
						   const Attribute &clr,
						   const Attribute &texPos,
						   const Attribute &norm,
						   fm::Size vertexCount,
						   fg::Primitive primitive,
						   const Texture *texture,
						   const Shader *shader,
						   const fm::mat4 &transformation,
						   const IndexPointer &indices)
	{
		if (pos.isUsed())
		{
			fg::Buffer::bind(pos.m_buffer,fg::ArrayBuffer);
			glCheck(glVertexPointer(pos.m_components,getSizeType(pos),pos.m_bytesPerVertex,pos.m_ptr));
		}
	
		if (clr.isUsed())
		{
			fg::Buffer::bind(clr.m_buffer,fg::ArrayBuffer);
			glCheck(glColorPointer(clr.m_components,getSizeType(clr),clr.m_bytesPerVertex,clr.m_ptr));
		}
	
		if (texPos.isUsed())
		{
			fg::Buffer::bind(texPos.m_buffer,fg::ArrayBuffer);
			glCheck(glTexCoordPointer(texPos.m_components,getSizeType(texPos),texPos.m_bytesPerVertex,texPos.m_ptr));
		}
	
		if (norm.isUsed())
		{
			fg::Buffer::bind(norm.m_buffer,fg::ArrayBuffer);
			glCheck(glNormalPointer(getSizeType(norm),norm.m_bytesPerVertex,norm.m_ptr));
		}
		
		fg::Texture::bind(texture,fg::Texture::Pixels);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&transformation.transpose()[0][0]);
		
		if (indices.m_bytesPerIndex && (indices.m_ptr || indices.m_buffer))
		{
			fg::Buffer::bind(indices.m_buffer,fg::IndexBuffer);
			glCheck(glDrawElements(primitive,vertexCount,getSizeType(indices),indices.m_ptr));
		}
		else
			glCheck(glDrawArrays(primitive,0,vertexCount));
	}
	
	fm::TlsPtr<drawFuncType> drawerFuncPtr;
	
	/////////////////////////////////////////////////////////////
	void setDrawFunc(drawFuncType *func)
	{
		drawerFuncPtr = func;
	}
	
	/////////////////////////////////////////////////////////////
	void draw(const Attribute &pos,
			  const Attribute &clr,
			  fm::Size vertexCount,
			  fg::Primitive primitive,
			  const Texture *texture,
			  const Shader *shader,
			  const fm::mat4 &transformation,
			  const IndexPointer &indices)
	{
		draw(pos,
			 clr,
			 Attribute::Unused,
			 Attribute::Unused,
			 vertexCount,
			 primitive,
			 texture,
			 shader,
			 transformation,
			 indices);
	}
	
	
	/////////////////////////////////////////////////////////////
	void draw(const Attribute &pos,
			  const Attribute &clr,
			  const Attribute &texPos,
			  fm::Size vertexCount,
			  fg::Primitive primitive,
			  const Texture *texture,
			  const Shader *shader,
			  const fm::mat4 &transformation,
			  const IndexPointer &indices)
	{
		draw(pos,
			 clr,
			 texPos,
			 Attribute::Unused,
			 vertexCount,
			 primitive,
			 texture,
			 shader,
			 transformation,
			 indices);
	}
	
	
	/////////////////////////////////////////////////////////////
	void draw(const Attribute &pos,
			  const Attribute &clr,
			  const Attribute &texPos,
			  const Attribute &norm,
			  fm::Size vertexCount,
			  fg::Primitive primitive,
			  const Texture *texture,
			  const Shader *shader,
			  const fm::mat4 &transformation,
			  const IndexPointer &indices)
	{
		if (drawerFuncPtr)
			(*drawerFuncPtr)(pos,
							 clr,
							 texPos,
							 norm,
							 vertexCount,
							 primitive,
							 texture,
							 shader,
							 transformation,
							 indices);
		else
			priv::defaultDraw(pos,
							  clr,
							  texPos,
							  norm,
							  vertexCount,
							  primitive,
							  texture,
							  shader,
							  transformation,
							  indices);
	}
}