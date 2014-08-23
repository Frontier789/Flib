#ifndef FRONTIER_VERTEX_INL_INCLUDED
#define FRONTIER_VERTEX_INL_INCLUDED
#include <FRONTIER/System/Vertex.hpp>
namespace fm
{
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		template<class T>
		nullClass::operator T() const
		{
			return T();
		}
	}

	/////////////////////////////////////////////////////////////
	template<class posType,class clrType,class texPosType,class normType>
	vertex<posType,clrType,texPosType,normType>::vertex(const posType &pos,const clrType &clr,const texPosType &texPos,const normType &norm) : pos(pos),
                                                                                                                                               clr(clr),
                                                                                                                                               texPos(texPos),
                                                                                                                                               norm(norm)
	{
		
	}
	
	//////////////////////////////////
	template<class posType,class clrType,class texPosType,class normType>
	vertex<posType,clrType,texPosType,normType> &vertex<posType,clrType,texPosType,normType>::operator()(const posType &pos,const clrType &clr)
	{
		this->pos = pos;
		this->clr = clr;
		return *this;
	}

	//////////////////////////////////
	template<class posType,class clrType,class texPosType,class normType>
	vertex<posType,clrType,texPosType,normType> &vertex<posType,clrType,texPosType,normType>::operator()(const posType &pos,const clrType &clr,const texPosType &texPos)
	{
		this->pos = pos;
		this->clr = clr;
		this->texPos = texPos;
		return *this;
	}

	//////////////////////////////////
	template<class posType,class clrType,class texPosType,class normType>
	vertex<posType,clrType,texPosType,normType> &vertex<posType,clrType,texPosType,normType>::operator()(const posType &pos,const clrType &clr,const texPosType &texPos,const normType &norm)
	{
		this->pos = pos;
		this->clr = clr;
		this->texPos = texPos;
		this->norm = norm;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class posType,class clrType,class texPosType>
	vertex<posType,clrType,texPosType>::vertex(const posType &pos,const clrType &clr,const texPosType &texPos) : pos(pos),
                                                                                                                 clr(clr),
                                                                                                                 texPos(texPos)
	{
		
	}
	
	//////////////////////////////////
	template<class posType,class clrType,class texPosType>
	vertex<posType,clrType,texPosType> &vertex<posType,clrType,texPosType>::operator()(const posType &pos,const clrType &clr)
	{
		this->pos = pos;
		this->clr = clr;
		return *this;
	}

	//////////////////////////////////
	template<class posType,class clrType,class texPosType>
	vertex<posType,clrType,texPosType> &vertex<posType,clrType,texPosType>::operator()(const posType &pos,const clrType &clr,const texPosType &texPos)
	{
		this->pos = pos;
		this->clr = clr;
		this->texPos = texPos;
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class posType,class clrType>
	vertex<posType,clrType>::vertex(const posType &pos,const clrType &clr) : pos(pos),
                                                                             clr(clr)
	{
		
	}
	
	//////////////////////////////////
	template<class posType,class clrType>
	vertex<posType,clrType> &vertex<posType,clrType>::operator()(const posType &pos,const clrType &clr)
	{
		this->pos = pos;
		this->clr = clr;
		return *this;
	}
}
#endif // FRONTIER_VERTEX_INL_INCLUDED
