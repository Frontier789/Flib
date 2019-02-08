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
#ifndef FRONTIER_WAPI_WINDOW_INCLUDED
#define FRONTIER_WAPI_WINDOW_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/Window/Event.hpp>

#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
	#include <mutex>
#endif

#define FRONTIER_WAPI_WINDOW

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#elif _WIN32_WINNT < 0x0500
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif

#include <windows.h>
#include <deque>

/** The name of the WINDCLASS */
#define FRONTIER_WINDOWS_CLASS_NAME "FLIB11_CLASS"
#define FRONTIER_WINDOWS_WCLASS_NAME L"FLIB11_CLASS"

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
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			static std::mutex m_windowCountMutex;      ///< Protects the m_windowCount to be accessed at the same time
		#endif
			static unsigned int m_windowCount;        ///< The number of windows open
			void cleanUp();      ///< Internal function used to free resources
			fm::Result createClass();  ///< Internal function used at initialization
			HWND m_hwnd;         ///< The id (handle) of the window
			bool m_showCursor;   ///< Indicates whether the cursor is shown in the window
			bool m_resizeable;   ///< Indicates whether the window can be resized on the borders
			bool m_enableRepeat; ///< Indicates whether key repeat is enabled
			bool m_cursorInside; ///< True iff the mouse is inside the window
			bool m_acceptDrop;   ///< True iff files can be dropped into the window
			LONG m_style;        ///< Internal variable used when going to fullscreen
			LONG m_exStyle;      ///< Internal variable used when going to fullscreen
			HICON m_icon;        ///< The last set icon
			bool m_isOpened;     ///< True iff the window opened successfully
			EventCallback m_eventCallback;   ///< Holds the handle of the event callback
			HWND m_ownedParent;     ///< Hidden parent (used when no taskbar indication is needed)
			Wapi::Window *m_parent; ///< The parent of the window
			bool m_decorActive;     ///< Indicates whether the window decoration is active
			std::deque<Wapi::Window *> m_children; ///< The handles of the windows whose parent is this window
			bool m_inSizeMoveMode; ///< True when the user is sizing or moving the window by hand
			bool m_windowMoved;    ///< True if the user moved the window

			static LRESULT CALLBACK forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Internal function that deduces the object and calls handleEvent
			LRESULT handleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Internal function that handles events of the window
			std::deque<Event> m_eventQueue; ///< A queue holding the unhandled events
			
			bool recFind(Wapi::Window *ancestor,HWND hwnd); ///< Internal function used to determine if @a ancestor is the ancestor of @a hwnd
			
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
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// @param parent The parent of the window (when the parent is deactivated so is the child)
			/// @param container The window that contains this (0 for nobody)
			///
			/////////////////////////////////////////////////////////////
			Window(int x,int y,unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Wapi::Window *parent = NULL,HWND container = 0);

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
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// @param parent The parent of the window (when the parent is deactivated so is the child)
			/// @param container The window that contains this (0 for nobody)
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result open(int x,int y,unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Wapi::Window *parent = NULL,HWND container = 0);
			

			/////////////////////////////////////////////////////////////
			/// @brief (Re)Open the window
			///
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// @param parent The parent of the window (when the parent is deactivated so is the child)
			/// @param container The window that contains this (0 for nobody)
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result open(unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Wapi::Window *parent = NULL,HWND container = 0);
			

			/////////////////////////////////////////////////////////////
			/// @brief Change the window that contains this window
			///
			/// @param container The window that contains this (0 for nobody)
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setContainer(HWND container);
			
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
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setActive();

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
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setRect(int x,int y,unsigned int w,unsigned int h);

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
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result getRect(int &x,int &y,unsigned int &w,unsigned int &h);


			/////////////////////////////////////////////////////////////
			/// @brief Changes the position of the window
			///
			/// The given data is used to determine the properties
			/// of the client (drawing) area, not the complete window
			///
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setPosition(int x,int y);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the position of the window
			///
			/// The returned data describes the client rect
			///
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result getPosition(int &x,int &y) const;


			/////////////////////////////////////////////////////////////
			/// @brief Changes the size of the window
			///
			/// The given data is used to determine the properties
			/// of the client (drawing) area, not the complete window
			///
			/// @param w The width of the window
			/// @param h The height of the window
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setSize(unsigned int w,unsigned int h);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the size of the window
			///
			/// The returned data describes the client rect
			///
			/// @param w The width of the window
			/// @param h The height of the window
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result getSize(unsigned int &w,unsigned int &h) const;

			/////////////////////////////////////////////////////////////
			/// @brief Set the title of the window
			///
			/// @param title The new title
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setTitle(const fm::String &title);

			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the title of the window
			///
			/// @param title Set to the title
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result getTitle(fm::String &title) const;

			/////////////////////////////////////////////////////////////
			/// @brief Show or hide the window
			///
			/// @param visible If true the window is shown
			///
			/// @return The result of the call
			///
			/////////////////////////////////////////////////////////////
			fm::Result setVisible(bool visible = true);

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
			/// @return True iff no error occured
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
	}
}

#endif // FRONTIER_WAPI_WINDOW_INCLUDED
