#include "SprEngine.hpp"

/////////////////////////////////////////////////////////////
void SprEngine::onScreenSize()
{
	vec2s size = getLayoutSize();
	vec2 scr = getSize();

	fm::Size i = 0;
	Cy(size.h)
	{
		Cx(size.w)
		{
			m_sprites[i].setPosition(vec2(x,y) * scr / size);
			m_sprites[i].setSize(vec2s(scr / size));
			++i;
		}
	}
}

/////////////////////////////////////////////////////////////
SprEngine::SprEngine(GuiContext &owner,vec2s size) : PixyEngine(owner,size)
{
	size = getLayoutSize();

	fg::Image emptyImg;
	emptyImg.create(vec2(10,10),Color::White);
	m_sprmgr.addImage(emptyImg,"empty",vec2());

	m_sprmgr.getAtlas().getTexture().setSmooth(false);

	m_sprites = m_sprmgr.getSprites("empty",size.area());

	fm::Size i = 0;

	Cx(size.w) Cy(size.h) m_sprites[i++].setColor(getData(vec2(x,y)).color);

	onScreenSize();
}

/////////////////////////////////////////////////////////////
void SprEngine::drawMap(fg::ShaderManager &shader)
{
	m_sprmgr.onDraw(shader);
}

/////////////////////////////////////////////////////////////
void SprEngine::setField(vec2s p,FieldData data)
{
	getData(p) = data;

	getSprite(p).setColor(data.color);
}
