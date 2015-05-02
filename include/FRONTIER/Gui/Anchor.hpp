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
#ifndef FRONTIER_ANCHOR_HPP_INCLUDED
#define FRONTIER_ANCHOR_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/macros/dont_include_inl_begin>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/macros/dont_include_inl_end>
#define FRONTIER_ANCHOR

namespace Fgui
{
	class Widget;
	
	/////////////////////////////////////////////////////////////
	/// @brief A simple class used to describe how a widget is situated in its parent
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Anchor
	{
	public:
		fm::vec2 pix; ///< The absolute offset in pixels
		fm::vec2 par; ///< The relative offset to the size of the parent
		fm::vec2 chl; ///< The relative offset to the size of the widget
	public:
		typedef Anchor &reference;
		typedef const Anchor &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param pix The absolute offset in pixels
		/// @param par The relative offset to the size of the parent
		/// @param chl The relative offset to the size of the widget
		/// 
		/////////////////////////////////////////////////////////////
		Anchor(const fm::vec2 &pix = fm::vec2(),const fm::vec2 &par = fm::vec2(),const fm::vec2 &chl = fm::vec2());

		/////////////////////////////////////////////////////////////
		/// @brief Get the position of the widget relative to its parent's origin
		/// 
		/// @param owner The owner widget of the anchor
		/// 
		/// @return The relative position
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getPosInParent(const Widget &owner) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the position of the widget relative to its root's origin
		/// 
		/// @param owner The owner widget of the anchor
		/// 
		/// @return The absolute position
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getPosInRoot(const Widget &owner) const;

		/////////////////////////////////////////////////////////////
		/// @brief The often used anchor (vec2(),vec2(.5,.5),vec2(.5,.5))
		/// 
		/////////////////////////////////////////////////////////////
		static const Anchor Middle;
	};
}


#endif // FRONTIER_ANCHOR_HPP_INCLUDED
