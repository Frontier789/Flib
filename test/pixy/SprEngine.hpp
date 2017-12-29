#ifndef SPR_ENGINE_HPP_INCLUDED
#define SPR_ENGINE_HPP_INCLUDED

#include <Frontier.hpp>
#include "PixyEngine.hpp"
#include <vector>

class SprEngine : public PixyEngine
{
	SpriteManager m_sprmgr;
	std::vector<Sprite> m_sprites;

	/////////////////////////////////////////////////////////////
	void onScreenSize() override;

public:
	/////////////////////////////////////////////////////////////
	SprEngine(GuiContext &owner,vec2s size = vec2s());

	/////////////////////////////////////////////////////////////
	void drawMap(fg::ShaderManager &shader) override;

	/////////////////////////////////////////////////////////////
	void setField(vec2s p,FieldData data) override;
	
	/////////////////////////////////////////////////////////////
	const Sprite &getSprite(vec2s p) const {return m_sprites[p.x + p.y * getLayoutSize().w];}
	
	/////////////////////////////////////////////////////////////
	Sprite &getSprite(vec2s p) {return m_sprites[p.x + p.y * getLayoutSize().w];}
};

#endif // SPR_ENGINE_HPP_INCLUDED
