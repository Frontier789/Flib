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
#ifndef FRONTIER_SPRITEMANAGER_HPP_INCLUDED
#define FRONTIER_SPRITEMANAGER_HPP_INCLUDED

#include <FRONTIER/Graphics/FloatAttributeUpdater.hpp>
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/TextureAtlas.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <string>

#define FRONTIER_SPRITEMANAGER

namespace fg
{
	class Image;
}

namespace fg
{
	template<class> class SpriteBase;
	
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		/// @brief Base class with for non templated functions for sprite manager
		/// 
		/////////////////////////////////////////////////////////////
		class SpriteManagerBaseNonTemplate : public fg::Drawable
		{
		public:
			fg::DrawData m_drawData; ///< The drawData
			bool m_useFrames; ///< Indicates whether frames are used
			
			FloatAttributeUpdater m_posProp; ///< Property for positions
			FloatAttributeUpdater m_dirProp; ///< Property for direction
			FloatAttributeUpdater m_sizProp; ///< Property for size
			FloatAttributeUpdater m_uvpProp; ///< Property for texture position
			FloatAttributeUpdater m_uvsProp; ///< Property for texture size
			FloatAttributeUpdater m_frmProp; ///< Property for frame size
			
			FloatAttributeUpdater m_vertPtsProp; ///< Property for vertex poses
			FloatAttributeUpdater m_vertUVsProp; ///< Property for vertex texture poses
			
			fg::ShaderManager m_instancingShader; ///< The shader used when instancing
			fm::Size m_spriteCount; ///< Number of sprites managed
			fm::Result m_insError;  ///< The result of the shader loading
			bool m_useInstancing;   ///< Indicates whether instancing is used
			
			fg::Glyph (*m_glyphGetterFunc)(SpriteManagerBaseNonTemplate*,fm::Size); ///< Function for propagating glyph back
		
		protected:
			/////////////////////////////////////////////////////////////
			/// @brief Load the needed shader
			/// 
			/////////////////////////////////////////////////////////////
			void loadShader();
			
			/////////////////////////////////////////////////////////////
			/// @brief Load the draw data
			/// 
			/////////////////////////////////////////////////////////////
			void setupDrawData();
			
			/////////////////////////////////////////////////////////////
			/// @brief Build the vertices of a quad for sprite
			/// 
			/// @param pts The points returned
			/// @param uvs The texture positions returned
			/// @param size The size of the quad
			/// @param shape The glyph for the sprite
			/// @param dir The direction of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			void buildVertices(fm::vec3 **pts,
							   fm::vec2 **uvs,
							   fm::vec3 pos,
							   fm::vec2 size,
							   const fg::Glyph &shape,
							   fm::vec2 dir);
			
			/////////////////////////////////////////////////////////////
			/// @brief Helper function for extracting edge points from sprite mesh
			/// 
			/// @param pts The control points returned
			/// @param spriteIndex The index f the sprite to be queried
			/// 
			/////////////////////////////////////////////////////////////
			void getControlPoints(fm::vec3 (&pts)[4],fm::Size spriteIndex) const;
		
			/////////////////////////////////////////////////////////////
			/// @brief Handle creating s sprite
			/// 
			/// @param spriteIndex An invalid sprite index
			/// @param shape The glyph to use
			/// @param pos The position of the new sprite
			/// @param size The size of the new sprite
			/// @param dir The direction of the new sprite
			/// 
			/////////////////////////////////////////////////////////////
			void handleCreate(fm::Size spriteIndex,const fg::Glyph &shape,const fm::vec3 &pos,const fm::vec2 &size,const fm::vec2 &dir);
			
			/////////////////////////////////////////////////////////////
			/// @brief Handle moving a sorites' data
			/// 
			/// @param index The index to move to
			/// @param oldIndex The inde to move from
			/// 
			/////////////////////////////////////////////////////////////
			void handleMoveFrom(fm::Size index,fm::Size oldIndex);
			
			/////////////////////////////////////////////////////////////
			/// @brief Handle destroying the last sprite
			/// 
			/////////////////////////////////////////////////////////////
			void handlePropPop();
			
		public:
			/////////////////////////////////////////////////////////////
			/// @brief Check if the manager uses instancing
			/// 
			/// @return True iff instancing is in use
			/// 
			/////////////////////////////////////////////////////////////
			bool useInstancing() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the result of loading the shader (if any)
			/// 
			/// @return The result
			/// 
			/////////////////////////////////////////////////////////////
			const fm::Result &getShaderLoadResult() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Handle changing the image id of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// @param shape The new shape of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			void handleImageIDChange(fm::Size spriteIndex,const fg::Glyph &shape);
		
			/////////////////////////////////////////////////////////////
			/// @brief Handle changing the position of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// @param pos The new position of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			void handlePosChange(fm::Size spriteIndex,const fm::vec3 &pos);
			
			/////////////////////////////////////////////////////////////
			/// @brief Handle changing the direction of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// @param dir The new direction of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			void handleDirChange(fm::Size spriteIndex,const fm::vec2 &dir);
			
			/////////////////////////////////////////////////////////////
			/// @brief Handle changing the size of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// @param size The new size of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			void handleSizeChange(fm::Size spriteIndex,const fm::vec2 &size);
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the current position of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// 
			/// @return The current position of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			fm::vec3 fetchPos(fm::Size spriteIndex) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the current direction of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// 
			/// @return The current direction of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			fm::vec2 fetchDir(fm::Size spriteIndex) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Get the current size of a sprite
			/// 
			/// @param spriteIndex The index of a valid sprite 
			/// 
			/// @return The current size of the sprite
			/// 
			/////////////////////////////////////////////////////////////
			fm::vec2 fetchSize(fm::Size spriteIndex) const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Check if frames are used
			/// 
			/// @return True iff frames are used
			/// 
			/////////////////////////////////////////////////////////////
			bool useFrames() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/// @param useFrames Decides whether framed sprites should be used
			/// @param glyphGetterFunc Glyph propagating function
			/// 
			/////////////////////////////////////////////////////////////
			SpriteManagerBaseNonTemplate(bool useFrames = false,fg::Glyph (*glyphGetterFunc)(SpriteManagerBaseNonTemplate*,fm::Size) = nullptr);
			
			/////////////////////////////////////////////////////////////
			/// @brief Update the drawcall
			/// 
			/////////////////////////////////////////////////////////////
			void updateDrawCall();
			
			/////////////////////////////////////////////////////////////
			/// @brief Handle being updated
			/// 
			/////////////////////////////////////////////////////////////
			void onUpdate(const fm::Time & /* dt */ ) override {};
			
			/////////////////////////////////////////////////////////////
			/// @brief Draw the sprites of the manager
			/// 
			/// @param shader The shader to draw with
			/// @param tex The texture from the atlas
			/// 
			/////////////////////////////////////////////////////////////
			void onDrawTex(fg::ShaderManager &shader,fg::Texture &tex);
			
			template<class ImageID>
			friend class SpriteBase;
		};
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to manage sprites
	/// 
	/////////////////////////////////////////////////////////////
	template<class ImageID>
	class SpriteManagerBase : public priv::SpriteManagerBaseNonTemplate
	{
		struct SpriteData
		{
			SpriteBase<ImageID> *ptr;
			ImageID imgID;
		};
		
		std::vector<SpriteData> m_spriteDatas;
		fg::TextureAtlas<ImageID> m_atlas;
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle creating s sprite
		/// 
		/// @param sprite An invalid sprite 
		/// @param imgID The imageId to use
		/// @param pos The position of the new sprite
		/// @param size The size of the new sprite
		/// @param dir The direction of the new sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handleCreate(SpriteBase<ImageID> &sprite,ImageID imgID,const fm::vec3 &pos,const fm::vec2 &size,const fm::vec2 &dir);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle destroying a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/////////////////////////////////////////////////////////////
		void handleDestroy(SpriteBase<ImageID> &sprite);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle emplacing a sprite
		/// 
		/// @param sprite A (moved) valid sprite 
		/// 
		/////////////////////////////////////////////////////////////
		void handleEmplace(SpriteBase<ImageID> &sprite);
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param useFrames Decides whether framed sprites should be used
		/// 
		/////////////////////////////////////////////////////////////
		SpriteManagerBase(bool useFrames = false);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~SpriteManagerBase();
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle changing the image id of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// @param imgID The new image id of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		void handleImageIDChange(SpriteBase<ImageID> &sprite,ImageID imgID);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current image id of a sprite
		/// 
		/// @param sprite A valid sprite 
		/// 
		/// @return The current image id of the sprite
		/// 
		/////////////////////////////////////////////////////////////
		ImageID fetchImgID(const SpriteBase<ImageID> &sprite) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Register an image with given id for use
		/// 
		/// @param img The image to register
		/// @param id The id to associate with
		/// @param frameSize The frame size for the image
		/// 
		/////////////////////////////////////////////////////////////
		void addImage(const fg::Image &img,ImageID id,const fm::vec2 &frameSize = fm::vec2());
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the size of a registered image
		/// 
		/// @param id The id of the image to query
		/// 
		/// @return The size of the image
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getImageSize(ImageID id) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the underlying atlas
		/// 
		/// @return The atlas
		/// 
		/////////////////////////////////////////////////////////////
		const fg::TextureAtlas<ImageID> &getAtlas() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the underlying atlas
		/// 
		/// @return The atlas
		/// 
		/////////////////////////////////////////////////////////////
		fg::TextureAtlas<ImageID> &getAtlas();
			
		/////////////////////////////////////////////////////////////
		/// @brief Create a sprite with given image id
		/// 
		/// @param id The id to use
		/// 
		/// @return The sprite
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase<ImageID> getSprite(ImageID id);
			
		/////////////////////////////////////////////////////////////
		/// @brief Fetch an existing sprite with given id
		/// 
		/// @param id The id search
		/// 
		/// @return The sprite
		/// 
		/////////////////////////////////////////////////////////////
		SpriteBase<ImageID> &getSpriteById(fm::Size id);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of existing sprites
		/// 
		/// @return The number of sprites
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getSpriteCount() const;
			
		/////////////////////////////////////////////////////////////
		/// @brief Draw the sprites of the manager
		/// 
		/// @param shader The shader to draw with
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
		
		friend SpriteBase<ImageID>;
	};
	
	typedef SpriteManagerBase<std::string> SpriteManager;
}

#endif // FRONTIER_SPRITEMANAGER_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/SpriteManager.inl>
#endif // FRONTIER_DONT_INCLUDE_INL

