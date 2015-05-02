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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_BUTTON_HPP_INCLUDED
#define FRONTIER_BUTTON_HPP_INCLUDED
#include <FRONTIER/System/macros/dont_include_inl_begin>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/dont_include_inl_end>
#define FRONTIER_BUTTON
#include <FRONTIER/Gui/Widget.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief A base class of all button classes
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Button : public Widget
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "Button"
		
	protected:
		void (*m_callback)(Button *,fm::IntPtr); ///< The function that gets called when the button is activated
		fm::IntPtr m_userData; ///< The user secified data passed to the callback

		bool m_cursorInside; ///< Indicates whether the cursor is inside the button
		bool m_grabbedMouse; ///< Indicates whether the mouse input is grabbed

		/////////////////////////////////////////////////////////////
		/// @brief Called when the cursor enters/leaves the button
		/// 
		/// @param on True on enter, false on leave
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onHover(bool on);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the left button is pressed and the cursor is inside the button or the mouse was grabbed and the left button is released
		/// 
		/// @param on True on press, false on release
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onClick(bool on);
		
		/////////////////////////////////////////////////////////////
		/// @brief Tests whther a point is inside the button
		/// 
		/// @param p The point
		/// 
		/// @return True iff the point is inside
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool contains(const fm::vec2 &p);
		
	public:
		typedef Button &reference;
		typedef const Button &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param name The identifier of the widget
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		/// @param callback The function that gets called when the button is activated
		/// @param userData The user secified data passed to the callback
		/// 
		/////////////////////////////////////////////////////////////
		Button(const std::string &name = "NoName",
			   const Anchor &anchor = Anchor(),
			   const fm::vec2 &size = fm::vec2(),
			   Widget *parent = fm::nullPtr,
			   void (*callback)(Button *,fm::IntPtr) = fm::nullPtr,
			   const fm::IntPtr &userData = 0x0);
		

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		virtual ~Button();

		/////////////////////////////////////////////////////////////
		/// @brief Handle an event
		/// 
		/// Automatically handles events and calls onHover and onClick when needed
		/// 
		/// @param ev The event descriptor
		/// 
		/// @param True if the event is not to be passed to the next widget
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(const fw::Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Set the callback
		/// 
		/// @param callback The new callback
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setCallback(void (*callback)(Button *,fm::IntPtr));

		/////////////////////////////////////////////////////////////
		/// @brief Get the current callback
		/// 
		/// @return The current callback
		/// 
		/////////////////////////////////////////////////////////////
		virtual void (*getCallback())(Button *,fm::IntPtr);

		/////////////////////////////////////////////////////////////
		/// @brief Set the user data
		/// 
		/// @param userData The new user data
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setUserData(fm::IntPtr userData);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current user data
		/// 
		/// @return The current user data
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::IntPtr getUserData();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the real type of the class
		/// 
		/// When creating a new class it may override this 
		/// function to provide a unique name as return value
		/// 
		/////////////////////////////////////////////////////////////
		virtual const std::string &getRealType() const;
	};
}


#endif // FRONTIER_ANCHOR_HPP_INCLUDED
