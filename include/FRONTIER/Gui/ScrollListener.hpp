////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_SCROLLLISTENERR_HPP_INCLUDED
#define FRONTIER_SCROLLLISTENERR_HPP_INCLUDED

#include <FRONTIER/Gui/MouseMoveListener.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_SCROLLLISTENERR

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Listener base class for scrolling
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ScrollListener : public virtual MouseMoveListener
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		ScrollListener();
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the element is scrolled
		/// 
		/// @param amount The amount the element is scrolled
		/// @param horizontal Indicates whether vertical or horizontal scroll happened
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onScroll(float amount,bool horizontal);
	};
}

#endif // FRONTIER_SCROLLLISTENERR_HPP_INCLUDED
