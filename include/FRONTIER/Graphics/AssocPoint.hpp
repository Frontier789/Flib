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
#ifndef FRONTIER_ASSOCPOINT_HPP_INCLUDED
#define FRONTIER_ASSOCPOINT_HPP_INCLUDED

#define FRONTIER_ASSOCPOINT

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to encapsulate Point
	///
	/// @ingroup Graphics
	/// 
	/////////////////////////////////////////////////////////////
	class Assoc
	{
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Encodes the type of an attribute
		/// 
		/////////////////////////////////////////////////////////////
		enum Point {
			Unused    = 0, ///< The attribute is not in use
			Position  = 1, ///< The attribute encodes position
			Color     = 2, ///< The attribute encodes color
			Normal    = 3, ///< The attribute encodes normal
			TextureUV = 4, ///< The attribute encodes texture position
			Tangent   = 5, ///< The attribute encodes tangent vector
			Bitangent = 6, ///< The attribute encodes bitangent vector
			Custom    = 7, ///< The attribute encodes a custom property
			Custom0   = 7  ///< The attribute encodes a custom property
		};
	};

	/////////////////////////////////////////////////////////////
	/// @brief Advance an AssocPoint
	/// 
	/// Intended to be used with Assoc::Custom
	/// 
	/// @param pt The AssocPoint to advance
	/// @param delta The amount to increase with
	/// 
	/// @return The resulting AssocPoint
	/// 
	/////////////////////////////////////////////////////////////
	Assoc::Point operator+(const Assoc::Point &pt,int delta);

	/////////////////////////////////////////////////////////////
	/// @brief Decrease an AssocPoint
	/// 
	/// Intended to be used with Assoc::Custom
	/// 
	/// @param pt The AssocPoint to decrease
	/// @param delta The amount to increase with
	/// 
	/// @return The resulting AssocPoint
	/// 
	/////////////////////////////////////////////////////////////
	Assoc::Point operator-(const Assoc::Point &pt,int delta);

	typedef Assoc::Point AssocPoint; ///< Easier typable name for Assoc::Point
}

#endif // FRONTIER_ASSOCPOINT_HPP_INCLUDED
