#ifndef FRONTIER_WINDOW_HPP_INCLUDED
#define FRONTIER_WINDOW_HPP_INCLUDED

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief Wrapper class used to manage windows os independently
	/// 
	/// @ingroup Window
	/// 
	/////////////////////////////////////////////////////////////
	class Window
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Enumeration used to indicate window style
		/// 
		/// @ingroup Window
		/// 
		/////////////////////////////////////////////////////////////
		enum WindowStyle {
		None       = 0x0000000,
		Close      = 0x0000001,
		Border     = 0x0000010,
		Resize     = 0x0000100,
		Minimize   = 0x0001000,
		Maximaze   = 0x0010000,
		Titlebar   = 0x0100000,
		Fullscreen = 0x1000000,
		Default    = 0x0111111
		};
	};
}

#endif // FRONTIER_WINDOW_HPP_INCLUDED
