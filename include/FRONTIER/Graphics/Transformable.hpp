////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_TRANSFORMABLE_HPP_INCLUDED
#define FRONTIER_TRANSFORMABLE_HPP_INCLUDED

#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Matrix.hpp>

#define FRONTIER_TRANSFORMABLE

namespace fg
{	
	/////////////////////////////////////////////////////////////
	/// @brief Describes a simple transfrmation consisting of a translation then a scaling
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Transformable
	{
		fm::vec2 m_size; ///< Current size
		fm::vec2 m_pos;  ///< Current position

	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Transformable();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		virtual ~Transformable() = default;

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen position of the transformable
		/// 
		/// @param pos The position on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		virtual Transformable &setPosition(const fm::vec2 &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen size of the transformable
		/// 
		/// @param size The size on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		virtual Transformable &setSize(const fm::vec2 &size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen position of the transformable
		/// 
		/// @return The onscreen position
		/// 
		/////////////////////////////////////////////////////////////
		const fm::vec2 &getPosition() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen size of the transformable
		/// 
		/// @return The onscreen size
		/// 
		/////////////////////////////////////////////////////////////
		const fm::vec2 &getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the matrix for the current transform
		/// 
		/// @return The matrix
		/// 
		/////////////////////////////////////////////////////////////
		fm::mat4 getMatrix() const;
	};
}

#endif // FRONTIER_TRANSFORMABLE_HPP_INCLUDED
