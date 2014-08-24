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
		return setAttribPointer(name,2,GlTypeIdentify<T>::value,0,pointer,stride);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline Shader::reference Shader::setAttribPointer(const std::string &name,const fm::vector3<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,3,GlTypeIdentify<T>::value,0,pointer,stride);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline Shader::reference Shader::setAttribPointer(const std::string &name,const fm::vector4<T> *pointer,unsigned int stride)
	{
		return setAttribPointer(name,4,GlTypeIdentify<T>::value,0,pointer,stride);
	}
}
