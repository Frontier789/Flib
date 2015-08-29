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
#ifndef FRONTIER_WIDGET_HPP_INCLUDED
#define FRONTIER_WIDGET_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Gui/Anchor.hpp>
#define FRONTIER_WIDGET
#include <string>
#include <deque>

namespace fw
{
	class Event;
}

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief The base class of all gui elements
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Widget
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "Widget"

	protected:
		std::string m_name; ///< The identifier of the instance
		Anchor m_anchor;    ///< The anchor of the widget
		fm::vec2 m_size;    ///< The size of the widget
		Widget *m_parent;   ///< The parent of the widget
		bool m_enabled;     ///< True when the widget is enabled

	public:
		typedef Widget &reference;
		typedef const Widget &const_reference;

		static std::deque<Widget *> allWidgets; ///< A deque containing a pointer to all widgets (only for debugging)

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param name The identifier of the instance
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		///
		/////////////////////////////////////////////////////////////
		Widget(const std::string &name = "NoName",const Anchor &anchor = Anchor(),const fm::vec2 &size = fm::vec2(),Widget *parent = fm::nullPtr);

		/////////////////////////////////////////////////////////////
		/// @brief Default copy constructor
		///
		/// @param copy The widget to be copied
		///
		/////////////////////////////////////////////////////////////
		Widget(const Widget &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~Widget();

		/////////////////////////////////////////////////////////////
		/// @brief Set the anchor of the widget
		///
		/// @param anchor The new anchor
		///
		/////////////////////////////////////////////////////////////
		virtual void setAnchor(const Anchor &anchor);

		/////////////////////////////////////////////////////////////
		/// @brief Get the anchor of the widget
		///
		/// @return The anchor
		///
		/////////////////////////////////////////////////////////////
		virtual const Anchor &getAnchor() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the parent of the widget
		///
		/// @param parent The new parent
		///
		/////////////////////////////////////////////////////////////
		virtual void setParent(Widget *parent);

		/////////////////////////////////////////////////////////////
		/// @brief Get the parent of the widget
		///
		/// @return The parent
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *getParent() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the widget
		///
		/// @param size The new size
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the widget
		///
		/// @return The size
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the name of the widget
		///
		/// @param size The new name
		///
		/////////////////////////////////////////////////////////////
		virtual void setName(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Get the name of the widget
		///
		/// @return The name
		///
		/////////////////////////////////////////////////////////////
		virtual std::string getName() const;

		/////////////////////////////////////////////////////////////
		/// @brief Enable or disable widget
		///
		/// Disabled widgets shouldn't process any events
		///
		/// @param enabled True to enable false to disable
		///
		/////////////////////////////////////////////////////////////
		virtual void setEnabled(bool enabled);

		/////////////////////////////////////////////////////////////
		/// @brief Find out if the widget is enabled or disabled
		///
		/// @return true if enabled false if disabled
		///
		/////////////////////////////////////////////////////////////
		virtual bool getEnabled() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the active widget
		///
		/// By default this function forwards to this widget's parent
		///
		/// @param active The new active widget
		///
		/////////////////////////////////////////////////////////////
		virtual void setActive(Widget *active);

		/////////////////////////////////////////////////////////////
		/// @brief Get the active widget
		///
		/// @return The active widget
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *getActive();

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the parent of the widget
		///
		/// If the widget does not have a parent vec2() is returned
		///
		/// @return The size of the parent
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getParentSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the root of the widget
		///
		/// If the widget does not have a parent the
		/// size of the widget itself is returned
		///
		/// @return The size of the root
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getRootSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the position of the widget in its parent
		///
		/// If the widget does not have a parent vec2() is returned
		///
		/// @return The position in the parent
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getPosInParent() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the position of the widget in its root
		///
		/// If the widget does not have a parent vec2() is returned
		///
		/// @return The position in the root
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getPosInRoot() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the root of the widget
		///
		/// @return The root (may be NULL)
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *getRoot();

		/////////////////////////////////////////////////////////////
		/// @brief Search the owned widgets for one with a given name
		///
		/// @param name The name to search for
		///
		/// @return The found widget (NULL if not found)
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *findNamed(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Handle being on the drawing phrase
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw();

		/////////////////////////////////////////////////////////////
		/// @brief Handle being on the fixed update phrase
		///
		/////////////////////////////////////////////////////////////
		virtual void onUpdate();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of parent
		///
		/////////////////////////////////////////////////////////////
		virtual void onParentChange();

		/////////////////////////////////////////////////////////////
		/// @brief Handle an event
		///
		/// @param ev The event descriptor
		///
		/// @param True if the event is not to be passed to the next widget
		///
		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(const fw::Event &ev);

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

#endif // FRONTIER_WIDGET_HPP_INCLUDED
