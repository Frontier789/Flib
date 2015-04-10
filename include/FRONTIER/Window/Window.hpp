////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
#ifndef FRONTIER_WINDOW_HPP_INCLUDED
#define FRONTIER_WINDOW_HPP_INCLUDED
#include <FRONTIER/Window/WindowPredef.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Vector2.hpp>
#define FRONTIER_WINDOW
#include <string>

namespace fg
{
	class Image;
}

namespace fw
{
	class Event;

	/////////////////////////////////////////////////////////////
	/// @brief Wrapper class used to manage windows and OpenGL contexts together, os independently
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Window : public fw::GLContext
	{
		priv::Window *m_window;  ///< The underlying implementation

		/////////////////////////////////////////////////////////////
		/// @brief Override of the inherited create function
		///
		/// @param windowHandle The handle of the associated window (shouldn't be NULL)
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool create(priv::WindowHandle windowHandle,fw::GLContext::Settings settings);

		/////////////////////////////////////////////////////////////
		/// @brief Override of the inherited create function
		///
		/// @param size The size of the offscreen buffer
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool create(const fm::vec2s &size,fw::GLContext::Settings settings);

		/////////////////////////////////////////////////////////////
		/// @brief Override of the inherited create function
		///
		/// @param settings The attributes of the context (only a hint!)
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool create(fw::GLContext::Settings settings);

		/////////////////////////////////////////////////////////////
		/// @brief Override of the inherited destroy function
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool destroy();
	public:
		typedef Window &reference;
		typedef const Window &const_reference;

		typedef priv::WindowHandle Handle; ///< The window handle type
		typedef priv::WindowEventCallback EventCallback; ///< The type of the event callback (os specific)

		/////////////////////////////////////////////////////////////
		/// @brief Enumeration used to indicate window style
		///
		/// @ingroup Window
		///
		/////////////////////////////////////////////////////////////
		enum WindowStyle {
			None        = 0x000, // The window is borderless
			Menu        = 0x001, // The window is borderless and not shown in the taskbar
			Close       = 0x002, // The window has a close button (adds Border and Titlebar)
			Border      = 0x004, // The window has border
			Resize      = 0x008, // The window is resizeable (adds Border)
			Hidden      = 0x010, // The window is not set visible when created
			Toolbar     = 0x020, // The window is a floating toolbar
			Minimize    = 0x040, // The window has minimize button (adds Border and Titlebar)
			Maximize    = 0x080, // The window has maximize button (adds Border and Titlebar)
			Titlebar    = 0x100, // The window has titlebar (adds Border)
			Fullscreen  = 0x200, // The window is fullscreen
			SkipTaskbar = 0x400, // The window is not shown in the taskbar
			Default     = 0x1CE  // The window is an ordinary window
		};

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// This function does not open the window
		///
		/////////////////////////////////////////////////////////////
		Window();

		/////////////////////////////////////////////////////////////
		/// @brief Construct the window from its attributes
		///
		/// @param pos The position of the window
		/// @param size The size of the window
		/// @param title Title of the window
		/// @param style Style of the window (see fw::WindowStyle)
		/// @param parent The parent of the window (when the parent is deactivated so is the child)
		/// @param container The window that contains the new one
		/// @param settings The settings (hints) for the OpenGL context
		///
		/////////////////////////////////////////////////////////////
		Window(const fm::vec2i &pos,const fm::vec2u &size,const std::string &title = std::string(),fw::Window::WindowStyle style = fw::Window::Default,Window *parent = NULL,Handle container = 0,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~Window();

		/////////////////////////////////////////////////////////////
		/// @brief Closes the window
		///
		/// Closing a closed window wont result in anything
		///
		/////////////////////////////////////////////////////////////
		void close();

		/////////////////////////////////////////////////////////////
		/// @brief (Re)Open the window
		///
		/// @param pos The position of the window
		/// @param size The size of the window
		/// @param title Title of the window
		/// @param style Style of the window (see fw::WindowStyle)
		/// @param parent The parent of the window (when the parent is deactivated so is the child)
		/// @param container The window that contains the new one
		/// @param settings The settings (hints) for the OpenGL context
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool open(const fm::vec2i &pos,const fm::vec2u &size,const std::string &title = std::string(),fw::Window::WindowStyle style = fw::Window::Default,Window *parent = NULL,Handle container = 0,fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Destroy the GL context
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool destroyContext();

		/////////////////////////////////////////////////////////////
		/// @brief (Re)create the GL context
		///
		/// @param settings The settings (hints) for the OpenGL context
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool createContext(fw::GLContext::Settings settings = fw::GLContext::Settings());

		/////////////////////////////////////////////////////////////
		/// @brief Check if the window is opened
		///
		/// @return True iff the window is open
		///
		/////////////////////////////////////////////////////////////
		bool isOpen() const;

		/////////////////////////////////////////////////////////////
		/// @brief Minimizes the window
		///
		/////////////////////////////////////////////////////////////
		void minimize();

		/////////////////////////////////////////////////////////////
		/// @brief Discover whether the window is minimized
		///
		/// @return True iff the window is minimized
		///
		/////////////////////////////////////////////////////////////
		bool isMinimized() const;

		/////////////////////////////////////////////////////////////
		/// @brief Maximazes the window
		///
		/////////////////////////////////////////////////////////////
		void maximize();

		/////////////////////////////////////////////////////////////
		/// @brief Discover whether the window is maximized
		///
		/// @return True iff the window is maximized
		///
		/////////////////////////////////////////////////////////////
		bool isMaximized() const;

		/////////////////////////////////////////////////////////////
		/// @brief Brings window in front
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool setFocus();

		/////////////////////////////////////////////////////////////
		/// @brief Binds the GL context of the window
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool setContextActive(bool active = true);

		/////////////////////////////////////////////////////////////
		/// @brief Shows or hides the cursor in the window
		///
		/// By default it is shown
		///
		/// @param show The cursor is shown iff true
		///
		/////////////////////////////////////////////////////////////
		void showCursor(bool show = true);

		/////////////////////////////////////////////////////////////
		/// @brief Changes the position and the size of the window
		///
		/// The given data is used to determine the properties
		/// of the client (drawing) area, not the complete window
		///
		/// @param pos The position of the upper-left corner
		/// @param size The size of the window
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool setRect(const fm::vec2i &pos,const fm::vec2u &size);

		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the position and the size of the window
		///
		/// The returned data describes the client rect
		///
		/// @param pos The position of the upper-left corner
		/// @param size The size of the window
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool getRect(fm::vec2i &pos,fm::vec2u &size);


		/////////////////////////////////////////////////////////////
		/// @brief Changes the position of the window
		///
		/// The given data is used to determine the properties
		/// of the client (drawing) area, not the complete window
		///
		/// @param pos The position of the upper-left corner
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool setPosition(const fm::vec2i &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the position of the window
		///
		/// The returned data describes the client rect
		///
		/// @return The position of the window
		///
		/////////////////////////////////////////////////////////////
		fm::vec2i getPosition() const;

		/////////////////////////////////////////////////////////////
		/// @brief Changes the size of the window
		///
		/// The given data is used to determine the properties
		/// of the client (drawing) area, not the complete window
		///
		/// @param size The size of the window
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool setSize(const fm::vec2u &size);

		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the size of the window
		///
		/// The returned data describes the client rect
		///
		/// @return The size of the window
		///
		/////////////////////////////////////////////////////////////
		fm::vec2u getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the title of the window
		///
		/// @param title The new title
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool setTitle(const std::string &title);

		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the title of the window
		///
		/// @return The title of the window
		///
		/////////////////////////////////////////////////////////////
		std::string getTitle() const;

		/////////////////////////////////////////////////////////////
		/// @brief Show or hide the window
		///
		/// @param visible If true the window is shown
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		void setVisible(bool visible = true);

		/////////////////////////////////////////////////////////////
		/// @brief Retrive the last event
		///
		/// If the event queue is empty false is returned and
		/// @a ev is not modified
		///
		/// @param ev Set to the last event
		///
		/// @return True iff there was a event
		///
		/////////////////////////////////////////////////////////////
		bool popEvent(Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Suspend thread until a event occures
		///
		/// If an error occures false is returned and ev is not modified
		///
		/// @param ev Set to the last event
		///
		/// @return True iff everything went right
		///
		/////////////////////////////////////////////////////////////
		bool waitEvent(Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Adds a new event to the back of the event queue
		///
		/// @param ev The new event
		///
		/////////////////////////////////////////////////////////////
		void postEvent(const Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Enables or disables keyrepeat
		///
		/// If enabled, when a key is held down
		/// multiple press events will be sent
		///
		/// @param enable True to enable false to disable
		///
		/////////////////////////////////////////////////////////////
		void enableKeyRepeat(bool enable = true);

		/////////////////////////////////////////////////////////////
		/// @brief Returns whether keyrepeat is enabled
		///
		/// @return True iff enabled
		///
		/////////////////////////////////////////////////////////////
		bool isKeyRepeatEnabled() const;

		/////////////////////////////////////////////////////////////
		/// @brief Enables or disables resize of the window
		///
		/// @param enable True to enable false to disable
		///
		/////////////////////////////////////////////////////////////
		void enableResize(bool enable = true);

		/////////////////////////////////////////////////////////////
		/// @brief Returns whether resize is enabled
		///
		/// @return True iff enabled
		///
		/////////////////////////////////////////////////////////////
		bool isResizeEnabled() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the icon of the window
		///
		/// @param icon The new icon
		///
		/////////////////////////////////////////////////////////////
		void setIcon(const fg::Image &icon);

		/////////////////////////////////////////////////////////////
		/// @brief Implicitly convert to HWND
		///
		/// If the window is closed (not opened) NULL is returned
		///
		/// @return The HandleWiNDow
		///
		/////////////////////////////////////////////////////////////
		operator Handle() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the window's handle
		///
		/// If the window is closed (not opened) NULL is returned
		///
		/// @return The HandleWiNDow
		///
		/////////////////////////////////////////////////////////////
		Handle getHandle() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the event callback
		///
		/// This function is called before a event is being handled
		/// The functions should return true iff no further processing is required
		///
		/// The type of the function is platform specific
		///
		/// @param callback The new callback
		///
		/////////////////////////////////////////////////////////////
		void setEventCallback(EventCallback callback);

		/////////////////////////////////////////////////////////////
		/// @brief Swap the back and front buffer
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		bool swapBuffers();

		/////////////////////////////////////////////////////////////
		/// @brief Access the underlying window implementation
		///
		/// @return The window
		///
		/////////////////////////////////////////////////////////////
		const priv::Window &getOSWindow() const;

		/////////////////////////////////////////////////////////////
		/// @brief Access the underlying window implementation
		///
		/// @return The window
		///
		/////////////////////////////////////////////////////////////
		priv::Window &getOSWindow();
	};


	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator |
	///
	/// @param left Left operand
	/// @param right Right operand
	///
	/// @return left|right casted to fw::Window::WindowStyle
	///
	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator|(const Window::WindowStyle &left,const Window::WindowStyle &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator &
	///
	/// @param left Left operand
	/// @param right Right operand
	///
	/// @return left|right casted to fw::Window::WindowStyle
	///
	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator&(const Window::WindowStyle &left,const Window::WindowStyle &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of unary operator ~
	///
	/// @param style The value to be inverted
	///
	/// @return ~style casted to fw::Window::WindowStyle
	///
	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator~(const Window::WindowStyle &style);
}

#endif // FRONTIER_WINDOW_HPP_INCLUDED
