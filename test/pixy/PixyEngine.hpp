#ifndef PIXY_ENGINE_HPP_INCLUDED
#define PIXY_ENGINE_HPP_INCLUDED

#include <Frontier.hpp>
#include <vector>
#include <iostream>
using namespace std;

class FieldData
{
public:
	vec4 color;
};

class PixyEngine : public GuiElement
{
	vec2s m_layoutSize;
	std::vector<FieldData> m_data;
	fm::Delegate<void,fw::Event &> m_evListener;
	fm::Delegate<void> m_tickListener;
	
	/////////////////////////////////////////////////////////////
	virtual void onScreenSize();

public:
	/////////////////////////////////////////////////////////////
	PixyEngine(GuiContext &owner,vec2s size = vec2s());

	/////////////////////////////////////////////////////////////
	void setEventListeners(fm::Delegate<void,fw::Event &> f_ev,fm::Delegate<void> f_tick);

	/////////////////////////////////////////////////////////////
	virtual void drawMap(fg::ShaderManager &shader) = 0;

	/////////////////////////////////////////////////////////////
	virtual void setField(vec2s p,FieldData data) = 0;

	/////////////////////////////////////////////////////////////
	void onDraw(fg::ShaderManager &shader) override;
	
	/////////////////////////////////////////////////////////////
	bool onEvent(fw::Event &ev) override;
	
	/////////////////////////////////////////////////////////////
	virtual void onUpdate(const fm::Time &dt) override;
	
	/////////////////////////////////////////////////////////////
	vec2s getLayoutSize() const {return m_layoutSize;}
	
	/////////////////////////////////////////////////////////////
	const FieldData &getData(vec2s p) const {return m_data[p.x + p.y * m_layoutSize.w];}
	
	/////////////////////////////////////////////////////////////
	FieldData &getData(vec2s p) {return m_data[p.x + p.y * m_layoutSize.w];}
};

#endif // PIXY_ENGINE_HPP_INCLUDED
