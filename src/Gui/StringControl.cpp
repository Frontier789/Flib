#include <FRONTIER/Gui/StringControl.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string StringControl::m_className = "StringControl";

	/////////////////////////////////////////////////////////////
	StringControl::StringControl(const std::string &name,
								 const Anchor &anchor,
								 const fm::vec2 &size,
								 Widget *parent,
								 const fm::String &defText,
								 fg::Font *font,
								 fm::Size characterSize,
								 void (*callback)(const fm::String &,StringControl *),
								 bool ascii,
								 bool monoSpace,
								 fm::vec4 textColor,
								 fm::vec4 selectColor,
								 fm::vec4 caretColor,
								 fm::vec4 bckColor) : ValueControlBase(name,
																		anchor,
																		size,
																		parent,
																		defText,
																		font,
																		characterSize,
																		monoSpace,
																		textColor,
																		selectColor,
																		bckColor,
																		caretColor),
																		m_callback(callback),
																		m_ascii(ascii)
	{

	}

	/////////////////////////////////////////////////////////////
	bool StringControl::acceptCharacter(fm::Uint32 character)
	{
		if (m_ascii)
			return character < 256;

		return true;
	}

	/////////////////////////////////////////////////////////////
	bool StringControl::isStringValid(const fm::String &str)
	{
		if (m_ascii)
			C(str.size())
				if (str[i] >= 256)
					return false;

		return true;
	}

	/////////////////////////////////////////////////////////////
	void StringControl::onContentUpdate()
	{
		if (m_callback)
			m_callback(getVal(),this);
	}

	/////////////////////////////////////////////////////////////
	void StringControl::setCallback(void (*callback)(const fm::String &,StringControl *))
	{
		m_callback = callback;
	}

	/////////////////////////////////////////////////////////////
	const fm::String &StringControl::getVal()
	{
		return m_lines[0];
	}

	/////////////////////////////////////////////////////////////
	void StringControl::setVal(const fm::String &value)
	{
		setContent(value);
		onContentUpdate();
	}

	/////////////////////////////////////////////////////////////
	bool StringControl::isAscii()
	{
		return m_ascii;
	}

	/////////////////////////////////////////////////////////////
	void StringControl::setAscii(bool ascii)
	{
		m_ascii = ascii;
	}
}

