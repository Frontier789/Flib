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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_XLIB_WINDOW_INCLUDED
#define FRONTIER_XLIB_WINDOW_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Window/Event.hpp>
#define FRONTIER_XLIB_WINDOW

#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>

#include <string>
#include <deque>

namespace fw
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Classes related to xlib are held in this namespace
	///
	/////////////////////////////////////////////////////////////
	namespace Xlib
	{
		/////////////////////////////////////////////////////////////
		///
		/// 	@brief Class used to open, resize and process events of a window in X
		///
		/////////////////////////////////////////////////////////////
		class Window
		{
			bool checkDisplay(); ///< Internal function
			std::deque<Event> m_eventQueue; ///< A queue holding the unhandled events
			void processEvent(XEvent &xev); ///< Internal function used to convert xevents
			bool m_opened;       ///< Indicates whether the window is open
			bool m_enableRepeat; ///< Indicates whether key repeat is enabled
			KeySym m_lastDown;   ///< Holds the last pressed key
			Display *m_disp;     ///< The connection to the x server
			Atom m_delAtom;      ///< The id of the window deletion message
			Atom m_stateAtom;    ///< The id of the window state
			unsigned int m_prevW;       ///< The width after the last resize
			unsigned int m_prevH;       ///< The height after the last resize
			Atom m_stateHiddenAtom;     ///< The id of the hidden state
			mutable ::Window m_win;     ///< The handle of the xwindow
			mutable ::Window m_rootWin; ///< A handle to the root window
			void getProperty(Atom *&atoms,unsigned long *count) const;  ///< Internal function used to get properties of the window
		public:

			typedef ::Window Handle; ///< The window handle type

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
			/// Upon internal error a message is prompted to fw::Wapi::log
			///
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			///
			/////////////////////////////////////////////////////////////
			Window(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style);

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
			/// Upon internal error a message is prompted to fw::Wapi::log
			///
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool open(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style);

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
			/// @brief Changes the position and the size of the window
			///
			/// The given data is used to determine the properties
			/// of the client (drawing) area, not the complete window
			///
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			/// @param w The width of the window
			/// @param h The height of the window
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool setRect(int x,int y,unsigned int w,unsigned int h);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the position and the size of the window
			///
			/// The returned data describes the client rect
			///
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			/// @param w The width of the window
			/// @param h The height of the window
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool getRect(int &x,int &y,unsigned int &w,unsigned int &h);


			/////////////////////////////////////////////////////////////
			/// @brief Changes the position of the window
			///
			/// The given data is used to determine the properties
			/// of the client (drawing) area, not the complete window
			///
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool setPosition(int x,int y);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the position of the window
			///
			/// The returned data describes the client rect
			///
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool getPosition(int &x,int &y) const;


			/////////////////////////////////////////////////////////////
			/// @brief Changes the size of the window
			///
			/// The given data is used to determine the properties
			/// of the client (drawing) area, not the complete window
			///
			/// @param w The width of the window
			/// @param h The height of the window
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool setSize(unsigned int w,unsigned int h);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the size of the window
			///
			/// The returned data describes the client rect
			///
			/// @param w The width of the window
			/// @param h The height of the window
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool getSize(unsigned int &w,unsigned int &h) const;

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
			/// @param title Set to the title
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool getTitle(std::string &title) const;

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
			void enableKeyRepeat(bool enable=true);

			/////////////////////////////////////////////////////////////
			/// @brief Returns whether keyrepeat is enabled
			///
			/// @return True iff enabled
			///
			/////////////////////////////////////////////////////////////
			bool isKeyRepeatEnabled() const;

			/////////////////////////////////////////////////////////////
			/// @brief Get the window's handle
			///
			/// If the window is closed (not opened) NULL is returned
			///
			/// @return The HandleWiNDow
			///
			/////////////////////////////////////////////////////////////
			::Window getHandle() const;

			/////////////////////////////////////////////////////////////
			/// @brief Implicitly convert to HWND
			///
			/// If the window is closed (not opened) NULL is returned
			///
			/// @return The HandleWiNDow
			///
			/////////////////////////////////////////////////////////////
			operator ::Window() const;
		};
	}
}

#endif // FRONTIER_XLIB_WINDOW_INCLUDED
