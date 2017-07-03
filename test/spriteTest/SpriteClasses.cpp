#include "SpriteClasses.hpp"

namespace test
{
	/////////////////////////////////////////////////////////////
	void Sprite::setId(fm::Size id)
	{
		m_id = id;
	}
	
	/////////////////////////////////////////////////////////////
	void Sprite::setManager(SpriteManager *manager)
	{
		m_manager = manager;
	}
		
	/////////////////////////////////////////////////////////////
	fm::Size Sprite::getId() const
	{
		return m_id;
	}
	
	/////////////////////////////////////////////////////////////
	Sprite::Sprite(SpriteManager &manager,ImageID imgId) : m_imgId(imgId)
	{
		manager.handleSpriteCreate(*this);
	}
		
	/////////////////////////////////////////////////////////////
	Sprite::Sprite(const Sprite &sprite) : m_manager(nullptr)
	{
		(*this) = sprite;
	}
	
	/////////////////////////////////////////////////////////////
	Sprite::Sprite(Sprite &&sprite) : m_manager(nullptr)
	{
		sprite.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::operator=(const Sprite &sprite)
	{
		destroy();
		Transformable::setPosition(sprite.getPosition());
		Transformable::setSize(sprite.getSize());
		m_imgId   = sprite.m_imgId;
		
		sprite.getManager()->handleSpriteCreate(*this);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::operator=(Sprite &&sprite)
	{
		return this->swap(sprite);
	}
		
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::swap(Sprite &sprite)
	{
		m_manager->handleSpriteSwap(*this,sprite);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Sprite::~Sprite()
	{
		destroy();
	}
	
	/////////////////////////////////////////////////////////////
	void Sprite::destroy()
	{
		if (m_manager)
			m_manager->handleSpriteDestroy(*this);
	}

	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setPosition(const fm::vec2 &pos)
	{
		Transformable::setPosition(pos);
		m_manager->handleSpriteRectChange(*this);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setSize(const fm::vec2 &size)
	{
		Transformable::setSize(size);
		m_manager->handleSpriteRectChange(*this);
		
		return *this;
	}
		
	/////////////////////////////////////////////////////////////
	Sprite &Sprite::setImgId(ImageID id)
	{
		m_imgId = id;
		m_manager->handleSpriteImageChange(*this);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	ImageID Sprite::getImgId() const
	{
		return m_imgId;
	}
	
	/////////////////////////////////////////////////////////////
	SpriteManager *Sprite::getManager() const
	{
		return m_manager;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/////////////////////////////////////////////////////////////
	SpriteManager::~SpriteManager()
	{
		for (auto ptr : m_sprites)
		{
			ptr->setManager(nullptr);
		}
	}
	
	/////////////////////////////////////////////////////////////
	Sprite SpriteManager::getSprite(ImageID id)
	{
		return Sprite(*this,id);
	}
		
	/////////////////////////////////////////////////////////////
	Sprite &SpriteManager::getSpriteById(fm::Size id)
	{
		return *m_sprites[id];
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size SpriteManager::getSpriteCount() const
	{
		return m_sprites.size();
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::addImage(const Image &img,ImageID id,fm::vec2 frameSize)
	{
		m_atlas.upload(img,id,frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 SpriteManager::getImageSize(ImageID id) const
	{
		return m_atlas.fetch(id).size;
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleSpriteCreate(Sprite &sprite)
	{
		fm::Size index = m_sprites.size();
		m_sprites.push_back(&sprite);
		
		sprite.setManager(this);
		sprite.setId(index);
		
		if (!sprite.getSize().area())
			sprite.Transformable::setSize(getImageSize(sprite.getImgId()));
		
		onCreateSprite(sprite);
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleSpriteDestroy(Sprite &sprite)
	{
		fm::Size spriteCount = m_sprites.size();
		fm::Size spriteId    = sprite.getId();
		
		if (spriteId != spriteCount-1)
		{
			std::swap(m_sprites[spriteId],m_sprites[spriteCount-1]);
			m_sprites[spriteId]->setId(spriteId);
		}
		
		m_sprites.pop_back();
		
		onDestroySprite(sprite);
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleSpriteRectChange(Sprite &sprite)
	{
		onRectChangeSprite(sprite);
	}
		
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleSpriteImageChange(Sprite &sprite)
	{
		onImageChangeSprite(sprite);
	}
		
	/////////////////////////////////////////////////////////////
	void SpriteManager::handleSpriteSwap(Sprite &sprite,Sprite &maybeSprite)
	{
		onSwapSprite(sprite,maybeSprite);
		
		if (!maybeSprite.getManager())
		{
			maybeSprite.setManager(this);
			maybeSprite.setId(sprite.getId());
			maybeSprite.m_imgId = sprite.getImgId();
			((Transformable&)maybeSprite) = sprite;
			
			sprite.setManager(nullptr);
		}
		
		if (maybeSprite.getManager() == this)
		{
			std::swap((Transformable&)sprite,(Transformable&)maybeSprite);
			std::swap(sprite.m_manager,maybeSprite.m_manager);
			std::swap(sprite.m_imgId,maybeSprite.m_imgId);
			std::swap(sprite.m_id,maybeSprite.m_id);
			std::swap(m_sprites[sprite.m_id],m_sprites[maybeSprite.m_id]);
			
			onSwapSpriteAfter(sprite,maybeSprite);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void SpriteManager::onSwapSpriteAfter(Sprite &sprite1,Sprite &sprite2)
	{
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	void calcSpriteData(fm::vec3 *pos,fm::vec4 *clr,fm::vec2 *texuv,const Sprite &sprite,const Glyph &shape)
	{
		const fm::vec2 pts[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,1)};
		
		C(4)
		{
			*pos++ = vec3(pts[i] * sprite.getSize() + sprite.getPosition(),0);
			*clr++ = vec4::White;
			*texuv++ = pts[i] * shape.size + shape.pos;
		}
	}
	
	/////////////////////////////////////////////////////////////
	void UniqueSpriteManager::onDestroySprite(Sprite &sprite)
	{
		fm::Size spriteCount = m_draws.size();
		fm::Size spriteId    = sprite.getId();
		
		if (spriteId != spriteCount-1)
			m_draws[spriteId].swap(m_draws[spriteCount-1]);
		
		m_draws.pop_back();
	}

	/////////////////////////////////////////////////////////////
	void UniqueSpriteManager::onCreateSprite(Sprite &sprite)
	{
		m_draws.push_back(DrawData());
		
		onRectChangeSprite(sprite);
	}
	
	/////////////////////////////////////////////////////////////
	void UniqueSpriteManager::onRectChangeSprite(Sprite &sprite)
	{
		fm::vec2 uvs[4];
		fm::vec3 pts[4];
		fm::vec4 clr[4];
		
		calcSpriteData(pts,clr,uvs,sprite,m_atlas.fetch(sprite.getImgId()));
		DrawData &dd = m_draws[sprite.getId()];
		
		dd[fg::Assoc::Position] = pts;
		
		if (!dd.getDrawCount())
		{
			dd[fg::Assoc::TextureUV] = uvs;
			dd[fg::Assoc::Color]     = clr;
			dd.addDraw(0,4,fg::TriangleFan);
		}
	}
		
	/////////////////////////////////////////////////////////////
	void UniqueSpriteManager::onImageChangeSprite(Sprite &sprite)
	{
		fm::vec2 uvs[4];
		fm::vec3 pts[4];
		fm::vec4 clr[4];
		
		calcSpriteData(pts,clr,uvs,sprite,m_atlas.fetch(sprite.getImgId()));
		DrawData &dd = m_draws[sprite.getId()];
		
		if (dd.getDrawCount())
			dd[fg::Assoc::TextureUV] = uvs;
	}
		
	/////////////////////////////////////////////////////////////
	void UniqueSpriteManager::onSwapSprite(Sprite &sprite,Sprite &maybeSprite)
	{
		if (maybeSprite.getManager() == (SpriteManager*)this)
			m_draws[sprite.getId()].swap(m_draws[maybeSprite.getId()]);
	}

	/////////////////////////////////////////////////////////////
	UniqueSpriteManager::UniqueSpriteManager()
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void UniqueSpriteManager::onDraw(ShaderManager &shader)
	{
		shader.useTexture(m_atlas.getTexture());
		shader.getTexUVStack().push().mul(m_atlas.getTexture().getPixToUnitMatrix());
		for (auto &dd : m_draws)
		{
			shader.draw(dd);
		}
		shader.getTexUVStack().pop();
	}
	
	
	
	
	
	
	
	
	void calcSpriteData6(fm::vec3 *pts,fm::vec4 *clr,fm::vec2 *uvs,const Sprite &sprite,const Glyph &shape)
	{
		const fm::vec2 tpt[] = {fm::vec2(0,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(0,0),fm::vec2(1,1),fm::vec2(0,1)};
		
		Cx(6)
		{
			*pts++ = tpt[x] * sprite.getSize() + sprite.getPosition();
			*clr++ = fm::vec4::White;
			*uvs++ = tpt[x] * shape.size + shape.pos;
		}
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::updateDrawCount()
	{
		fm::Size drawCount = getSpriteCount();
		
			cout << "updateDrawCount w/ drawCount = " << drawCount << endl;
		if (drawCount > m_allocSize)
		{
			setAllocSize(std::max(drawCount,m_allocSize*2));
		}
		
		m_drawData.getDraw(0).set(0,drawCount*6,fg::Triangles);
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::setAllocSize(fm::Size allocSize)
	{
		m_allocSize = allocSize;
		
		fm::vec3 *pts = new fm::vec3[allocSize*6];
		fm::vec2 *uvs = new fm::vec2[allocSize*6];
		fm::vec4 *clr = new fm::vec4[allocSize*6];
		
		C(std::min(allocSize,getSpriteCount()))
		{
			Sprite &sprite = getSpriteById(i);
			Glyph shape = m_atlas.fetch(sprite.getImgId());
			
			calcSpriteData6(pts + i*6,clr + i*6,uvs + i*6,sprite,shape);
		}
		
		m_drawData.texPositions.set(uvs,allocSize*6);
		m_drawData.positions   .set(pts,allocSize*6);
		m_drawData.colors      .set(clr,allocSize*6);
		
		delete[] pts;
		delete[] uvs;
		delete[] clr;
		
		cout << "setAllocSize w/ allocSize = " << allocSize << endl;
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onDestroySprite(Sprite &sprite)
	{
		onCreateSprite(getSpriteById(sprite.getId()));
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onCreateSprite(Sprite &sprite)
	{
		updateDrawCount();
		
		onRectChangeSprite(sprite);
		onImageChangeSprite(sprite);
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onRectChangeSprite(Sprite &sprite)
	{
		fm::vec3 pts[6];
		fm::vec2 uvs[6];
		fm::vec4 clr[6];
		
		calcSpriteData6(pts,clr,uvs,sprite,m_atlas.fetch(sprite.getImgId()));
		
		m_drawData.positions.getData().buf->updateData(pts,sizeof(pts),sizeof(pts) * sprite.getId());
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onImageChangeSprite(Sprite &sprite)
	{
		fm::vec3 pts[6];
		fm::vec2 uvs[6];
		fm::vec4 clr[6];
		
		calcSpriteData6(pts,clr,uvs,sprite,m_atlas.fetch(sprite.getImgId()));
		
		m_drawData.texPositions.getData().buf->updateData(uvs,sizeof(uvs),sizeof(uvs) * sprite.getId());
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onSwapSprite(Sprite &sprite,Sprite &maybeSprite)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onSwapSpriteAfter(Sprite &sprite1,Sprite &sprite2)
	{
		onRectChangeSprite(sprite1);
		onImageChangeSprite(sprite1);
		
		onRectChangeSprite(sprite2);
		onImageChangeSprite(sprite2);
	}
	
	/////////////////////////////////////////////////////////////
	PerOneSpriteManager::PerOneSpriteManager() : m_allocSize(0)
	{
		m_drawData.addDraw(0,0,fg::Triangles);
	}
	
	/////////////////////////////////////////////////////////////
	void PerOneSpriteManager::onDraw(ShaderManager &shader)
	{
		shader.getTexUVStack().push().mul(m_atlas.getTexture().getPixToUnitMatrix());
		shader.useTexture(m_atlas.getTexture());
		
		shader.draw(m_drawData);
		
		shader.getTexUVStack().pop();
	}
}
