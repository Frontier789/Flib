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
#ifndef FRONTIER_DRAWABLE_INL_INCLUDED
#define FRONTIER_DRAWABLE_INL_INCLUDED
namespace fg
{
	namespace priv
	{
		template<class T>
		Attribute::Attribute(const T *ptr,unsigned int stride) : buf(NULL),
																 dim(T::dimensions),
																 size(sizeof(typename T::value_type)),
																 stride(stride),
																 ptr(ptr)
		{
	
		}
	}

	template <class pt,class ct,class tpt,class nt>
	void draw(const fm::vertex<pt,ct,tpt,nt> *vertices,unsigned int vertexCount,fg::Primitive primitive,priv::IndexPointer indp)
	{
		if (nt::dimensions)
			draw(priv::Attribute((pt*)((unsigned char*)vertices),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 priv::Attribute((ct*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 priv::Attribute((tpt*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize+fm::vertex<pt,ct,tpt,nt>::clrSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 priv::Attribute((nt*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize+fm::vertex<pt,ct,tpt,nt>::clrSize+fm::vertex<pt,ct,tpt,nt>::texPosSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 vertexCount,primitive,indp);
		else if (tpt::dimensions)
			draw(priv::Attribute((pt*)((unsigned char*)vertices),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 priv::Attribute((ct*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 priv::Attribute((tpt*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize+fm::vertex<pt,ct,tpt,nt>::clrSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 vertexCount,primitive,indp);
		else 
			draw(priv::Attribute((pt*)((unsigned char*)vertices),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 priv::Attribute((ct*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 vertexCount,primitive,indp);
	}

	template <class pt,class ct,class tpt,class nt,std::size_t S>
	void draw(const fm::vertex<pt,ct,tpt,nt> (&vertices)[S],fg::Primitive primitive,priv::IndexPointer indp)
	{
		draw(vertices,S,primitive,indp);
	}

	template<class T>
	priv::Attribute attr(const T *ptr,unsigned int stride=0)
	{
		return priv::Attribute(ptr,stride);
	}

	template<class T>
	priv::Attribute attr(fg::Buffer &buf,unsigned int offset=0,unsigned int stride=0)
	{
		priv::Attribute ret( (T*)0,stride );
		ret.buf = &buf;
		ret.ptr = (void*)offset;
		return ret;
	}
}
#endif //FRONTIER_DRAWABLE_INL_INCLUDED