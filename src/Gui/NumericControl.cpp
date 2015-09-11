#include <FRONTIER/Gui/NumericControl.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string NumericControl::m_className = "NumericControl";

	/////////////////////////////////////////////////////////////
	NumericControl::NumericControl(const std::string &name,
								  const Anchor &anchor,
								  const fm::vec2 &size,
								  Widget *parent,
								  const fm::String &defText,
								  fg::Font *font,
								  fm::Size characterSize,
								  void (*callback)(double,NumericControl *),
								  bool integer,
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
																		  m_val(0),
																		  m_callback(callback),
																		  m_integer(integer)
	{
		m_callback = fm::nullPtr;
		onContentUpdate();
		m_callback = callback;
	}

	/////////////////////////////////////////////////////////////
	bool NumericControl::acceptCharacter(fm::Uint32 character)
	{
		if (m_integer)
			return fm::String("0123456789-+").find(character) != fm::String::npos;

		return fm::String("0123456789.-+").find(character) != fm::String::npos;
	}

	/////////////////////////////////////////////////////////////
	bool NumericControl::isStringValid(const fm::String &str)
	{
		if (str.count('.') > 1 || str.count('+')+str.count('-') > 1)
			return false;

		if (str.count('+')+str.count('-') == 1 && str[0]!='+' && str[0]!='-')
			return false;

		return true;
	}

	/////////////////////////////////////////////////////////////
	void NumericControl::onContentUpdate()
	{
		const fm::String &str = getText();

		double sign = (str[0]=='-' ? -1.0 : 1.0);
		double whole = 0;
		double fraction = 0;

		C(str.size())
		{
			if (str[i]>='0' && str[i]<='9')
				whole *= 10,
				whole += str[i]-'0';

			if (str[i]=='.')
			{
				for (fm::Size x = str.size()-1;x>0;x--)
				{
					if (str[x]>='0' && str[x]<='9')
						fraction += str[x]-'0',
						fraction /= 10;

					if (str[x]=='.') break;
				}

				break;
			}
		}

		m_val = sign*(whole+fraction);

		if (m_callback)
			m_callback(m_val,this);
	}

	/////////////////////////////////////////////////////////////
	void NumericControl::setCallback(void (*callback)(double,NumericControl *))
	{
		m_callback = callback;
	}

	/////////////////////////////////////////////////////////////
	double NumericControl::getVal()
	{
		return m_val;
	}

	/////////////////////////////////////////////////////////////
	void NumericControl::setVal(double value)
	{
		fm::String s;

		int sign = value<0 ? -1 : 1;
		fm::Int64 whole = (fm::Int64)value;
		double fraction = value-whole;

		if (whole<0) whole *= -1.0;
		if (fraction<0) fraction *= -1.0;

		while (whole) s.push_front( char(whole%10+'0') ), whole/=10;

		if (sign == -1) s.push_front('-');

		if (fraction*10000000000.0 >= 1.0)
		{
			s += '.';
			for (int i=0;i<10 && fraction;i++)
				fraction *= 10,
				s.push_back( char((int)fraction % 10 + '0') ),
				fraction -= int(fraction);
		}

		setContent(s);
		onContentUpdate();
	}

	/////////////////////////////////////////////////////////////
	bool NumericControl::isInteger()
	{
		return m_integer;
	}

	/////////////////////////////////////////////////////////////
	void NumericControl::setInteger(bool integer)
	{
		if (!m_integer && integer)
			setVal(floor(getVal()));
		m_integer = integer;
	}
}

