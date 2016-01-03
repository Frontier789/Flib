////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_MATRIXSTACK_HPP_INCLUDED
#define FRONTIER_MATRIXSTACK_HPP_INCLUDED
#include <FRONTIER/System/MAtrix.hpp>
#define FRONTIER_MATRIXSTACK
#include <deque>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief A simple class used to manage a matrix stack
	/// 
	/////////////////////////////////////////////////////////////
	template<fm::Size W = 4, fm::Size H = 4, class T = float, class Container = std::deque<fm::matrix<W,H,T> > >
	class MatrixStack
	{
		Container m_container; ///< The underlying container
	public:
		typedef MatrixStack<W,H,T,Container> &reference;
		typedef const MatrixStack<W,H,T,Container> &const_reference;
			
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack();
			
		/////////////////////////////////////////////////////////////
		/// @brief Initialize with one matrix
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack(const fm::matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/////////////////////////////////////////////////////////////
		template<class Container2>
		MatrixStack(const MatrixStack<W,H,T,Container2> &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Push a new matrix to the stack
		/// 
		/// @param mat The matrix to push
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack<W,H,T,Container> &push(const fm::matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @brief Push the last matrix to the stack
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack<W,H,T,Container> &push();

		/////////////////////////////////////////////////////////////
		/// @brief Multiply the top matrix
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack<W,H,T,Container> &mul(const fm::matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @brief Multiply the top matrix from the right side
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack<W,H,T,Container> &preMul(const fm::matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @brief Pop the top matrix in the stack (or identity matrix is empty)
		/// 
		/// @return The matrix
		/// 
		/////////////////////////////////////////////////////////////
		fm::matrix<W,H,T> pop();

		/////////////////////////////////////////////////////////////
		/// @brief Get the top matrix
		/// 
		/// @return The matrix
		/// 
		/////////////////////////////////////////////////////////////
		fm::matrix<W,H,T> top() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the top matrix
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		MatrixStack<W,H,T,Container> &top(const fm::matrix<W,H,T> &mat);

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the stack
		/// 
		/// @return The size
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size size() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Test if empty
		/// 
		/// @return True iff empty
		/// 
		/////////////////////////////////////////////////////////////
		bool empty()const ;
	};
}


#endif // FRONTIER_MATRIXSTACK_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/MatrixStack.inl>
#endif