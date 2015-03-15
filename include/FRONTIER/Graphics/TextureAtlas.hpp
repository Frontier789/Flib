////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
#ifndef FRONTIER_TEXTUREATLAS_HPP_INCLUDED
#define FRONTIER_TEXTUREATLAS_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/Glyph.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Rect.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_TEXTUREATLAS
#include <map>

namespace fg
{
	class Texture;
	class Image;
	
	namespace priv
	{
		class FRONTIER_API TextureAtlasImpl;
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to manage <a href="http://en.wikipedia.org/wiki/Texture_atlas">Texture Atlases</a>
	/// 
	/// All uploaded textures are associated with a key of type MappedType
	/// which with the textures' loaction can be fetced
	/// 
	/////////////////////////////////////////////////////////////
	template<class MappedType,class Compare = std::less<MappedType> >
	class TextureAtlas : public fm::NonCopyable
	{
		std::map<MappedType,Glyph,Compare> m_glyphTable; ///< The map of uploaded textures
		priv::TextureAtlasImpl *m_impl; ///< The underlying non-templated implementation
		Compare m_comp; ///< Used when sorting MappedType typed data
	public:
		typedef TextureAtlas &reference;
		typedef const TextureAtlas &const_reference;
		

		/////////////////////////////////////////////////////////////
		/// @brief Simple class that holds the attributes of a key and the associated Image
		/// 
		/////////////////////////////////////////////////////////////
		class MapPoint
		{
		public:
			const fg::Image *img; ///< The associated Image
			MappedType point; ///< The key value
			fm::vec2s midpt;  ///< The associated offset
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/// @param img The associated Image
			/// @param point The key value
			/// @param midpt The associated offset
			/// 
			/////////////////////////////////////////////////////////////
			MapPoint(const fg::Image *img = fm::nullPtr,MappedType point = MappedType(),fm::vec2s midpt = fm::vec2s());
			
			/////////////////////////////////////////////////////////////
			/// @brief The internal comparation function that is used when sorting MapPoints
			/// 
			/// @param l The left operand
			/// @param r The right operand
			/// 
			/////////////////////////////////////////////////////////////
			static bool compare(const MapPoint &l,const MapPoint &r);
		};

		/////////////////////////////////////////////////////////////
		/// @brief Simple class that holds the attributes of a key associated with the subimage of one Image
		/// 
		/////////////////////////////////////////////////////////////
		class GlyphPoint
		{
		public:
			fm::rect2s rct;   ///< The layout of the subimage
			fm::vec2s  midpt; ///< The associated offset
			MappedType point; ///< The key value
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/// @param rct The layout of the subimage
			/// @param midpt The associated offset
			/// @param point The key value
			/// 
			/////////////////////////////////////////////////////////////
			GlyphPoint(fm::rect2s rct = fm::rect2s(),fm::vec2s midpt = fm::vec2s(),MappedType point = MappedType());
			
			/////////////////////////////////////////////////////////////
			/// @brief The internal comparation function that is used when sorting GlyphPoints
			/// 
			/// @param l The left operand
			/// @param r The right operand
			/// 
			/////////////////////////////////////////////////////////////
			static bool compare(const GlyphPoint &l,const GlyphPoint &r);
		};
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param comparator The instance of type Compare to be used when sorting MappedType typed data
		/// 
		/////////////////////////////////////////////////////////////
		TextureAtlas(const Compare &comparator = Compare());
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~TextureAtlas();
		
		/////////////////////////////////////////////////////////////
		/// @brief Add a new Texture to the atlas
		/// 
		/// @param img The image of the new Texture
		/// @param point The associated key
		/// @param midpt The associated offset
		/// 
		/// @return The properties of the uploaded Texture in a Glyph
		/// 
		/////////////////////////////////////////////////////////////
		Glyph upload(const fg::Image &img,const MappedType &point,const fm::vec2s &midpt=fm::vec2s());
		
		/////////////////////////////////////////////////////////////
		/// @brief Add new Textures to the atlas
		/// 
		/// It's more optimal to upload more 
		/// Textures with sorting than one-by-one
		/// 
		/// @param points The data of each new Texture to be added
		/// @param pointCount The number of MapPoints in @a points
		/// @param sortPoints Iff true @a points will be sorted
		/// 
		/////////////////////////////////////////////////////////////
		void upload(MapPoint *points,fm::Size pointCount,bool sortPoints=true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Add new Textures from one shared Image to the atlas
		/// 
		/// It's more optimal to upload more 
		/// Textures with sorting than one-by-one
		/// 
		/// @param atlas The Image that contains the new Textures
		/// @param points The data of each new Texture to be added
		/// @param pointCount The number of MapPoints in @a points
		/// @param sortPoints Iff true @a points will be sorted (as an optimization)
		/// 
		/////////////////////////////////////////////////////////////
		void upload(const fg::Image &atlas,GlyphPoint *points,fm::Size pointCount,bool sortPoints=true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the properties of an alredy uploaded Texture
		/// 
		/// The operation is logarithmic in complexity to the number of uploaded Textures
		/// If @a point is not found in m_glyphTable Glyph() is returned
		/// 
		/// @param point The key of the Texture to be retrieved
		/// 
		/// @param The properties of the Texture
		/// 
		/////////////////////////////////////////////////////////////
		Glyph fetch(const MappedType &point) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Reset the whole Atlas
		/// 
		/// All uploaded Textures are discarded
		/// 
		/////////////////////////////////////////////////////////////
		void reset();
		
		/////////////////////////////////////////////////////////////
		/// @brief Rearrange the uploaded Texture in a better way
		/// 
		/// It is a SLOW operation and invalidates every data 
		/// returned be TextureAtlas::fetch
		/// 
		/////////////////////////////////////////////////////////////
		void optimize();
		
		/////////////////////////////////////////////////////////////
		/// @brief Access the container Texture
		/// 
		/// @return The Texture
		/// 
		/////////////////////////////////////////////////////////////
		Texture &getTexture();
		
		/////////////////////////////////////////////////////////////
		/// @brief Access the container Texture
		/// 
		/// @return The Texture
		/// 
		/////////////////////////////////////////////////////////////
		const Texture &getTexture() const;
	};
}

#endif //FRONTIER_TEXTUREATLAS_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/TextureAtlas.inl>
#endif