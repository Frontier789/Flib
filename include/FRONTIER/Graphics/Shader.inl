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
namespace fg
{
	namespace priv
	{
		template<class pt,class ct,class tpt,class nt>
		inline unsigned int getStride()
		{
			return (pt::dimensions  ? sizeof(pt)  : 0)+
				   (ct::dimensions  ? sizeof(ct)  : 0)+
				   (tpt::dimensions ? sizeof(tpt) : 0)+
				   (nt::dimensions  ? sizeof(nt)  : 0);
		}
		template<class T>
		class GlTypeIdentify {};
		template<>
		class GlTypeIdentify<char> 			 {public:enum{value=0x1400};}; ///GL_BYTE
		template<>
		class GlTypeIdentify<unsigned char>  {public:enum{value=0x1401};}; ///GL_UNSIGNED_BYTE
		template<>
		class GlTypeIdentify<short> 		 {public:enum{value=0x1402};}; ///GL_SHORT
		template<>
		class GlTypeIdentify<unsigned short> {public:enum{value=0x1403};}; ///GL_UNSIGNED_SHORT
		template<>
		class GlTypeIdentify<int>            {public:enum{value=0x1404};}; ///GL_INT
		template<>
		class GlTypeIdentify<unsigned int>   {public:enum{value=0x1405};}; ///GL_UNSIGNED_INT
		template<>
		class GlTypeIdentify<float>  		 {public:enum{value=0x1406};}; ///GL_FLOAT
		template<>
		class GlTypeIdentify<double>  	     {public:enum{value=0x140A};}; ///GL_DOUBLE
	}

	/////////////////////////////////////////////////////////////
	inline Shader::TexUniformData::TexUniformData(int location,int slot,int act_id) : location(location),
																					  slot(slot),
																					  act_id(act_id)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline Shader::reference Shader::setAttribPointer(const std::string &posName,const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		setAttribPointer(posName,&pointer[0].pos,priv::getStride<pt,ct,tpt,nt>());
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline Shader::reference Shader::setAttribPointer(const std::string &posName,const std::string &clrName,const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		setAttribPointer(posName,&pointer[0].pos,priv::getStride<pt,ct,tpt,nt>());
		setAttribPointer(clrName,&pointer[0].clr,priv::getStride<pt,ct,tpt,nt>());
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline Shader::reference Shader::setAttribPointer(const std::string &posName,const std::string &clrName,const std::string &texPosName,const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		setAttribPointer(posName   ,&pointer[0].pos   ,priv::getStride<pt,ct,tpt,nt>());
		setAttribPointer(clrName   ,&pointer[0].clr   ,priv::getStride<pt,ct,tpt,nt>());
		setAttribPointer(texPosName,&pointer[0].texPos,priv::getStride<pt,ct,tpt,nt>());
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class pt,class ct,class tpt,class nt>
	inline Shader::reference Shader::setAttribPointer(const std::string &posName,const std::string &clrName,const std::string &texPosName,const std::string &normName,const fm::vertex<pt,ct,tpt,nt> *pointer)
	{
		setAttribPointer(posName   ,&pointer[0].pos   ,priv::getStride<pt,ct,tpt,nt>());
		setAttribPointer(clrName   ,&pointer[0].clr   ,priv::getStride<pt,ct,tpt,nt>());
		setAttribPointer(texPosName,&pointer[0].texPos,priv::getStride<pt,ct,tpt,nt>());
		setAttribPointer(normName  ,&pointer[0].norm  ,priv::getStride<pt,ct,tpt,nt>());
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline Shader::reference Shader::setAttribPointer(const std::string &name,const fm::vector2<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,2,priv::GlTypeIdentify<T>::value,0,pointer,stride);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline Shader::reference Shader::setAttribPointer(const std::string &name,const fm::vector3<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,3,priv::GlTypeIdentify<T>::value,0,pointer,stride);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline Shader::reference Shader::setAttribPointer(const std::string &name,const fm::vector4<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,4,priv::GlTypeIdentify<T>::value,0,pointer,stride);
	}
	
	/////////////////////////////////////////////////////////////
	template<fm::MATRIX::StorageOrder storeOrder>
	inline Shader::reference Shader::setUniform(const std::string &name,const fm::mat3 &m)
	{
		return setUniform(name,m,storeOrder);
	}
	
	/////////////////////////////////////////////////////////////
	template<fm::MATRIX::StorageOrder storeOrder>
	inline Shader::reference Shader::setUniform(const std::string &name,const fm::mat4 &m)
	{
		return setUniform(name,m,storeOrder);
	}
}
