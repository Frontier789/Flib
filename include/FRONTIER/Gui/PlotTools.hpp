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
#ifndef FRONTIER_PLOTTOOLS_HPP_INCLUDED
#define FRONTIER_PLOTTOOLS_HPP_INCLUDED
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/Gui/GuiPlot.hpp>
#define FRONTIER_PLOTTOOLS

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Structure for holding plot describing data
	/// 
	/////////////////////////////////////////////////////////////
	struct PlotOptions
	{
		fm::vec4 bgColor; ///< Background color of the plot window
		float mouseSensitivity; ///< Sensitivity of the rotation with mouse
		float scrollScale; ///< Amount to zoom when scrolling
		
		/////////////////////////////////////////////////////////////
		/// @brief Create plot options from given parameters
		/// 
		/////////////////////////////////////////////////////////////
		PlotOptions(fm::vec4 bgColor = .1,float mouseSensitivity = 1,float scrollScale = 1) : 
			bgColor(bgColor), mouseSensitivity(mouseSensitivity), scrollScale(scrollScale) {}
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Plot a 3d mesh in a detached window on a separate thread
	/// 
	/// @param m The mesh to plot
	/// @param opts The plotting options
	/// 
	/// @return The plot object
	/// 
	/////////////////////////////////////////////////////////////
	GuiPlot plot(fg::Mesh m,PlotOptions opts = PlotOptions());
}

#endif // FRONTIER_PLOTTOOLS_HPP_INCLUDED
