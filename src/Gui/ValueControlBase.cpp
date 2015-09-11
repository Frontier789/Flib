#include <FRONTIER/Gui/ValueControlBase.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string ValueControlBase::m_className = "ValueControlBase";

	/////////////////////////////////////////////////////////////
	void ValueControlBase::setViewPos(const fm::vec2 &viewPos)
	{
		fm::vec2 vp(viewPos.x,0);
		BasicControl::setViewPos(viewPos);
	}

	/////////////////////////////////////////////////////////////
	ValueControlBase::ValueControlBase(const std::string &name,
									   const Anchor &anchor,
									   const fm::vec2 &size,
									   Widget *parent,
									   const fm::String &defText,
									   fg::Font *font,
									   fm::Size characterSize,
									   bool monoSpace,
									   fm::vec4 textColor,
									   fm::vec4 selectColor,
									   fm::vec4 caretColor,
									   fm::vec4 bckColor) : BasicControl(name,
																		   anchor,
																		   size,
																		   parent,
																		   defText,
																		   font,
																		   characterSize,
																		   true,
																		   monoSpace,
																		   textColor,
																		   selectColor,
																		   bckColor,
																		   caretColor),
																		   m_lastValidText(defText)
	{
		setSize(fm::vec2(getSize().w,m_metrics.lineGap+2));
	}

	/////////////////////////////////////////////////////////////
	bool ValueControlBase::handleEvent(const fw::Event &ev)
	{
		if (getActive() == this)
		{
			if (ev.type == fw::Event::MouseWheelMoved)
				return true;
			if (ev.type == fw::Event::KeyPressed)
			{
				if (ev.key.code == fw::Keyboard::Enter)
				{
					if (!isStringValid(getText()))
						setContent(m_lastValidText);
					else
						onContentUpdate(),
						m_lastValidText = getText();

					return true;
				}
			}
			if (ev.type == fw::Event::ButtonPressed)
			{
				if (!fm::rect2f(getPosInRoot(),getSize()).contains(fm::vec2::loadxy(ev.mouse)))
				{
					if (!isStringValid(getText()))
						setContent(m_lastValidText);
					else
						onContentUpdate(),
						m_lastValidText = getText();

					setActive(fm::nullPtr);
				}
			}

			if (ev.type == fw::Event::TextEntered)
				if (!acceptCharacter(ev.text.utf8character))
					return true;
		}

		return BasicControl::handleEvent(ev);
	}

	/////////////////////////////////////////////////////////////
	const fm::String &ValueControlBase::getText()
	{
		return m_lines[0];
	}

	/////////////////////////////////////////////////////////////
	const std::string &ValueControlBase::getRealType() const
	{
		return m_className;
	}
}

