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
#ifndef FRONTIER_WAPI_WINDOW_INCLUDED
#define FRONTIER_WAPI_WINDOW_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Window/Event.hpp>
#define FRONTIER_WAPI_WINDOW

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#elif _WIN32_WINNT < 0x0500
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif

#include <windows.h>
#include <string>
#include <deque>

/** The name of the WINDCLASS */
#define FRONTIER_WINDOWS_CLASS_NAME "FLIB_CLASS"
#define FRONTIER_WINDOWS_WCLASS_NAME L"FLIB_CLASS"

namespace fg
{
	class Image;
}

namespace fw
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Windows-only classes are held in this namespace
	///
	/////////////////////////////////////////////////////////////
	namespace Wapi
	{
		/////////////////////////////////////////////////////////////
		///
		/// 	@brief Class used to open, resize and process events of a window in winOS
		///
		/////////////////////////////////////////////////////////////
		class FRONTIER_API Window : public fm::NonCopyable
		{
		public:
			typedef bool (*EventCallback)(Window *,unsigned int,fm::UintPtr,fm::IntPtr,fm::IntPtr*);
			
		private:
			static unsigned int m_windowCount; ///< The number of windows open
			bool cleanUp();      ///< Internal function used to free resources
			bool init();         ///< Internal function used at initialization
			HWND m_hwnd;         ///< The id (handle) of the window
			bool m_showCursor;   ///< Indicates whether the cursor is shown in the window
			bool m_resizeable;   ///< Indicates whether the window can be resized on the borders
			bool m_enableRepeat; ///< Indicates whether key repeat is enabled
			bool m_cursorInside; ///< True iff the mouse is inside the window
			bool m_acceptDrop;   ///< True iff files can be dropped into the window
			WPARAM m_lastDown;   ///< Contains the last pressed key
			LONG m_style;        ///< Internal variable used when going to fullscreen
			LONG m_exStyle;      ///< Internal variable used when going to fullscreen
			HICON m_icon;        ///< The last set icon
			EventCallback m_eventCallback; ///< Holds the handle of the event callback

			static LRESULT CALLBACK forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Internal function that deduces the object and calls handleEvent
			LRESULT handleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Internal function that handles events of the window
			std::deque<Event> m_eventQueue; ///< A queue holding the unhandled events

			/////////////////////////////////////////////////////////////
			/// @brief User defined hittest
			///
			/// See <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms645618(v=vs.85).aspx">this msdn article</a>
			/// for what to return
			///
			/////////////////////////////////////////////////////////////
			LRESULT (*m_cursorHitTest)(const POINT &mousePos,
									   const RECT  &winRect,
									   const RECT  &clientRect,
									   bool resizeable,
									   const LRESULT &defResult);
		public:
			typedef Window &reference;
			typedef const Window &const_reference;

			typedef HWND Handle; ///< The window handle type

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
			/// Upon internal error a message is prompted to fw::fw_log 
			///
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// @param toolkit True indicates that the window needs no indication in the taskbar
			///
			/////////////////////////////////////////////////////////////
			Window(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style,HWND parent = 0);

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
			/// Upon internal error a message is prompted to fw::fw_log 
			///
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// @param toolkit True indicates that the window needs no indication in the taskbar
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool open(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style,HWND parent = 0);
			
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
			/// @brief Set the title of the window
			///
			/// @param title The new title
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool setTitle(const std::wstring &title);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the title of the window
			///
			/// @param title Set to the title
			///
			/// @return True iff everything went right
			///
			/////////////////////////////////////////////////////////////
			bool getTitle(std::wstring &title) const;

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
			/// @brief Set the cursor hittest function
			///
			/// NULL means that the default hittest will be used
			///
			/// The function @a hitTestFunc recieves the mouse position (POINT)
			/// The window rect (RECT) the client rect (RECT) and the value
			/// that would be returned by default (LRESULT)
			///
			/// @param hitTestFunc The new hittest function
			///
			/////////////////////////////////////////////////////////////
			void setCursorHitTest(LRESULT (*hitTestFunc)(const POINT&,const RECT&,const RECT&,bool,const LRESULT&));

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
			/// @brief Enables/disables files to be dropped in the window
			///
			/////////////////////////////////////////////////////////////
			void enableDrop(bool enable = true);
			
			/////////////////////////////////////////////////////////////
			/// @brief Find out if dropping files into the window is enabled
			/// 
			/// @return True iff dropping is enabled
			/// 
			/////////////////////////////////////////////////////////////
			bool isDropEnabled() const;

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
			/// @brief Makes the window (non)fullscreen
			///
			/// Making a fullscreen window fullscreen or making a nonfullscreen
			/// one nonfullscreen have no effect
			///
			/// Having more than one fullscreen window may cause platform-specific problems
			///
			/// If @a width x @a height should be a valid monitor resolution
			///
			/// @return True iff the operation was successful
			///
			/////////////////////////////////////////////////////////////
			bool setFullscreen(unsigned int width,unsigned int height,bool fullscreen = true);

			/////////////////////////////////////////////////////////////
			/// @brief Set the small and the big icon of the window
			///
			/// @param icon The new icon
			///
			/// @return True iff the operation was successful
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
			operator HWND() const;

			/////////////////////////////////////////////////////////////
			/// @brief Get the window's handle
			///
			/// If the window is closed (not opened) NULL is returned
			///
			/// @return The HandleWiNDow
			///
			/////////////////////////////////////////////////////////////
			HWND getHandle() const;

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
		////////////////////////////////////////////////////////////
		/// @class fw::Wapi::Window
		/// @ingroup Window
		///
		/// This class is used to open and manage windows in winOS.
		/// By manage i mean process its events, resize, move, set its title etc
		///
		/// fw::Wapi::Window creates and maintains a WNDCLASS named FRONTIER_WINDOWS_CLASS_NAME (macro)
		///
		/// Including this file directly will cause the loss platform independency (winOS only)
		///
		/// Example:
		/// @code
		/// fw::Wapi::Window win;
		/// win.open(50,50,640,480,"test_window",fw::Default&~fw::Resize);
		///
		/// win.setPosition(150,150);
		/// win.setSize(500,500);
		/// @endcode
		///
		///
		/// @see fw::Window
		///
		////////////////////////////////////////////////////////////
	}
}

#endif // FRONTIER_WAPI_WINDOW_INCLUDED
