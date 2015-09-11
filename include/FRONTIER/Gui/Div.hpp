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
#ifndef FRONTIER_DIV_HPP_INCLUDED
#define FRONTIER_DIV_HPP_INCLUDED
#include <FRONTIER/Gui/Widget.hpp>
#define FRONTIER_DIV

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief A simple container class that holds one widget
	///
	/// The final size of the div is max(contentSize+margin*2 , minSize)
	///
	/// The content must be allocated with new (not new[])
	/// And will be deallocated automaitcally using delete
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Div : public Widget
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "Div"

	protected:
		fm::vec2 m_margin; ///< The margin
		Widget *m_content; ///< The contained widget

	public:
		typedef Div &reference;
		typedef const Div &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param name The identifier of the instance
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		/// @param margin The margin of the div
		/// @param content The contained widget
		///
		/////////////////////////////////////////////////////////////
		Div(const std::string &name = "NoName",
			const Anchor &anchor = Anchor(),
			const fm::vec2 &size = fm::vec2(),
			Widget *parent = fm::nullPtr,
			const fm::vec2 &margin = fm::vec2(),
			Widget *content = fm::nullPtr);


		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~Div();

		/////////////////////////////////////////////////////////////
		/// @brief Get the minimal size of the div
		///
		/// @return The minimal size
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the available space in the widget
		///
		/// @return The size
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getInternalSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the offset of the available space in the widget
		///
		/// @return The offset
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getInternalPos() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the minimal size of the div
		///
		/// @param size The new minimal size
		///
		/////////////////////////////////////////////////////////////
		virtual void setMinSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the margin of the div
		///
		/// @param margin The new margin
		///
		/////////////////////////////////////////////////////////////
		virtual void setMargin(const fm::vec2 &margin);

		/////////////////////////////////////////////////////////////
		/// @brief Set the parent of the div
		///
		/// @param parent The new parent
		///
		/////////////////////////////////////////////////////////////
		virtual void setParent(Widget *parent);

		/////////////////////////////////////////////////////////////
		/// @brief Set the minimal size of the div
		///
		/// @param size The new minimal size
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the anchor
		///
		/// @param anchor The new anchor
		///
		/////////////////////////////////////////////////////////////
		virtual void setAnchor(const Anchor &anchor);

		/////////////////////////////////////////////////////////////
		/// @brief Search the content for a widget with a given name
		///
		/// @param name The name to search for
		///
		/// @return The found widget (NULL if not found)
		///
		/////////////////////////////////////////////////////////////
		Widget *findNamed(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Draw the div
		///
		/// Passed through to the contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the fixed update
		///
		/// Passed through to the contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void onUpdate();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of parent
		///
		/// Passed through to the contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void onParentChange();

		/////////////////////////////////////////////////////////////
		/// @brief Pass through the event to the contained widget
		///
		/// @param ev The event
		///
		/// @return True if the event is handled
		///
		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(const fw::Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Get the contained widget
		///
		/// @return The contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *getContent();

		/////////////////////////////////////////////////////////////
		/// @brief Set the contained widget
		///
		/// @param content The new contained widget
		///
		/////////////////////////////////////////////////////////////
		virtual void setContent(Widget *content);

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

#endif // FRONTIER_DIV_HPP_INCLUDED
