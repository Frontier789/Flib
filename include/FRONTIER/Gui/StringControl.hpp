#ifndef FRONTIER_STRINGCONTROL_HPP_INCLUDED
#define FRONTIER_STRINGCONTROL_HPP_INCLUDED

#include <FRONTIER/Gui/ValueControlBase.hpp>
#include <FRONTIER/Gui/BasicControl.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Control used to deal with integers and floating points
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class StringControl : public ValueControlBase
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "StringControl"

		void (*m_callback)(const fm::String &,StringControl *); ///< The callback used when the value changes
		bool m_ascii;

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
		/// @param callback The callback used when the value changes
		/// @param ascii Only allow ascii characters
		/// @param monoSpace Set if every character should occupy the same size
		/// @param textColor The color of the text
		/// @param selectColor The color of the selection
		/// @param caretColor The color of the carets
		/// @param bckColor The color of the background
		/// 
		/////////////////////////////////////////////////////////////
		StringControl(const std::string &name = "NoName",
					  const Anchor &anchor = Anchor(),
					  const fm::vec2 &size = fm::vec2(),
					  Widget *parent = fm::nullPtr,
					  const fm::String &defText = "0",
					  fg::Font *font = fm::nullPtr,
					  fm::Size characterSize = 20,
					  void (*callback)(const fm::String &,StringControl *) = fm::nullPtr,
					  bool ascii = false,
					  bool monoSpace = true,
					  fm::vec4 textColor = fm::vec4::Black,
					  fm::vec4 selectColor = fm::vec4(.75,.75,.75),
					  fm::vec4 caretColor = fm::vec4::Black,
					  fm::vec4 bckColor = fm::vec4(0,0,0,0));

		/////////////////////////////////////////////////////////////
		/// @brief Decide if a given character can be inserted into the control
		///
		/// @param character The character
		///
		/// @return True iff the character is acceptable
		///
		/////////////////////////////////////////////////////////////
		bool acceptCharacter(fm::Uint32 character);

		/////////////////////////////////////////////////////////////
		/// @brief Decide if a string would be valid content of the control
		///
		/// @param str The string
		///
		/// @return True iff the string is acceptable
		///
		/////////////////////////////////////////////////////////////
		bool isStringValid(const fm::String &str);

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of the content
		///
		/// Use getText() to access the new content
		///
		/////////////////////////////////////////////////////////////
		void onContentUpdate();

		/////////////////////////////////////////////////////////////
		/// @brief Change the callback used when the value is changed
		///
		/// @param callback The callback used when the value changes
		///
		/////////////////////////////////////////////////////////////
		void setCallback(void (*callback)(const fm::String &,StringControl *));

		/////////////////////////////////////////////////////////////
		/// @brief Get the value of the control
		///
		/// If integer is true the return value is integer
		///
		/// @return The value
		///
		/////////////////////////////////////////////////////////////
		const fm::String &getVal();

		/////////////////////////////////////////////////////////////
		/// @brief Change the value of the control
		///
		/// @param value The new value
		///
		/////////////////////////////////////////////////////////////
		void setVal(const fm::String &value);

		/////////////////////////////////////////////////////////////
		/// @brief Check whether the control uses ascii
		///
		/// @return True iff the control only accepts ascii characters
		///
		/////////////////////////////////////////////////////////////
		bool isAscii();

		/////////////////////////////////////////////////////////////
		/// @brief Set whether the control uses ascii
		///
		/// @param integer Iff true the control only accepts ascii characters
		///
		/////////////////////////////////////////////////////////////
		void setAscii(bool ascii);
	};
}

#endif // FRONTIER_STRINGCONTROL_HPP_INCLUDED

