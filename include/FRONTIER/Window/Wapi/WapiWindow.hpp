#ifndef FRONTIER_WAPI_WINDOW_INCLUDED
#define FRONTIER_WAPI_WINDOW_INCLUDED
#define FRONTIER_WAPI_WINDOW
#include <windows.h>

/** The name of the WINDCLASS */
#define FRONTIER_WINDOWS_CLASS_NAME "FLIB_CLASS"

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
			bool cleanUp(); ///< Iternal function used to free resources
			bool init();    ///< Iternal function used at initialization
			HWND m_hwnd;    ///< The id (handle) of the window
			static LRESULT CALLBACK forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); ///< Iternal function that handles Windows api events
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
			Window(int x,int y,int w,int h,const char *title,unsigned int style);
			
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
			bool open(int x,int y,int w,int h,const char *title,unsigned int style);
			
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
