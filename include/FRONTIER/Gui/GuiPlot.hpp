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
#ifndef FRONTIER_GUIPLOT_HPP_INCLUDED
#define FRONTIER_GUIPLOT_HPP_INCLUDED
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <FRONTIER/Gui/GuiWindow.hpp>
#define FRONTIER_GUIPLOT
#include <future>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Basic plot class representing a detached plot window
	/// 
	/////////////////////////////////////////////////////////////
	struct GuiPlot
	{
		/////////////////////////////////////////////////////////////
		/// @brief Default move constructor
		/// 
		/////////////////////////////////////////////////////////////
		GuiPlot(GuiPlot &&) = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Create a new guiplot
		/// 
		/// @param winSize The initial size of the window
		/// @param title The title of the plot window
		/// 
		/////////////////////////////////////////////////////////////
		GuiPlot(fm::vec2i winSize,fm::String title = "plot");
		
		/////////////////////////////////////////////////////////////
		/// @brief Create a new guiplot based on a guielement allocator
		/// 
		/// @param winSize The initial size of the window
		/// @param title The title of the plot window
		/// @param allocator The gui element allocator to use
		/// 
		/////////////////////////////////////////////////////////////
		GuiPlot(fm::vec2i winSize,fm::String title,fm::Delegate<GuiElement*,GuiWindow&> allocator);
		
		/////////////////////////////////////////////////////////////
		/// @brief Desctructor; stops the plot if it is running
		/// 
		/////////////////////////////////////////////////////////////
		~GuiPlot();
		
		/////////////////////////////////////////////////////////////
		/// @brief Stop the plotting
		/// 
		/// Forcefully stops the plot, in a thread-safe manner
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiPlot &stop();
		
		/////////////////////////////////////////////////////////////
		/// @brief Wait until the plot finishes
		/// 
		/// This may happen due to the user closing the plot window or the call of stop
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiPlot &wait();
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the plot is running
		/// 
		/// This may happen due to the user closing the plot window or the call of stop
		/// 
		/// @return True iff the plot is running
		/// 
		/////////////////////////////////////////////////////////////
		bool running() const;
		
	protected:
		virtual GuiElement *createPlotterElement() {return nullptr;} ///< Should be overwritten by child classes
		
		GuiWindow &win() {return m_win;} ///< Lets child classes access the window
		
	private:
		GuiWindow m_win; ///< The window of the plot
		std::future<void> m_fut; ///< Implementation detail
	};
}

#endif // FRONTIER_GUIPLOT_HPP_INCLUDED
