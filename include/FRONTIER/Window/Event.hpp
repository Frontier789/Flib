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
#ifndef FRONTIER_EVENT_HPP_INCLUDED
#define FRONTIER_EVENT_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/Window/Window.hpp>

#define FRONTIER_EVENT
#include <string>
#include <vector>

namespace fg
{
	class Image;
}

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store button enumerations and state check function
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Keyboard
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
			Enter,       ///< The enter ('\\n') key
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
			Tab,         ///< The tab ('\\t') key
			CapsLock,    ///< The caps lock key
			LShift,      ///< The left shift key
			RShift,      ///< The right shift key
			LCtrl,       ///< The left control key
			RCtrl,       ///< The right control key
			LSuper,      ///< The left super (usually a logo like windows') key
			RSuper,      ///< The right super (usually a logo like windows') key
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

		/////////////////////////////////////////////////////////////
		/// @brief Convert a key to a 3d offset vector
		///
		/// @param key The key to convert
		/// @param allowWASD Indicates whether pressing WASD keys should affect return value
		/// @param allowArrows Indicates whether pressing the arrow keys should affect return value
		/// @param allowShiftSpace Indicates whether pressing LeftShift or Space should affect return value
		///
		/// @return The 3d directional vector
		///
		/////////////////////////////////////////////////////////////
		static fm::vec3i keyToDelta(Key key,bool allowWASD = true,bool allowArrows = true,bool allowShiftSpace = true);
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to store a mouse corsor
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API MouseCursor
	{
		void *m_impl; ///< System dependent implementation
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param img The image to use
		/// @param clickSpot The position where the mouse clicks
		/// @param transparencyLimit The limit below the texel is considered transparent
		///
		/////////////////////////////////////////////////////////////
		MouseCursor(const fg::Image &img,
					fm::vec2s clickSpot = fm::vec2s(),
					fm::Uint8 transparencyLimit = 128);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~MouseCursor();
		
		/////////////////////////////////////////////////////////////
		/// @brief Load an image 
		///
		/// @param img The image to use
		/// @param clickSpot The position where the mouse clicks
		/// @param transparencyLimit The limit below the texel is considered transparent
		///
		/////////////////////////////////////////////////////////////
		void loadFromImage(const fg::Image &img,
						   fm::vec2s clickSpot = fm::vec2s(),
						   fm::Uint8 transparencyLimit = 128);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set this cursor as current
		///
		/////////////////////////////////////////////////////////////
		void setAsCurrent() const;
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store button enumerations and state check function
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Mouse
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

		/////////////////////////////////////////////////////////////
		/// @brief Enumerates the known mouse cursors
		///
		/////////////////////////////////////////////////////////////
		enum Cursor {
			Arrow,           ///< The default arrow cursor
			IBeam,           ///< The text entering cursor
			Click,           ///< The clickable cursor
			No,              ///< The forbidding cursor
			Loading,         ///< The loading cursor
			SizeTop,         ///< The sizer cursor to top side
			SizeLeft,        ///< The sizer cursor to left side
			SizeBottom,      ///< The sizer cursor to bottom side
			SizeRight,       ///< The sizer cursor to right side
			SizeTopRight,    ///< The sizer cursor to top right corner
			SizeTopLeft,     ///< The sizer cursor to top left corner
			SizeBottomLeft,  ///< The sizer cursor to bottom left corner
			SizeBottomRight, ///< The sizer cursor to bottom right corner
			CursorCount      ///< The number of cursors
		};

		/////////////////////////////////////////////////////////////
		/// @brief Change the cursor
		///
		/// @param cursor The new cursor
		///
		/////////////////////////////////////////////////////////////
		static void setCursor(Mouse::Cursor cursor);

		/////////////////////////////////////////////////////////////
		/// @brief Change the cursor
		///
		/// @param cursor The new cursor
		///
		/////////////////////////////////////////////////////////////
		static void setCursor(const MouseCursor &cursor);

		/////////////////////////////////////////////////////////////
		/// @brief Get the cursor's distance from the upper left corner
		///
		/// @return Position of the cursor
		///
		/////////////////////////////////////////////////////////////
		static fm::vec2i getPosition();

		/////////////////////////////////////////////////////////////
		/// @brief Get the cursor's distance from the upper left corner of a specific window
		///
		/// @param window The target window
		///
		/// @return Position of the cursor
		///
		/////////////////////////////////////////////////////////////
		static fm::vec2i getPosition(const fw::priv::Window &window);

		/////////////////////////////////////////////////////////////
		/// @brief Get the cursor's distance from the upper left corner of a specific window
		///
		/// @param window The target window
		///
		/// @return Position of the cursor
		///
		/////////////////////////////////////////////////////////////
		static fm::vec2i getPosition(const fw::Window &window);

		/////////////////////////////////////////////////////////////
		/// @brief Set the cursor's position
		///
		/// The position is relative to the upper left corner
		///
		/// @param pos The new position of the cursor
		///
		/////////////////////////////////////////////////////////////
		static void setPosition(const fm::vec2i &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Set the cursor's position relative to a specific window
		///
		/// The position is relative to the upper left corner
		///
		/// @param pos The new position of the cursor
		/// @param window The target window
		///
		/////////////////////////////////////////////////////////////
		static void setPosition(const fm::vec2i &pos,const fw::priv::Window &window);

		/////////////////////////////////////////////////////////////
		/// @brief Set the cursor's position relative to a specific window
		///
		/// The position is relative to the upper left corner
		///
		/// @param pos The new position of the cursor
		/// @param window The target window
		///
		/////////////////////////////////////////////////////////////
		static void setPosition(const fm::vec2i &pos,const fw::Window &window);
	};

	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Simple class used to store data about KeyPressed KeyReleased events
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API KeyboardEvent
	{
	public:
		bool shift; ///< Indicates that shift was held when the event occured
		bool ctrl;  ///< Indicates that control was held when the event occured
		bool alt;   ///< Indicates that alt was held when the event occured
		Keyboard::Key code; ///< Identifies the key
	};

	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store data about ButtonPressed ButtonReleased events
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ButtonEvent
	{
	public:
		int x; ///< X coordinate of the cursor
		int y; ///< Y coordinate of the cursor
		Mouse::Button button; ///< Identifies the mouse button
		
		template<class T>
		operator fm::vector2<T>() const; ///< convert to vector2
	};

	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store data about MouseMoved events
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API MotionEvent
	{
	public:
		int x;  ///< X coordinate of the cursor
		int y;  ///< Y coordinate of the cursor

		template<class T>
		operator fm::vector2<T>() const; ///< convert to vector2
	};

	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store data about a MouseWheelMoved event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API MouseWheelEvent
	{
	public:
		int x; ///< X coordinate of the cursor
		int y; ///< Y coordinate of the cursor
		bool shift;  ///< Indicates that shift was held when the event occured
		bool ctrl;   ///< Indicates that control was held when the event occured
		bool alt;    ///< Indicates that alt was held when the event occured
		float delta; ///< The amount the wheel moved (+1 means it was rotated once, forward, away from the user)
		bool horizontal; ///< True for horizontal scroll, false for vertical scroll
		
		template<class T>
		operator fm::vector2<T>() const; ///< convert to vector2
	};

	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store data about a Resized event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ResizeEvent
	{
	public:
		unsigned int w; ///< The new width
		unsigned int h; ///< The new height

		template<class T>
		operator fm::vector2<T>() const; ///< convert to vector2
	};

	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store data about a TextEntered event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API TextEvent
	{
	public:
		char character;     ///< The entered character
		fm::Uint32 utf8character; ///< The entered character (utf8)
	};

	/////////////////////////////////////////////////////////////
	/// @brief Simple class used to store data about a FileDrop event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API DropEvent
	{
	public:
		int x; ///< X coordinate of the cursor
		int y; ///< Y coordinate of the cursor
		std::vector<std::string> files; ///< The name(s) of file(s)

		template<class T>
		operator fm::vector2<T>() const; ///< convert to vector2
	};

	/////////////////////////////////////////////////////////////
	/// @brief Class used to store properties of a window event
	///
	/// @ingroup Window
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Event
	{
	public:
		typedef Event &reference;
		typedef const Event &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Indicates what field of the event is present
		///
		/////////////////////////////////////////////////////////////
		enum EventType {
			Empty,           ///< The event is not filled
			Closed,          ///< The window is closed
			Resized,         ///< The window's size is changed
			FocusLost,       ///< The window lost focus
			FocusGained,     ///< The window gained focus
			KeyPressed,      ///< A key is pressed (The key field can be used to track further information)
			KeyReleased,     ///< A key is released (The key field can be used to track further information)
			ButtonPressed,   ///< A mouse button is pressed (The mouse field can be used to track further information)
			ButtonReleased,  ///< A mouse button is released (The mouse field can be used to track further information)
			TextEntered,     ///< A character is entered (not the same as KeyPressed)
			MouseWheelMoved, ///< The mouse wheel was moved (The wheel field can be used to track further information)
			MouseMoved,      ///< The cursor is moved inside the window is released (The motion field can be used to track further information)
			FileDrop,        ///< The user dropped file(s) into the window
			MouseLeft,       ///< The mouse left the window (the motion field can be used to track further information)
			MouseEntered     ///< The mouse entered the window (the motion field can be used to track further information)
		};

		EventType type; ///< The type of the event

		/////////////////////////////////////////////////////////////
		/// @brief Holds the fields of the event
		///
		/// We only use one field at a time so union is sufficient
		///
		/////////////////////////////////////////////////////////////
		union {
			KeyboardEvent   key;    ///< used with KeyPressed, KeyReleased events
			ButtonEvent     mouse;  ///< used with ButtonPressed, ButtonReleased events
			MotionEvent     motion; ///< used with MouseMoved, MouseEntered, MouseLeft events
			ResizeEvent     size;   ///< used with Resize event
			TextEvent       text;   ///< used with TextEntered event
			MouseWheelEvent wheel;  ///< used with MouseWheelMoved event
		};

		DropEvent drop;  ///< used with FileDrop event

		/////////////////////////////////////////////////////////////
		/// @brief Capable of holding data related to a event
		///
		/////////////////////////////////////////////////////////////
		class EventData
		{
		public:
			union {
				KeyboardEvent   key;    ///< used with KeyPressed KeyReleased events
				ButtonEvent     mouse;  ///< used with ButtonPressed ButtonReleased events
				MotionEvent     motion; ///< used with MouseMoved, MouseEntered, MouseLeft event
				ResizeEvent     size;   ///< used with Resize event
				TextEvent       text;   ///< used with TextEntered
				MouseWheelEvent wheel;  ///< used with MouseWheelMoved event
			};

			DropEvent drop; ///< used with FileDrop event

			EventData(KeyboardEvent key);     ///< Set up the union to hold data for KeyPressed KeyReleased events
			EventData(ButtonEvent mouse);     ///< Set up the union to hold data for ButtonPressed ButtonReleased events
			EventData(MotionEvent motion);    ///< Set up the union to hold data for MouseMoved event
			EventData(ResizeEvent size);      ///< Set up the union to hold data for Resize event
			EventData(TextEvent text);        ///< Set up the union to hold data for TextEntered
			EventData(MouseWheelEvent wheel); ///< Set up the union to hold data for MouseWheelMoved event
			EventData(DropEvent drop);        ///< Set up the union to hold data for FileDrop event
		};

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Event();

		/////////////////////////////////////////////////////////////
		/// @brief Initialize event with a type
		///
		/// @param type The initial type
		///
		/////////////////////////////////////////////////////////////
		Event(EventType type);

		/////////////////////////////////////////////////////////////
		/// @brief Initialize event with a type
		///
		/// @param type The initial type
		/// @param data The initial data of the event
		///
		/////////////////////////////////////////////////////////////
		Event(EventType type,EventData data);

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

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Window/Event.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
