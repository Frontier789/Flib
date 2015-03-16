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
#ifndef FRONTIER_TEXTUREATLAS_INL_INCLUDED
#define FRONTIER_TEXTUREATLAS_INL_INCLUDED
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <algorithm>

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp>::MapPoint::MapPoint(const fg::Image *img,MT point,fm::vec2s midpt) : img(img),
																									point(point),
																									midpt(midpt)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline bool TextureAtlas<MT,Cp>::MapPoint::compare(const MapPoint &l,const MapPoint &r)
	{
		return l.img->getSize().area() > r.img->getSize().area();
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp>::GlyphPoint::GlyphPoint(fm::rect2s rct,fm::vec2s midpt,MT point) : rct(rct),
																								  midpt(midpt),
																								  point(point)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline bool TextureAtlas<MT,Cp>::GlyphPoint::compare(const GlyphPoint &l,const GlyphPoint &r)
	{
		return l.rct.size.area() > r.rct.size.area();
	}

	namespace priv
	{
		class TextureAtlasImpl
		{
		public:
			class Node;

		private:
			Texture m_tex;
			Node *m_root;

			/////////////////////////////////////////////////////////////
			void resizeTex(const fm::vec2s &newSize);
		public:

			/////////////////////////////////////////////////////////////
			TextureAtlasImpl();

			/////////////////////////////////////////////////////////////
			TextureAtlasImpl(const TextureAtlasImpl &copy);

			/////////////////////////////////////////////////////////////
			~TextureAtlasImpl();

			/////////////////////////////////////////////////////////////
			Glyph upload(const fg::Image &img,const fm::vec2s &midpt);

			/////////////////////////////////////////////////////////////
			void reset();

			/////////////////////////////////////////////////////////////
			Glyph *upload(const fg::Image &atlas,void *glyphPoints,fm::Size stride,fm::Size count);

			/////////////////////////////////////////////////////////////
			Texture &getTexture();

			/////////////////////////////////////////////////////////////
			const Texture &getTexture() const;
		};
	}
	
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	TextureAtlas<MT,Cp>::TextureAtlas(const TextureAtlas &copy) : m_glyphTable(copy.m_glyphTable),
																  m_impl(new priv::TextureAtlasImpl(*(copy.m_impl))),
																  m_comp(copy.m_comp)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp>::TextureAtlas(const Cp &comparator) : m_impl(new priv::TextureAtlasImpl),
																	 m_comp(comparator)
	{

	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp>::~TextureAtlas()
	{
		delete m_impl;
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline Glyph TextureAtlas<MT,Cp>::upload(const fg::Image &img,const MT &point,const fm::vec2s &midpt)
	{
		return m_glyphTable[point] = m_impl->upload(img,midpt);
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline Glyph TextureAtlas<MT,Cp>::fetch(const MT &point) const
	{
		typename std::map<MT,Glyph,Cp>::const_iterator it = m_glyphTable.find(point);

		return it==m_glyphTable.end() ? Glyph() : it->second;
	}
		
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline bool TextureAtlas<MT,Cp>::isUploaded(const MT &point) const
	{
		return m_glyphTable.find(point)!=m_glyphTable.end();
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline void TextureAtlas<MT,Cp>::reset()
	{
		m_glyphTable.clear();
		m_impl->reset();
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline void TextureAtlas<MT,Cp>::upload(TextureAtlas<MT,Cp>::MapPoint *points,fm::Size pointCount,bool sortPoints)
	{
		// sort if needed
		if (sortPoints)
			std::sort(points,points+pointCount,TextureAtlas<MT,Cp>::MapPoint::compare);

		// upload each point
		for (fm::Size i=0;i<pointCount;i++)
			if (points[i].img)
				upload(*points[i].img,*points[i].point,*points[i].midpt);
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline void TextureAtlas<MT,Cp>::upload(const fg::Image &atlas,TextureAtlas<MT,Cp>::GlyphPoint *points,fm::Size pointCount,bool sortPoints)
	{
		// sort if needed
		if (sortPoints)
			std::sort(points,points+pointCount,TextureAtlas<MT,Cp>::GlyphPoint::compare);

		// upload in one go
		Glyph *newGlyphs = m_impl->upload(atlas,points,sizeof(*points),pointCount);

		// insert resulted data
		for(fm::Size i=0;i<pointCount;i++)
			m_glyphTable.insert(std::pair<MT,Glyph>(points[i].point,newGlyphs[i]));

		delete[] newGlyphs;
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline void TextureAtlas<MT,Cp>::optimize()
	{
		// retrieve Texture
		fg::Image img = m_impl->getTexture().copyToImage();

		fm::Size glyphCount = m_glyphTable.size();

		// extract data from map
		GlyphPoint *data = new GlyphPoint[glyphCount];
		typename std::map<MT,Glyph,Cp>::const_iterator it;
		fm::Size i;

		for (it=m_glyphTable.begin(),i=0;it!=m_glyphTable.end();++it,i++)
		{
			const Glyph &glyph = it->second;
			data[i].rct = fm::rect2s(glyph.pos,glyph.size);
			data[i].midpt = glyph.leftdown;
			data[i].point = it->first;
		}


		// clear the map
		m_glyphTable.clear();
		m_impl->reset();

		// upload with sorting
		upload(img,(GlyphPoint*)data,glyphCount,true);

		// free used memory
		delete[] data;
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline Texture &TextureAtlas<MT,Cp>::getTexture()
	{
		return m_impl->getTexture();
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline const Texture &TextureAtlas<MT,Cp>::getTexture() const
	{
		return m_impl->getTexture();
	}
}
#endif //FRONTIER_TEXTUREATLAS_INL_INCLUDED
