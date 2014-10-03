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
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	namespace priv
	{
		Attribute::Attribute() : buf(NULL),
					  			 dim(0),
					  			 size(0),
					  			 stride(0),
					  			 ptr(NULL)
		{
	
		}
		unsigned int Attribute::getSizeType() const
		{
			#ifdef GL_FLOAT
			if (size==sizeof(GLfloat))
				return GL_FLOAT;
			#endif
	
			#ifdef GL_SHORT
			if (size==sizeof(GLshort))
				return GL_SHORT;
			#endif
	
			#ifdef GL_INT
			if (size==sizeof(GLint))
				return GL_INT;
			#endif
	
			#ifdef GL_DOUBLE
			if (size==sizeof(GLdouble))
				return GL_DOUBLE;
			#endif
	
			return 0;
		}

		Attribute Attribute::Empty = Attribute();

		void defaultDraw(const Attribute &pos,bool hasPos,
					  	 const Attribute &clr,bool hasClr,
					  	 const Attribute &texPos,bool hasTexPos,
					  	 const Attribute &norm,bool hasNorm,
					  	 fg::Primitive primitive,unsigned int vertexCount,void *indices,unsigned int indexSize)
		{
			bool inited=false;
			if (!inited)
				glCheck(glEnableClientState(GL_VERTEX_ARRAY)),
				glCheck(glEnableClientState(GL_COLOR_ARRAY)),
				glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY)),
				inited = true;
				
			if (hasPos)
			{
				fg::Buffer::bind(pos.buf,fg::ArrayBuffer);
				if (pos.dim && pos.size && (pos.buf || pos.ptr))
					glCheck(glVertexPointer(pos.dim,pos.getSizeType(),pos.stride,pos.ptr));
			}
		
			if (hasClr)
			{
				fg::Buffer::bind(clr.buf,fg::ArrayBuffer);
				if (clr.dim && clr.size && (clr.buf || clr.ptr))
					glCheck(glColorPointer(clr.dim,clr.getSizeType(),clr.stride,clr.ptr));
			}
		
			if (hasTexPos)
			{
				fg::Buffer::bind(texPos.buf,fg::ArrayBuffer);
				if (texPos.dim && texPos.size && (texPos.buf || texPos.ptr))
					glCheck(glTexCoordPointer(texPos.dim,texPos.getSizeType(),texPos.stride,texPos.ptr));
			}
		
			if (hasNorm)
			{
				fg::Buffer::bind(norm.buf,fg::ArrayBuffer);
				if (norm.dim && norm.size && (norm.buf || norm.ptr))
					glCheck(glNormalPointer(norm.getSizeType(),norm.stride,norm.ptr));
			}
			if (indices && indexSize)
				glCheck(glDrawElements(primitive,vertexCount,indexSize==sizeof(GLbyte) ? GL_UNSIGNED_BYTE : (indexSize==sizeof(GLshort) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),indices));
			else
				glCheck(glDrawArrays(primitive,0,vertexCount));
		}
		IndexPointer::IndexPointer() : size(0u),
						 			   ptr(NULL)
		{

		}
		IndexPointer::IndexPointer(unsigned char *ptr) : size(sizeof(GLbyte)),
										   				 ptr(ptr)
		{
			
		}
		IndexPointer::IndexPointer(unsigned short *ptr) : size(sizeof(GLshort)),
										    			  ptr(ptr)
		{
			
		}
		IndexPointer::IndexPointer(unsigned int *ptr) : size(sizeof(GLint)),
										  				ptr(ptr)
		{
			
		}
		drawFuncType drawFunc = defaultDraw;
	}
	void draw(priv::Attribute pos,priv::Attribute clr,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp)
	{
		priv::drawFunc(pos,true,clr,true,priv::Attribute(),false,priv::Attribute(),false,primitive,vertexCount,indp.ptr,indp.size);
	}

	void draw(priv::Attribute pos,priv::Attribute clr,priv::Attribute texPos,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp)
	{
		priv::drawFunc(pos,true,clr,true,texPos,true,priv::Attribute(),false,primitive,vertexCount,indp.ptr,indp.size);
	}

	void draw(priv::Attribute pos,priv::Attribute clr,priv::Attribute texPos,priv::Attribute norm,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp)
	{
		priv::drawFunc(pos,true,clr,true,texPos,true,norm,true,primitive,vertexCount,indp.ptr,indp.size);
	}
}