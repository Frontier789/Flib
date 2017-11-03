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
#ifndef FRONTIER_SPRITEMANAGER_INL_INCLUDED
#define FRONTIER_SPRITEMANAGER_INL_INCLUDED

namespace fg
{
	namespace priv
	{
		template<class ImageID>
		fg::Glyph glyphGetterFunc(SpriteManagerBaseNonTemplate *mgr,fm::Size spriteIndex)
		{
			SpriteManagerBase<ImageID> *manager = (SpriteManagerBase<ImageID>*)mgr;
			
			return manager->getAtlas().fetch(manager->fetchImgID(manager->getSpriteById(spriteIndex)));
		}
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteManagerBase<ImageID>::SpriteManagerBase(bool useFrames) : SpriteManagerBaseNonTemplate(useFrames,&priv::glyphGetterFunc<ImageID>)
	{
		m_atlas.getTexture().setSmooth(true);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteManagerBase<ImageID>::~SpriteManagerBase()
	{
		for (auto &sprDat : m_spriteDatas)
			sprDat.ptr->setManager(nullptr);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline void SpriteManagerBase<ImageID>::handleCreate(SpriteBase<ImageID> &sprite,ImageID imgID,const fm::vec3 &pos,const fm::vec2 &size,const fm::vec2 &dir)
	{
		sprite.setId(getSpriteCount());
		sprite.setManager(this);
		
		m_spriteDatas.push_back(SpriteData{&sprite,imgID});
		fg::Glyph shape = m_atlas.fetch(imgID);
		
		priv::SpriteManagerBaseNonTemplate::handleCreate(sprite.getId(),shape,pos,size,dir);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline void SpriteManagerBase<ImageID>::handleDestroy(SpriteBase<ImageID> &sprite)
	{
		SpriteBase<ImageID> &lastSprite = *m_spriteDatas.back().ptr;
		
		fm::Size index = sprite.getId();
		fm::Size oldIndex = lastSprite.getId();
		
		if (oldIndex != index)
		{
			lastSprite.setId(index);
			m_spriteDatas[index] = m_spriteDatas[oldIndex];
			
			priv::SpriteManagerBaseNonTemplate::handleMoveFrom(index,oldIndex);
		}
		
		m_spriteDatas.pop_back();
		
		priv::SpriteManagerBaseNonTemplate::handlePropPop();
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	void SpriteManagerBase<ImageID>::handleEmplace(SpriteBase<ImageID> &sprite)
	{
		m_spriteDatas[sprite.getId()].ptr = &sprite;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline void SpriteManagerBase<ImageID>::handleImageIDChange(SpriteBase<ImageID> &sprite,ImageID imgID)
	{
		m_spriteDatas[sprite.getId()].imgID = imgID;
		fg::Glyph shape = m_atlas.fetch(imgID);
		
		priv::SpriteManagerBaseNonTemplate::handleImageIDChange(sprite.getId(),shape);
	}

	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline ImageID SpriteManagerBase<ImageID>::fetchImgID(const SpriteBase<ImageID> &sprite) const
	{
		return m_spriteDatas[sprite.getId()].imgID;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline void SpriteManagerBase<ImageID>::addImage(const fg::Image &img,ImageID id,const fm::vec2 &frameSize)
	{
		m_atlas.upload(img,id,frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline void SpriteManagerBase<ImageID>::addImage(const fg::Texture &tex,ImageID id,const fm::vec2 &frameSize)
	{
		m_atlas.upload(tex,id,frameSize);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline fm::vec2 SpriteManagerBase<ImageID>::getImageSize(ImageID id)
	{
		if (id == ImageID() && !m_atlas.isUploaded(id))
		{
			fg::Image tmp(fm::vec2(1,1),fg::Color::White);
			addImage(tmp,ImageID());
		}
		
		return m_atlas.fetch(id).size;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline const fg::TextureAtlas<ImageID> &SpriteManagerBase<ImageID>::getAtlas() const
	{
		return m_atlas;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline fg::TextureAtlas<ImageID> &SpriteManagerBase<ImageID>::getAtlas()
	{
		return m_atlas;
	}
		
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID> SpriteManagerBase<ImageID>::getSprite(ImageID id)
	{
		return SpriteBase<ImageID>(*this,id);
	}
		
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID> &SpriteManagerBase<ImageID>::getSpriteById(fm::Size id)
	{
		return *m_spriteDatas[id].ptr;
	}
	 
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline fm::Size SpriteManagerBase<ImageID>::getSpriteCount() const
	{
		return m_spriteDatas.size();
	}
	 
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline void SpriteManagerBase<ImageID>::onDraw(fg::ShaderManager &shader)
	{
		return priv::SpriteManagerBaseNonTemplate::onDrawTex(shader,m_atlas.getTexture());
	}
}

#endif // FRONTIER_SPRITEMANAGER_INL_INCLUDED