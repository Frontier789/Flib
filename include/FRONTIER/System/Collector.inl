////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_COLLECTOR_INL_INCLUDED
#define FRONTIER_COLLECTOR_INL_INCLUDED

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	Collector<T,Container>::Collector()
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	template<class Container2>
	Collector<T,Container>::Collector(const Collector<T,Container2> &copy) : m_container(copy.m_container.begin(),copy.m_container.end())
	{

	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	T &Collector<T,Container>::operator[](const fm::Size &i)
	{
		return m_container[i];
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	const T &Collector<T,Container>::operator[](const fm::Size &i) const
	{
		return m_container[i];
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	template<class T2>
	const Collector<T,Container> &Collector<T,Container>::operator=(const T2 &element) const
	{
		m_container.resize(0);
		m_container.push_back((T)element);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	fm::Size Collector<T,Container>::size() const
	{
		return m_container.size();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	typename Container::iterator Collector<T,Container>::begin()
	{
		return m_container.begin();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	typename Container::const_iterator Collector<T,Container>::begin() const
	{
		return m_container.begin();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	typename Container::iterator Collector<T,Container>::end()
	{
		return m_container.end();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	typename Container::const_iterator Collector<T,Container>::end() const
	{
		return m_container.end();
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	const Collector<T,Container> &Collector<T,Container>::push_back(const T &data) const
	{
		m_container.push_back(data);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container>
	const Collector<T,Container> &Collector<T,Container>::push_front(const T &data) const
	{
		m_container.push_front(data);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator,(const Collector<T,Container> &collector,const T2 &data)
	{
		return collector.push_back((T)data);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator,(const T2 &data,const Collector<T,Container> &collector)
	{
		return collector.push_front((T)data);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator<<(const Collector<T,Container> &collector,const T2 &data)
	{
		return collector.push_back((T)data);
	}

	/////////////////////////////////////////////////////////////
	template<class T,class Container,class T2>
	const Collector<T,Container> &operator>>(const T2 &data,const Collector<T,Container> &collector)
	{
		return collector.push_front((T)data);
	}
}


#endif // FRONTIER_COLLECTOR_INL_INCLUDED
