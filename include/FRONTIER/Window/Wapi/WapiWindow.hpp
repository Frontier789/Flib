#ifndef FRONTIER_WAPI_WINDOW_INCLUDED
#define FRONTIER_WAPI_WINDOW_INCLUDED
#include <FRONTIER/Window/Event.hpp>
#define FRONTIER_WAPI_WINDOW
#include <windows.h>
#include <string>
#include <queue>

/** The name of the WINDCLASS */
#define FRONTIER_WINDOWS_CLASS_NAME "FLIB_CLASS"
#define FRONTIER_WINDOWS_WCLASS_NAME L"FLIB_CLASS"

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
		class Window
		{
			static unsigned int m_windowCount; ///< The number of windows open
			bool cleanUp();      ///< Iternal function used to free resources
			bool init();         ///< Iternal function used at initialization
			HWND m_hwnd;         ///< The id (handle) of the window
			bool m_showCursor;   ///< Indicates whether the cursor is shown in the window
			bool m_resizeable;   ///< Indicates whehter the window can be resized on the borders
			bool m_enableRepeat; ///< Indicates whether key repeat is enabled
			WPARAM m_lastDown;   ///< Contains the last pressed key
			static LRESULT CALLBACK forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Iternal function that deduces the object and calls handleEvent
			LRESULT handleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Iternal function that handles events of the window
			std::queue<Event> m_eventQueue; ///< A queue holding the unhandled events
			
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
			
			typedef HWND Handle; ///< Used in fw::Window
			
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
			/// Upon iternal error a message is prompted to fw::Wapi::log
			/// 
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// 
			/////////////////////////////////////////////////////////////
			Window(int x,int y,int w,int h,const std::string &title,unsigned int style);
			
			/////////////////////////////////////////////////////////////
			/// @brief Construct the window from its attributes
			/// 
			/// Upon iternal error a message is prompted to fw::Wapi::log
			/// 
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// 
			/////////////////////////////////////////////////////////////
			Window(int x,int y,int w,int h,const std::wstring &title,unsigned int style);
			
			/////////////////////////////////////////////////////////////
			/// @brief (Re)Open the window
			/// 
			/// Upon iternal error a message is prompted to fw::Wapi::log
			/// 
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool open(int x,int y,int w,int h,const std::string &title,unsigned int style);
			
			/////////////////////////////////////////////////////////////
			/// @brief (Re)Open the window
			/// 
			/// Upon iternal error a message is prompted to fw::Wapi::log
			/// 
			/// @param x X position of the window
			/// @param y Y position of the window
			/// @param w Width of the window
			/// @param h Height of the window
			/// @param title Title of the window
			/// @param style Style of the window (see fw::WindowStyle)
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool open(int x,int y,int w,int h,const std::wstring &title,unsigned int style);
			
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
			/// @brief Minimizes the window
			/// 
			/////////////////////////////////////////////////////////////
			void minimize();
			
			/////////////////////////////////////////////////////////////
			/// @brief Maximazes the window
			/// 
			/////////////////////////////////////////////////////////////
			void maximize();
			
			/////////////////////////////////////////////////////////////
			/// @brief Brings window in front
			/// 
			/// @return True if everything went right
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
			void showCursor(bool show=true);
			
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
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setRect(int x,int y,int w,int h);
			
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
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool getRect(int &x,int &y,int &w,int &h);
			
			
			/////////////////////////////////////////////////////////////
			/// @brief Changes the position of the window
			/// 
			/// The given data is used to determine the properties 
			/// of the client (drawing) area, not the complete window
			/// 
			/// @param x The offset on the X axis from the left side
			/// @param y The offset on the Y axis from the upper side
			/// 
			/// @return True if everything went right
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
			/// @return True if everything went right
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
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setSize(int w,int h);
			
			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the size of the window
			/// 
			/// The returned data describes the client rect
			/// 
			/// @param w The width of the window
			/// @param h The height of the window
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool getSize(int &w,int &h) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Set the title of the window
			/// 
			/// @param title The new title
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setTitle(const std::string &title);
			
			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the title of the window
			/// 
			/// @param title Set to the title
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool getTitle(std::string &title) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Set the title of the window
			/// 
			/// @param title The new title
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool setTitle(const std::wstring &title);
			
			/////////////////////////////////////////////////////////////
			/// @brief Retrieve the title of the window
			/// 
			/// @param title Set to the title
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool getTitle(std::wstring &title) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Show or hide the window
			/// 
			/// @param visible If true the window is shown 
			/// 
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			void setVisible(bool visible=true);
			
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
			/// @brief Check if the window is opened
			/// 
			/// @return True if the window is open
			/// 
			/////////////////////////////////////////////////////////////
			bool isOpen() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Retrive the last event
			/// 
			/// If the event queue is empty false is returned and
			/// @a ev is not modified
			/// 
			/// @param ev Set to the last event
			/// 
			/// @return True if there was a event
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
			/// @return True if everything went right
			/// 
			/////////////////////////////////////////////////////////////
			bool waitEvent(Event &ev);
			
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
			/// @return True if enabled	
			/// 
			/////////////////////////////////////////////////////////////
			bool isKeyRepeatEnabled() const;
			
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
