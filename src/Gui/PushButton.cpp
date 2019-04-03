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
#include <FRONTIER/Gui/PushButton.hpp>
#include <FRONTIER/Gui/GuiContext.hpp>

namespace fgui
{
	void PushButton::applyState()
	{
		auto img = m_bckImgs[int(m_state)];
		
		m_bckgSprite.setImageID(img);
		
		setSize(getSize());
		setPosition(getPosition());
		m_bckgSprite.setColor(m_bckgClrs[int(m_state)]);
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont) : PushButton(cont, "")
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	PushButton::PushButton(GuiContext &cont,const fm::String &text,fm::Delegate<void,GuiButton &> callback) : 
		GuiButton(cont,text,callback),
		m_bckgSprite(cont.getSpriteManager()),
		m_bckgClrs{
			fm::vec4::White,
			fm::vec4::White,
			fm::vec4::White
		},
		m_bckImgs{
			"Button_Bckg_Norm" ,
			"Button_Bckg_Hover",
			"Button_Bckg_Press"
		}
	{
		applyState();
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setBgImage(const std::string &id)
	{
		setBgImage(id,id,id);
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setBgImage(const std::string &normal,const std::string &hover,const std::string &press)
	{
		setBgImage(normal,hover,press,fm::vec2());
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setBgImage(const std::string &id,fm::vec2 frameSize)
	{
		setBgImage(id,id,id,frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setBgImage(const std::string &normal,const std::string &hover,const std::string &press,fm::vec2 frameSize)
	{
		std::string prev = m_bckImgs[int(m_state)];
		m_bckImgs = {normal, hover, press};
		
		auto &cont = getOwnerContext();
		for (auto img : m_bckImgs) {
			if (!cont.hasImage(img)) {
				cont.addImage(img,img,frameSize);
			}
		}
		
		if (prev != m_bckImgs[int(m_state)]) {
			applyState();
		}
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
	
	/////////////////////////////////////////////////////////////
	void PushButton::setBgColor(fm::vec4 normal,fm::vec4 hover,fm::vec4 press)
	{
		m_bckgClrs = {normal, hover, press};
		
		m_bckgSprite.setColor(m_bckgClrs[int(m_state)]);
	}
	
	/////////////////////////////////////////////////////////////
	void PushButton::setBgColor(fm::vec4 color)
	{
		m_bckgClrs = {color, color, color};
		
		m_bckgSprite.setColor(m_bckgClrs[int(m_state)]);
	}
}
