//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_DRAWABLE_HPP_INCLUDED
#define FRONTIER_DRAWABLE_HPP_INCLUDED
#define FRONTIER_DRAWABLE
#include <cstddef>
namespace fm
{
	template<class,class,class,class>
	class vertex;
}
namespace fg
{
	class Buffer;

	enum Primitive {
		Lines         = 1u,
		Points        = 0u,
		LineLoop      = 2u,
		Triangles     = 4u,
		LineStrip     = 3u,
		TriangleFan   = 6u,
		TriangleStrip = 5u
	};
	namespace priv
	{
		class Attribute
		{
		public:
			fg::Buffer *buf;
			unsigned int dim;
			unsigned int size;
			unsigned int stride;
			const void *ptr;
			template<class T>
			Attribute(const T *ptr,unsigned int stride=0);
			Attribute();
			unsigned int getSizeType() const;
			static Attribute Empty;
		};
		void defaultDraw(const Attribute &pos,bool hasPos,
					     const Attribute &clr,bool hasClr,
					     const Attribute &texPos,bool hasTexPos,
					     const Attribute &norm,bool hasNorm,
					     fg::Primitive primitive,unsigned int vertexCount,void *indices,unsigned int indexSize);
		class IndexPointer
		{
		public:
			unsigned int size;
			void *ptr;
			IndexPointer();
			IndexPointer(unsigned char *ptr);
			IndexPointer(unsigned short *ptr);
			IndexPointer(unsigned int *ptr);
		};

	}
	typedef void (*drawFuncType)(const priv::Attribute &,bool,
				     			 const priv::Attribute &,bool,
				     			 const priv::Attribute &,bool,
				     			 const priv::Attribute &,bool,
				     			 fg::Primitive primitive,unsigned int vertexCount,void *indices,unsigned int indexSize);
	void setDrawFunc(drawFuncType func);
	
	void draw(priv::Attribute pos,priv::Attribute clr,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp=priv::IndexPointer());
	void draw(priv::Attribute pos,priv::Attribute clr,priv::Attribute texPos,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp=priv::IndexPointer());
	void draw(priv::Attribute pos,priv::Attribute clr,priv::Attribute texPos,priv::Attribute norm,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp=priv::IndexPointer());

	template <class pt,class ct,class tpt,class nt>
	void draw(const fm::vertex<pt,ct,tpt,nt> *vertices,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp=priv::IndexPointer());
	template <class pt,class ct,class tpt,class nt,std::size_t S>
	void draw(const fm::vertex<pt,ct,tpt,nt> (&vertices)[S],fg::Primitive primitive,priv::IndexPointer indp=priv::IndexPointer());
	
	template<class T>
	priv::Attribute attr(const T *ptr,unsigned int stride=0);
	template<class T>
	priv::Attribute attr(fg::Buffer &buf,unsigned int offset=0,unsigned int stride=0);

}
#endif //FRONTIER_DRAWABLE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Drawable.inl>
#endif
