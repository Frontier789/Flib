#include "PixyGame.hpp"
#include "SprEngine.hpp"

/////////////////////////////////////////////////////////////
PixyGame::PixyGame(fm::String title,vec2s mapSize) : 
					m_win(vec2(640,480),title),
					m_lastHover(-1,-1),
					m_tick(0),
					dst(0,1)
{
	m_engine = new SprEngine(m_win,mapSize);
	m_win.getMainLayout().addChildElement(m_engine);
	m_win.enableKeyRepeat();

	m_engine->onUpdate(Time(0.5));

	m_engine->setEventListeners([&](fw::Event &ev){
		
		onEvent(ev);
	},[&](){

		onTick(++m_tick);
	});
}

/////////////////////////////////////////////////////////////
void PixyGame::onEvent(fw::Event &ev)
{
	if (ev.type == Event::MouseMoved || 
		ev.type == Event::MouseEntered || 
		ev.type == Event::MouseLeft)
	{
		vec2i curHover = vec2(ev.motion) / m_win.getSize() * m_engine->getLayoutSize();
		if (curHover != m_lastHover)
		{
			if (validPoint(m_lastHover))
			{
				onHover(m_lastHover,m_engine->getData(m_lastHover),false);
			}

			if (validPoint(curHover))
			{
				onHover(curHover,m_engine->getData(curHover),true);
			}

			m_lastHover = curHover;
		}
	}

	if (ev.type == Event::KeyPressed)
	{
		onKey(ev.key);
	}
}

/////////////////////////////////////////////////////////////
bool PixyGame::validPoint(vec2i p) const
{
	return fm::rect2i(vec2(),m_engine->getLayoutSize()-vec2i(1)).contains(p);
}

/////////////////////////////////////////////////////////////
void PixyGame::run()
{
	m_win.runGuiLoop();
}

/////////////////////////////////////////////////////////////
void PixyGame::onHover(vec2s,FieldData &,bool)
{

}

/////////////////////////////////////////////////////////////
void PixyGame::onKey(fw::KeyboardEvent) 
{
	
}

/////////////////////////////////////////////////////////////
void PixyGame::onTick(fm::Size)
{

}

/////////////////////////////////////////////////////////////
double PixyGame::random() const
{
	return dst(gen);
}

/////////////////////////////////////////////////////////////
vec4 PixyGame::getMapColor(vec2i p) const
{
	if (validPoint(p)) return m_engine->getData(p).color;

	return vec4::Black;
}

/////////////////////////////////////////////////////////////
void PixyGame::setMapColor(vec2i p,vec4 c)
{
	if (validPoint(p))
	{
		auto data = m_engine->getData(p);
		data.color = c;

		m_engine->setField(p,data);
	}
}

/////////////////////////////////////////////////////////////
vec2s PixyGame::getMapSize() const
{
	return m_engine->getLayoutSize();
}
