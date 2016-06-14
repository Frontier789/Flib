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
namespace fm
{
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container>::MatrixStack()
	{

	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container>::MatrixStack(const fm::matrix<W,H,T> &mat) : m_container(1,mat)
	{

	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	template<class Container2>
	inline MatrixStack<W,H,T,Container>::MatrixStack(const MatrixStack<W,H,T,Container2> &copy) : m_container(copy.m_container.begin(),copy.m_container.end())
	{

	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container> &MatrixStack<W,H,T,Container>::push(const fm::matrix<W,H,T> &mat)
	{
		m_container.push_back(mat);

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container> &MatrixStack<W,H,T,Container>::push()
	{
		m_container.push_back(top());

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container> &MatrixStack<W,H,T,Container>::mul(const fm::matrix<W,H,T> &mat)
	{
		if (!empty())
			m_container.back() *= mat;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container> &MatrixStack<W,H,T,Container>::preMul(const fm::matrix<W,H,T> &mat)
	{
		if (!empty())
			m_container.back() = mat * m_container.back();

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline fm::matrix<W,H,T> MatrixStack<W,H,T,Container>::pop()
	{
		if (empty())
			return fm::matrix<W,H,T>();
		else
		{
			fm::matrix<W,H,T> cpy = m_container.back();
			m_container.pop_back();
			return cpy;
		}
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline fm::matrix<W,H,T> MatrixStack<W,H,T,Container>::top() const
	{
		if (empty())
			return fm::matrix<W,H,T>();

		return m_container.back();
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline MatrixStack<W,H,T,Container> &MatrixStack<W,H,T,Container>::top(const fm::matrix<W,H,T> &mat)
	{
		if (empty())
            push(mat);
        else
			m_container.back() = mat;

		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline bool MatrixStack<W,H,T,Container>::empty() const
	{
		return size() == 0;
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline void MatrixStack<W,H,T,Container>::clear() const
	{
		m_container.clear();
	}

	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T,class Container>
	inline fm::Size MatrixStack<W,H,T,Container>::size() const
	{
		return m_container.size();
	}
}
