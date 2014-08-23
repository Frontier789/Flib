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