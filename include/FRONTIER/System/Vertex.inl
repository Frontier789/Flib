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
