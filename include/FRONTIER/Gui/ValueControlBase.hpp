#ifndef FRONTIER_VALUECONTROLBASE_HPP_INCLUDED
#define FRONTIER_VALUECONTROLBASE_HPP_INCLUDED

#include <FRONTIER/Gui/BasicControl.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief The base class of value controls
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class ValueControlBase : public BasicControl
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "ValueControlBase"

		fm::String m_lastValidText; ///< Stores the last valid text

		/////////////////////////////////////////////////////////////
		/// @brief Override of BasicControl::setViewPos
		///
		/// Does not enable changing the view on the y axis
		///
		/// @param viewPos The new view position
		///
		/////////////////////////////////////////////////////////////
		virtual void setViewPos(const fm::vec2 &viewPos);
	public:

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param name The identifier of the instance
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		/// @param defText The text to contain by default
		/// @param font The font used to display
		/// @param characterSize The default character size
		/// @param monoSpace Set if every character should occupy the same size
		/// @param textColor The color of the text
		/// @param selectColor The color of the selection
		/// @param caretColor The color of the carets
		/// @param bckColor The color of the background
		///
		/////////////////////////////////////////////////////////////
		ValueControlBase(const std::string &name = "NoName",
						 const Anchor &anchor = Anchor(),
						 const fm::vec2 &size = fm::vec2(),
						 Widget *parent = fm::nullPtr,
						 const fm::String &defText = "0",
						 fg::Font *font = fm::nullPtr,
						 fm::Size characterSize = 20,
						 bool monoSpace = true,
						 fm::vec4 textColor = fm::vec4::Black,
						 fm::vec4 selectColor = fm::vec4(.75,.75,.75),
					     fm::vec4 caretColor = fm::vec4::Black,
						 fm::vec4 bckColor = fm::vec4(0,0,0,0));

		/////////////////////////////////////////////////////////////
		/// @brief Override of BasicControl::handleEvent
		///
		/// filters unaccepted characters and upon pressing enter
		/// validates value
		///
		/// @param ev The event
		///
		/// @return True iff the event was handled
		///
		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(const fw::Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Get the current content of the control
		///
		/// @return The text of the control
		///
		/////////////////////////////////////////////////////////////
		virtual const fm::String &getText();

		/////////////////////////////////////////////////////////////
		/// @brief Decide if a given character can be inserted into the control
		///
		/// @param character The character
		///
		/// @return True iff the character is acceptable
		///
		/////////////////////////////////////////////////////////////
		virtual bool acceptCharacter(fm::Uint32 character) = 0;

		/////////////////////////////////////////////////////////////
		/// @brief Decide if a string would be valid content of the control
		///
		/// @param str The string
		///
		/// @return True iff the string is acceptable
		///
		/////////////////////////////////////////////////////////////
		virtual bool isStringValid(const fm::String &str) = 0;

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of the content
		///
		/// Use getText() to access the new content
		///
		/////////////////////////////////////////////////////////////
		virtual void onContentUpdate() = 0;

		/////////////////////////////////////////////////////////////
		/// @brief Get the real type of the class
		///
		/////////////////////////////////////////////////////////////
		virtual const std::string &getRealType() const;
	};
}

#endif // FRONTIER_VALUE_CONTROL_BASE_HPP_INCLUDED

