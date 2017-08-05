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
#ifndef FRONTIER_SPRITE_HPP_INCLUDED
#define FRONTIER_SPRITE_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>

namespace fg
{
	template<class> class SpriteManagerBase;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to easily manage a lightweight sprite
	///
	/// Requires a SpriteManager
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	class SpriteBase
	{
		SpriteManagerBase<ImageID> *m_manager; ///< The manager of the sprite
		fm::Size m_id; ///< The id of the sprite
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~SpriteBase();
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the sprite with given manager and image
		/// 
		/// @param manager The manager
		/// @param imgID The image id
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase(SpriteManagerBase<ImageID> &manager,ImageID imgID = ImageID());
		
        /////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param sprite The sprite to copy
		/// 
        /////////////////////////////////////////////////////////////
		SpriteBase(const SpriteBase<ImageID> &sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param sprite The sprite to move
		/// 
        /////////////////////////////////////////////////////////////
		SpriteBase(SpriteBase<ImageID> &&sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		/// 
		/// @param sprite The sprite to copy
		/// 
		/// @return Reference to itself
		/// 
        /////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &operator=(const SpriteBase<ImageID> &sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief Move assignment
		/// 
		/// @param sprite The sprite to move
		/// 
		/// @return Reference to itself
		/// 
        /////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &operator=(SpriteBase<ImageID> &&sprite);
		
        /////////////////////////////////////////////////////////////
		/// @brief swap two sprites
		/// 
		/// @param sprite The sprite to swap with
		/// 
		/// @return Reference to itself
		/// 
        /////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &swap(SpriteBase<ImageID> &sprite);

		/////////////////////////////////////////////////////////////
		/// @brief Set the image of the sprite
		/// 
		/// @param imgID The image id
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &setImageID(ImageID imgID);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the image of the sprite
		/// 
		/// @return The image id
		/// 
		/////////////////////////////////////////////////////////////
		ImageID getImageID() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen position of the sprite
		/// 
		/// @param pos The position on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &setPosition(const fm::vec3 &pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen position of the sprite
		/// 
		/// @return The onscreen position
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec3 getPosition() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the direction the sprite is facing
		/// 
		/// @param dir The direction
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &setDirection(const fm::vec2 &dir);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the direction the sprite is facing
		/// 
		/// @return The direction of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getDirection() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the onscreen size of the sprite
		/// 
		/// @param size The size on the screen
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &setSize(const fm::vec2 &size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the onscreen size of the sprite
		/// 
		/// @return The onscreen size
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the id of the sprite
		/// 
		/// Mainly used by SpriteManager
		/// 
		/// @return The id
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getId() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the manager of the sprite
		/// 
		/// @return The manager
		/// 
		/////////////////////////////////////////////////////////////
		SpriteManagerBase<ImageID> *getManager() const;
	
	private:
		/////////////////////////////////////////////////////////////
		/// @brief Set the id of the sprite
		/// 
		/// Mainly used by SpriteManager
		/// 
		/// @param index The new index
		/// 
		/////////////////////////////////////////////////////////////
		void setId(fm::Size index);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the manager of the sprite
		/// 
		/// Mainly used by SpriteManager
		/// 
		/// @param manager The manager
		/// 
		/////////////////////////////////////////////////////////////
		void setManager(SpriteManagerBase<ImageID> *manager);
		
		friend SpriteManagerBase<ImageID>;
	};
	
	typedef SpriteBase<int> Sprite;
}

#endif // FRONTIER_SPRITE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Sprite.inl>
#endif // FRONTIER_DONT_INCLUDE_INL

