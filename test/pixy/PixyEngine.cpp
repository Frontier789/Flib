#include "PixyEngine.hpp"

/////////////////////////////////////////////////////////////
void PixyEngine::onScreenSize()
{

}

/////////////////////////////////////////////////////////////
PixyEngine::PixyEngine(GuiContext &owner,vec2s size) : GuiElement(owner)
{
	if (size.area() == 0)
		size = owner.getSize() / 40;

	m_data.resize(size.area());
	m_layoutSize = size;
}

/////////////////////////////////////////////////////////////
void PixyEngine::setEventListeners(fm::Delegate<void,fw::Event &> f_ev,fm::Delegate<void> f_tick)
{
	m_evListener = f_ev;
	m_tickListener = f_tick;
}

/////////////////////////////////////////////////////////////
void PixyEngine::onDraw(fg::ShaderManager &shader)
{
	drawMap(shader);

	GuiElement::onDraw(shader);
}

/////////////////////////////////////////////////////////////
bool PixyEngine::onEvent(fw::Event &ev)
{
	m_evListener(ev);

	if (ev.type == Event::Resized)
	{
		setSize(ev.size);
		onScreenSize();
	}

	return GuiElement::onEvent(ev);
}
	
/////////////////////////////////////////////////////////////
void PixyEngine::onUpdate(const fm::Time &dt)
{
	m_tickListener();
	
	GuiElement::onUpdate(dt);
}
