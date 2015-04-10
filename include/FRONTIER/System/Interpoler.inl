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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_INTERPOLER_INL_INCLUDED
#define FRONTIER_INTERPOLER_INL_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <cmath>
namespace fm
{
	/// val_map /////////////////////////////////////////////////////////
	template<class VAL,class T>
	inline Interpoler_Base<VAL,T>::val_map::val_map() : t(T()),
                                                        val(VAL())
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	inline Interpoler_Base<VAL,T>::val_map::val_map(const T &t,const VAL &val) : t(t),
                                                                                 val(val)
	{

	}

	/// Constructors /////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler_Base<VAL,T>::Interpoler_Base()
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler_Base<VAL,T>::Interpoler_Base(const std::vector<Interpoler_Base<VAL,T>::val_map> &data) : m_data(data.begin(),data.end())
	{

	}


	/// Functions /////////////////////////////////////////////////////////
	template<class VAL,class T>
	inline void Interpoler_Base<VAL,T>::push_back(const T &t,const VAL &val)
	{
		m_data.push_back(val_map(t,val));
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	inline void Interpoler_Base<VAL,T>::push_back(const val_map &tval)
	{
		m_data.push_back(tval);
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler_Base<VAL,T>::resize(unsigned int size)
	{
		m_data.resize(size);
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	unsigned int Interpoler_Base<VAL,T>::size() const
	{
		return m_data.size();
	}

	////////////////////////////////////////////////////////////

	template<class VAL,class T>
	void Interpoler_Base<VAL,T>::setPt(unsigned int index,const Interpoler_Base<VAL,T>::val_map &tval)
	{
		m_data[index] = tval;
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler_Base<VAL,T>::setPt(unsigned int index,const T &t,const VAL &val)
	{
		m_data[index] = val_map(t,val);
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler_Base<VAL,T>::setVal(unsigned int index,const VAL &val)
	{
		m_data[index].val = val;
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler_Base<VAL,T>::setT(unsigned int index,const T &t)
	{
		m_data[index].t = t;
		updated();
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const T &Interpoler_Base<VAL,T>::getT(unsigned int index) const
	{
		return m_data[index].t;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const VAL &Interpoler_Base<VAL,T>::getVal(unsigned int index) const
	{
		return m_data[index].val;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const typename Interpoler_Base<VAL,T>::val_map &Interpoler_Base<VAL,T>::getPt(unsigned int index) const
	{
		return m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const std::vector<typename Interpoler_Base<VAL,T>::val_map> &Interpoler_Base<VAL,T>::getData() const
	{
		return m_data;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	VAL Interpoler_Base<VAL,T>::operator()(const T &t) const
	{
		return calcVal(t);
	}










	/// Polynomial /////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Polynomial,VAL,T>::Interpoler() : m_isPolinomValid(true)
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	template<InterpolerType Type2>
	Interpoler<Polynomial,VAL,T>::Interpoler(const Interpoler<Type2,VAL,T> &copy) : Interpoler_Base<VAL,T>(copy.getData()),
                                                                                    m_isPolinomValid(false)
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler<Polynomial,VAL,T>::updated()
	{
		m_isPolinomValid = false;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	VAL Interpoler<Polynomial,VAL,T>::calcVal(const T &t) const
	{
		createPolinom();
		VAL ret=VAL();
		T mult=T(1.0);
		C(m_polinom.size())
			ret=ret+m_polinom[i]*mult,
			mult=mult*t;
		return ret;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const std::vector<VAL> &Interpoler<Polynomial,VAL,T>::getPolinom() const
	{
		createPolinom();
		return m_polinom;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler<Polynomial,VAL,T>::createPolinom() const
	{
		if (m_isPolinomValid)
			return;
		m_isPolinomValid=true;

		const std::vector<val_map> &m_data = Interpoler_Base<VAL,T>::m_data;

		std::vector<VAL> cache;
		cache.resize(m_data.size());
		C(m_data.size())
			cache[i] = m_data[i].val;


		if (m_data.size())
			Cx(m_data.size()-1)
				for(int i=m_data.size()-1;i>x;i--)
					cache[i]=(cache[i]+cache[i-1]*T(-1.0))*(T(1.0)/(m_data[i].t-m_data[i-x-1].t));


		std::vector<T> lastPoli,lastPoliC;
		lastPoli.push_back(T(1.0));
		m_polinom.resize(0);
		C(m_data.size())
		{
			if (m_polinom.size() < lastPoli.size())
				m_polinom.resize(lastPoli.size(),VAL());
			Cx(lastPoli.size())
				m_polinom[x] = m_polinom[x]+cache[i]*lastPoli[x];

			lastPoli.push_back(T());
			lastPoliC = lastPoli;
			Cy(lastPoli.size())
				lastPoli[y] = (y ? lastPoliC[y-1] : T()) - lastPoliC[y]*m_data[i].t;
		}

		while (m_polinom.size())
		{
			bool done=false;
			VAL act=m_polinom.back()+m_polinom.back();
			C(sizeof(VAL))
				if (((char*)&act)[i]!=((char*)&m_polinom.back())[i])
				{
					done=true;
					break;
				}
			if (done)
				break;
			m_polinom.pop_back();
		}
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Polynomial,VAL,T> Interpoler<Polynomial,VAL,T>::operator+(const_reference other) const
	{
		createPolinom();
		other.createPolinom();
		Interpoler<Polynomial,VAL,T> ret;
		ret.m_isPolinomValid = true;
		ret.m_polinom.resize(m_polinom.size() > other.m_polinom.size() ? m_polinom.size() : other.m_polinom.size());
		C(ret.m_polinom.size())
			ret.m_polinom[i] = (i >= m_polinom.size() ? other.m_polinom[i] : (i >= other.m_polinom.size() ? m_polinom[i] : other.m_polinom[i]+m_polinom[i]) );
		return ret;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Polynomial,VAL,T> Interpoler<Polynomial,VAL,T>::operator*(const T &ratio) const
	{
		createPolinom();
		Interpoler<Polynomial,VAL,T> ret;
		ret.m_isPolinomValid = true;
		ret.m_polinom = m_polinom;
		C(ret.m_polinom.size())
			ret.m_polinom[i] = ret.m_polinom[i]*ratio;
		return ret;
	}



	/// Linear /////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Linear,VAL,T>::Interpoler()
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	template<InterpolerType Type2>
	Interpoler<Linear,VAL,T>::Interpoler(const Interpoler<Type2,VAL,T> &copy) : Interpoler_Base<VAL,T>(copy.getData())
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler<Linear,VAL,T>::updated()
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	VAL Interpoler<Linear,VAL,T>::calcVal(const T &t) const
	{
		const std::vector<val_map> &m_data = Interpoler_Base<VAL,T>::m_data;

		if (!m_data.size())
			return VAL();
		const val_map *down=0,*up=0;
		C(m_data.size())
			if (m_data[i].t <= t && (!down || m_data[i].t > down->t))
				down = &m_data[i];
			else if (m_data[i].t >= t && (!up || m_data[i].t < up->t))
				up = &m_data[i];

		if (!down && up)
			return up->val;
		if (!up && down)
			return down->val;
		if (!up && !down)
			return VAL();

		return (down->val*(up->t-t)+up->val*(t-down->t))*(T(1.0)/(up->t-down->t));
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	typename Interpoler<Linear,VAL,T>::val_map &Interpoler<Linear,VAL,T>::operator[](unsigned int index)
	{
		return Interpoler_Base<T>::m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const typename Interpoler<Linear,VAL,T>::val_map &Interpoler<Linear,VAL,T>::operator[](unsigned int index) const
	{
		return Interpoler_Base<T>::m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Linear,VAL,T> Interpoler<Linear,VAL,T>::operator+(const_reference other) const
	{
		Interpoler<Linear,VAL,T> ret;
		ret.resize(Interpoler_Base<VAL,T>::size());
		for(fm::Size i=0;i<Interpoler_Base<VAL,T>::size();i++)
			ret.setPt(i,Interpoler_Base<VAL,T>::getT(i),Interpoler_Base<VAL,T>::getVal(i)+other(Interpoler_Base<VAL,T>::getT(i)));
		return ret;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Linear,VAL,T> Interpoler<Linear,VAL,T>::operator*(const T &ratio) const
	{
		Interpoler<Linear,VAL,T> ret;
		ret.resize(Interpoler_Base<VAL,T>::size());
		for(fm::Size i=0;i<Interpoler_Base<VAL,T>::size();i++)
			ret.setPt(i,Interpoler_Base<VAL,T>::getT(i),Interpoler_Base<VAL,T>::getVal(i)*ratio);
		return ret;
	}



	/// Nearest /////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Nearest,VAL,T>::Interpoler()
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	template<InterpolerType Type2>
	Interpoler<Nearest,VAL,T>::Interpoler(const Interpoler<Type2,VAL,T> &copy) : Interpoler_Base<VAL,T>(copy.getData())
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	void Interpoler<Nearest,VAL,T>::updated()
	{

	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	VAL Interpoler<Nearest,VAL,T>::calcVal(const T &t) const
	{
		const std::vector<val_map> &m_data = Interpoler_Base<VAL,T>::m_data;

		if (!m_data.size())
			return VAL();
        const T *dt=0;
		fm::Size mindti=0;
		C(m_data.size())
            if (!dt || std::abs(m_data[i].t-t)<std::abs(*dt - t))
                dt = &m_data[i].t,
                mindti=i;
		return m_data[mindti].val;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	typename Interpoler<Nearest,VAL,T>::val_map &Interpoler<Nearest,VAL,T>::operator[](unsigned int index)
	{
		return Interpoler_Base<VAL,T>::m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	const typename Interpoler<Nearest,VAL,T>::val_map &Interpoler<Nearest,VAL,T>::operator[](unsigned int index) const
	{
		return Interpoler_Base<VAL,T>::m_data[index];
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Nearest,VAL,T> Interpoler<Nearest,VAL,T>::operator+(const_reference other) const
	{
		Interpoler<Nearest,VAL,T> ret;
		ret.resize(Interpoler_Base<VAL,T>::size());
		for(fm::Size i=0;i<Interpoler_Base<VAL,T>::size();i++)
			ret.setPt(i,Interpoler_Base<VAL,T>::getT(i),Interpoler_Base<VAL,T>::getVal(i)+other(Interpoler_Base<VAL,T>::getT(i)));
		return ret;
	}

	////////////////////////////////////////////////////////////
	template<class VAL,class T>
	Interpoler<Nearest,VAL,T> Interpoler<Nearest,VAL,T>::operator*(const T &ratio) const
	{
		Interpoler<Nearest,VAL,T> ret;
		ret.resize(Interpoler_Base<VAL,T>::size());
		for(fm::Size i=0;i<Interpoler_Base<VAL,T>::size();i++)
			ret.setPt(i,Interpoler_Base<VAL,T>::getT(i),Interpoler_Base<VAL,T>::getVal(i)*ratio);
		return ret;
	}
}
#endif
