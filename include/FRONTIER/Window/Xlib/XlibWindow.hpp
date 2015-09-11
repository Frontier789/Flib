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

namespace fg
{
	class Image;
}

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
		/// @brief Used by fw::Xlib::Window for storing DragNDrop related atoms
		///
		/////////////////////////////////////////////////////////////
		class XdndAtoms
		{
		public:
			Atom XdndAware;      ///< Holds the atom named "XdndAware"
			Atom XdndPosition;   ///< Holds the atom named "XdndPosition"
			Atom XdndEnter;      ///< Holds the atom named "XdndEnter"
			Atom XdndStatus;     ///< Holds the atom named "XdndStatus"
			Atom XdndDrop;       ///< Holds the atom named "XdndDrop"
			Atom XdndSelection;  ///< Holds the atom named "XdndSelection"
			Atom XdndFinished;   ///< Holds the atom named "XdndFinished"
			Atom XdndActionCopy; ///< Holds the atom named "XdndActionCopy"
			Atom XdndTypeList;   ///< Holds the atom named "XdndTypeList"
			Atom exchangeAtom;   ///< Holds the atom named "exchangeAtom"

			/////////////////////////////////////////////////////////////
			/// @brief Fills in the atoms' values
			/// 
			/// @param disp The connection to the X server
			/// 
			/////////////////////////////////////////////////////////////
			void init(Display *disp);
		};

		/////////////////////////////////////////////////////////////
		///
		/// 	@brief Class used to open, resize and process events of a window in X
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API Window : public fm::NonCopyable
		{
		public:
			typedef bool (*EventCallback)(Window *,XEvent &);
			
		private:
			std::deque<Event> m_eventQueue; ///< A queue holding the unhandled events
			void processEvent(XEvent &xev); ///< Internal function used to convert xevents
			bool checkDisplay(); ///< Internal function
			bool m_opened;       ///< Indicates whether the window is open
			bool m_enableRepeat; ///< Indicates whether key repeat is enabled
			KeySym m_lastDown;   ///< Holds the last pressed key
			Display *m_disp;     ///< The connection to the x server
			Atom m_delAtom;      ///< The id of the window deletion message
			Atom m_stateAtom;    ///< The id of the window state
			bool m_resizeable;   ///< Indicates whether the window can be resized on the borders
			unsigned int m_prevW;       ///< The width after the last resize
			unsigned int m_prevH;       ///< The height after the last resize
			Atom m_stateHiddenAtom;     ///< The id of the hidden state
			Atom m_maxHorAtom;          ///< The id of the horizontally maximization state
			Atom m_maxVertAtom;         ///< The id of the vertical maximization state
			Atom m_uri_listAtom;        ///< The id of the uri list storing
			bool m_supportUriList;      ///< Indicates whether the dropped data is uri list
			Cursor m_emptyCursor;       ///< Cannot "hide" the cursor so display this empty one instead
			XdndAtoms m_xdndAtoms;      ///< Holds the dragNdrop related atoms
			Xlib::Window *m_parent;     ///< The parent of the window
			mutable ::Window m_win;     ///< The handle of the xwindow
			mutable ::Window m_rootWin; ///< A handle to the root window
			EventCallback m_eventCallback; ///< Holds the handle of the event callback
			void getStateProperties(Atom *&atoms,unsigned long *count) const;  ///< Internal function used to get properties of the window
			std::deque<Xlib::Window *> m_children; ///< Holds the window's children
			XIC m_xic; ///< X inout context
			
		public:
			typedef Window &reference;
			typedef const Window &const_reference;

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
			/// @param parent The owner of the new window
			/// @param container The window which will contain the new window
			///
			/////////////////////////////////////////////////////////////
			Window(int x,int y,unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Xlib::Window *parent = NULL,::Window container = 0);

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
			/// @param parent The owner of the new window
			/// @param container The window which will contain the new window
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool open(int x,int y,unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Xlib::Window *parent = NULL,::Window container = 0);

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
			bool setActive();

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
			bool setTitle(const fm::String &title);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the title of the window
			///
			/// @param title Set to the title
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool getTitle(fm::String &title) const;

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
			/// @brief Set the small and the big icon of the window
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
			operator ::Window() const;

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
			/// @brief Set the event callback
			/// 
			/// This function is called before a event is being handled
			/// The functions should return true iff no further processing is required
			/// 
			/// @param callback The new callback
			/// 
			/////////////////////////////////////////////////////////////
			void setEventCallback(EventCallback callback);
		};
	}
}

#endif // FRONTIER_XLIB_WINDOW_INCLUDED
