////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Gui/PushButton.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>

namespace fgui
{
	void PushButton::applyState(InnerState state)
	{
		if (state == Normal)
			m_bckgSprite.setImageID("Button_Bckg_Norm");
			
		if (state == Hovered)
			m_bckgSprite.setImageID("Button_Bckg_Hover");
			
		if (state == Pressed)
			m_bckgSprite.setImageID("Button_Bckg_Press");
		
		setSize(getSize());
		setPosition(getPosition());
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont) : GuiButton(cont),
											   m_bckgSprite(cont.getSpriteManager(),"Button_Bckg_Norm")
	{
		setSize(getSize());
		setPosition(getPosition());
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont,const fm::String &text,fm::Delegate<void,GuiButton &> callback) : GuiButton(cont,text),
																											  m_bckgSprite(cont.getSpriteManager(),"Button_Bckg_Norm")
	{
		setSize(getSize());
		setPosition(getPosition());
		setCallback(callback);
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::onDraw(fg::ShaderManager &shader)
	{
		GuiButton::onDraw(shader);
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setSize(fm::vec2s size)
	{
		fm::vec2s textSize = getGuiText().getSize();
		
		textSize += m_bckgSprite.getFrameSize() * 2;
		
		if (size.w < textSize.w) size.w = textSize.w;
		if (size.h < textSize.h) size.h = textSize.h;
		
		GuiButton::setSize(size);
		
		m_bckgSprite.setSize(getSize());
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setPosition(fm::vec2i pos)
	{
		GuiButton::setPosition(pos);
		
		m_bckgSprite.setPosition(getPosition());
	}
}
