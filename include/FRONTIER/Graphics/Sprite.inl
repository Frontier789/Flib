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
#ifndef FRONTIER_SPRITE_INL_INCLUDED
#define FRONTIER_SPRITE_INL_INCLUDED

#include <FRONTIER/Graphics/SpriteManager.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID>::~SpriteBase()
	{
		if (getManager())
			getManager()->handleDestroy(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID>::SpriteBase(SpriteManagerBase<ImageID> &manager,ImageID imgID) : m_manager(&manager)
	{
		getManager()->handleCreate(*this,
								   imgID,
								   fm::vec3(),
								   getManager()->getImageSize(imgID),
								   fm::vec2(0,1));
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID>::SpriteBase(const SpriteBase<ImageID> &sprite) : m_manager(sprite.getManager())
	{
		getManager()->handleCreate(*this,
									sprite.getImageID(),
									sprite.getPosition(),
									sprite.getSize(),
									sprite.getDirection());
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID>::SpriteBase(SpriteBase<ImageID> &&sprite) : m_manager(nullptr)
	{
		sprite.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID> &SpriteBase<ImageID>::setImageID(ImageID imgID)
	{
		getManager()->handleImageIDChange(*this,imgID);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline ImageID SpriteBase<ImageID>::getImageID() const
	{
		return getManager()->fetchImgID(*this);
	}
	
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID> &SpriteBase<ImageID>::operator=(const SpriteBase<ImageID> &sprite)
	{
		getManager()->handleDestroy(*this);
		
		setManager(sprite.getManager());
		
		getManager()->handleCreate(*this,
									sprite.getImageID(),
									sprite.getPosition(),
									sprite.getSize(),
									sprite.getDirection());
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID> &SpriteBase<ImageID>::operator=(SpriteBase<ImageID> &&sprite)
	{
		getManager()->handleDestroy(*this);
		setManager(nullptr);
		
		sprite.swap(*this);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	inline SpriteBase<ImageID> &SpriteBase<ImageID>::swap(SpriteBase<ImageID> &sprite)
	{
		if (getManager() == sprite.getManager())
		{
			fm::Size tmpId = getId();
			setId(sprite.getId());
			sprite.setId(tmpId);
			
			getManager()->handleEmplace(*this);
			getManager()->handleEmplace(sprite);
		}
		else if (getManager() && !sprite.getManager())
		{
			sprite.setId(getId());
			sprite.setManager(getManager());
			
			setManager(nullptr);
			
			sprite.getManager()->handleEmplace(sprite);
		}
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	SpriteBase<ImageID> &SpriteBase<ImageID>::setPosition(const fm::vec3 &pos)
	{
		getManager()->handlePosChange(getId(),pos);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	fm::vec3 SpriteBase<ImageID>::getPosition() const
	{
		return getManager()->fetchPos(getId());
	}

	/////////////////////////////////////////////////////////////
	template<class ImageID>
	SpriteBase<ImageID> &SpriteBase<ImageID>::setDirection(const fm::vec2 &dir)
	{
		getManager()->handleDirChange(getId(),dir);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	fm::vec2 SpriteBase<ImageID>::getDirection() const
	{
		return getManager()->fetchDir(getId());
	}

	/////////////////////////////////////////////////////////////
	template<class ImageID>
	SpriteBase<ImageID> &SpriteBase<ImageID>::setSize(const fm::vec2 &size)
	{
		getManager()->handleSizeChange(getId(),size);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	fm::vec2 SpriteBase<ImageID>::getSize() const
	{
		return getManager()->fetchSize(getId());
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	fm::Size SpriteBase<ImageID>::getId() const
	{
		return m_id;
	}

	/////////////////////////////////////////////////////////////
	template<class ImageID>
	SpriteManagerBase<ImageID> *SpriteBase<ImageID>::getManager() const
	{
		return m_manager;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	void SpriteBase<ImageID>::setId(fm::Size index)
	{
		m_id = index;
	}
	
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	void SpriteBase<ImageID>::setManager(SpriteManagerBase<ImageID> *manager)
	{
		m_manager = manager;
	}
}

#endif // FRONTIER_SPRITE_INL_INCLUDED