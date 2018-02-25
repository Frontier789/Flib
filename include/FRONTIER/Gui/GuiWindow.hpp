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
#ifndef FRONTIER_GUIWINDOW_HPP_INCLUDED
#define FRONTIER_GUIWINDOW_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/Window/Window.hpp>

#define FRONTIER_GUIWINDOW

namespace fw
{
	class Event;
}

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Wrapper class that extends the functionality of Window
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiWindow : public fw::Window, public GuiContext
	{
	protected:
		void drawElementsGuiLoop() override; ///< Draw the elements inside gui loop

	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// This function does not open the window
		///
		/////////////////////////////////////////////////////////////
		GuiWindow();

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param win The window to move
		///
		/////////////////////////////////////////////////////////////
		GuiWindow(GuiWindow &&win);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the window from its attributes
		///
		/// @param pos The position of the window
		/// @param size The size of the window
		/// @param title Title of the window
		/// @param style Style of the window (see fw::WindowStyle)
		/// @param settings The settings (hints) for the OpenGL context
		///
		/////////////////////////////////////////////////////////////
		GuiWindow(const fm::vec2i &pos,const fm::vec2u &size,const fm::String &title = " ",fw::Window::WindowStyle style = fw::Window::Default,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Construct the window from its attributes
		/// 
		/// @param size The size of the window
		/// @param title Title of the window
		/// @param style Style of the window (see fw::WindowStyle)
		/// @param settings The settings (hints) for the OpenGL context
		///
		/////////////////////////////////////////////////////////////
		GuiWindow(const fm::vec2u &size,const fm::String &title = " ",fw::Window::WindowStyle style = fw::Window::Default,fw::GLContext::Settings settings = fw::GLContext::Settings());
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~GuiWindow() = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the size of the context
		///
		/// @return The size of the context
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2s getSize() const override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the context
		///
		/// @param size The size of the context
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size) override;

		/////////////////////////////////////////////////////////////
		/// @brief Swap two gui windows
		///
		/// @param win The window to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiWindow &swap(GuiWindow &win);

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		///
		/// @param win The window to move
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		GuiWindow &operator=(GuiWindow &&win);

		/////////////////////////////////////////////////////////////
		/// @brief Handle an event
		/// 
		/// forwards to the owned gui elements
		/// 
		/// @param ev The event
		/// 
		/////////////////////////////////////////////////////////////
		virtual void handleEvent(fw::Event &ev) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Call the draw function of the elements in the main layout
		/// 
		/////////////////////////////////////////////////////////////
		virtual void drawElements() override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle all pending events
		/// 
		/// This function is to be implemented by derived classes
		/// 
		/////////////////////////////////////////////////////////////
		virtual void handlePendingEvents() override;
	};
}

#endif // FRONTIER_GUIWINDOW_HPP_INCLUDED
