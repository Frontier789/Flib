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
}
