//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_EVENT_HPP_INCLUDED
#define FRONTIER_EVENT_HPP_INCLUDED

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Simple class used to store button enumerations and state check function
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class Keyboard
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Enumerates the known keyboard keys
		///
		/////////////////////////////////////////////////////////////
		enum Key {
			A,           ///< The 'a' key
			B,           ///< The 'b' key
			C,           ///< The 'c' key
			D,           ///< The 'd' key
			E,           ///< The 'e' key
			F,           ///< The 'f' key
			G,           ///< The 'g' key
			H,           ///< The 'h' key
			I,           ///< The 'i' key
			J,           ///< The 'j' key
			K,           ///< The 'k' key
			L,           ///< The 'l' key
			M,           ///< The 'm' key
			N,           ///< The 'n' key
			O,           ///< The 'o' key
			P,           ///< The 'p' key
			Q,           ///< The 'q' key
			R,           ///< The 'r' key
			S,           ///< The 's' key
			T,           ///< The 't' key
			U,           ///< The 'u' key
			V,           ///< The 'v' key
			W,           ///< The 'w' key
			X,           ///< The 'x' key
			Y,           ///< The 'y' key
			Z,           ///< The 'z' key
			Up,          ///< The up key
			Left,        ///< The left key
			Down,        ///< The down key
			Right,       ///< The right key
			Escape,      ///< The escape key (Esc) key
			Enter,       ///< The enter ('\n') key
			PrintScreen, ///< The print screen key
			ScrollLock,  ///< The scrollock key
			PauseBreak,  ///< The pause break key
			Backspace,   ///< The backspace key
			Insert,      ///< The insert key
			Delete,      ///< The delete back key
			Home,        ///< The home key
			End,         ///< The end key
			PageUp,      ///< The page up (pgup) key
			PageDown,    ///< The page down (pgdn) key
			Divide,      ///< The divide ('/') key
			Multiply,    ///< The multiply ('*') key
			Minus,       ///< The minus ('-') key
			Plus,        ///< The plus ('+') key
			Comma,       ///< The comma (',') key
			Tab,         ///< The tab ('\t') key
			CapsLock,    ///< The caps lock key
			LShift,      ///< The left shift key
			RShift,      ///< The right shift key
			LCtrl,       ///< The left control key
			RCtrl,       ///< The right control key
			LWindows,    ///< The left windows key
			RWindows,    ///< The right windows key
			Print,       ///< The print key
			LAlt,        ///< The left alt key
			RAlt,        ///< The right alt key
			Space,       ///< The Space (' ') key
			F1,          ///< The F1 key
			F2,          ///< The F2 key
			F3,          ///< The F3 key
			F4,          ///< The F4 key
			F5,          ///< The F5 key
			F6,          ///< The F6 key
			F7,          ///< The F7 key
			F8,          ///< The F8 key
			F9,          ///< The F9 key
			F10,         ///< The F10 key
			F11,         ///< The F11 key
			F12,         ///< The F12 key
			Num0,        ///< The '0' key 
			Num1,        ///< The '1' key 
			Num2,        ///< The '2' key 
			Num3,        ///< The '3' key 
			Num4,        ///< The '4' key 
			Num5,        ///< The '5' key 
			Num6,        ///< The '6' key 
			Num7,        ///< The '7' key 
			Num8,        ///< The '8' key 
			Num9,        ///< The '9' key
			Numpad0,     ///< The '0' key on the numpad
			Numpad1,     ///< The '1' key on the numpad
			Numpad2,     ///< The '2' key on the numpad
			Numpad3,     ///< The '3' key on the numpad
			Numpad4,     ///< The '4' key on the numpad
			Numpad5,     ///< The '5' key on the numpad
			Numpad6,     ///< The '6' key on the numpad
			Numpad7,     ///< The '7' key on the numpad
			Numpad8,     ///< The '8' key on the numpad
			Numpad9,     ///< The '9' key on the numpad
			KeyCount,    ///< The number of keys
			Unknown      ///< The key is not known
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Check key status
		///
		/// @param key The key
		///
		/// @return True if the key is held
		///
		/////////////////////////////////////////////////////////////
		static bool isKeyHeld(Key key);
	};

	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Simple class used to store button enumerations and state check function
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class Mouse
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Enumerates the known mouse buttons
		///
		/////////////////////////////////////////////////////////////
		enum Button {
			Left,        ///< The left mouse button
			Right,       ///< The right mouse button
			Middle,      ///< The middle mouse button
			ButtonCount, ///< The number of mouse buttons
			Unknown      ///< The mouse button is not known
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Check button status
		/// 
		/// @param button The button
		///
		/// @return True if the button is held
		///
		/////////////////////////////////////////////////////////////
		static bool isButtonHeld(Button button);
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
	/// 	@brief Simple class used to store data about a Resize event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class ResizeEvent
	{
	public:
		int w; ///< new width
		int h; ///< new height
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
			Resize,         ///< The window's size is changed
			Closed,         ///< The window is closed
			Minimize,       ///< The window was asked to minimize
			Maximize,       ///< The window was asked to maximize
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
			ResizeEvent   size;  ///< used with Resize event
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
