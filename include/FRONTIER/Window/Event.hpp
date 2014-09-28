#ifndef FRONTIER_EVENT_HPP_INCLUDED
#define FRONTIER_EVENT_HPP_INCLUDED

namespace fw
{
	class Keyboard
	{
	public:
		enum Key {
			KeyCount,
			Unknown
		};
	};

	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Simple class used to store data about a MouseMoved event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class Mouse
	{
	public:
		enum Button {
			ButtonCount,
			Unknown
		};
	};

	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Simple class used to store data about KeyPressed KeyReleased events
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class KeyboardEvent
	{
	public:
		Keyboard::Key code; ///< Identifies the key
		bool shift; ///< Indicates that shift was held when the event occured
		bool ctrl;  ///< Indicates that control was held when the event occured
		bool alt;   ///< Indicates that alt was held when the event occured
	};

	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Simple class used to store data about ButtonPressed ButtonReleased events
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class ButtonEvent
	{
	public:
		Mouse::Button button; ///< Identifies the mouse button
		int x; ///< X coordinate of the cursor
		int y; ///< Y coordinate of the cursor
	};
	
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Simple class used to store data about a MouseMoved event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class MouseEvent
	{
	public:
		int x; ///< X coordinate of the cursor
		int y; ///< Y coordinate of the cursor
	};
	
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Class used to store properties of a window event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class Event
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Indicates what field of the event is present
		///
		/////////////////////////////////////////////////////////////
		enum EventType {
			Empty,          ///< The event is not filled
			Closed,         ///< The window is closed
			FocusLost,      ///< The window lost focus
			FocusGained,    ///< The window gained focus
			KeyPressed,     ///< A key is pressed (The key field can be used to track further information)
			KeyReleased,    ///< A key is released (The key field can be used to track further information)
			ButtonPressed,  ///< A mouse button is pressed (The mouse field can be used to track further information)
			ButtonReleased, ///< A mouse button is released (The mouse field can be used to track further information)
			MouseMoved      ///< The cursor is moved inside the window is released (The pos field can be used to track further information)
		};
		
		EventType type; ///< The type of the event
		
		/////////////////////////////////////////////////////////////
		/// @brief Holds the fileds of the event
		///
		/// We only use one field at a time so union is sufficent
		///
		/////////////////////////////////////////////////////////////
		union {
			KeyboardEvent key;   ///< used with KeyPressed KeyReleased events
			ButtonEvent   mouse; ///< used with ButtonPressed ButtonReleased events
			MouseEvent    pos;   ///< used with MouseMoved event
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Event();
		
		/////////////////////////////////////////////////////////////
		/// @brief Use the event as a bool
		/// 
		/// @return True if the event is not empty
		/// 
		/////////////////////////////////////////////////////////////
		operator bool() const;
	};
}

#endif // FRONTIER_EVENT_HPP_INCLUDED
