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
#ifndef FRONTIER_TEXTUREATLAS_INL_INCLUDED
#define FRONTIER_TEXTUREATLAS_INL_INCLUDED
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <algorithm>

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp>::MapPoint::MapPoint(const fg::Image *img,MT point,const fm::vec2 &leftdown) : img(img),
																											 point(point),
																											 leftdown(leftdown)
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
	inline TextureAtlas<MT,Cp>::GlyphPoint::GlyphPoint(const fm::rect2s &rct,const fm::vec2 &leftdown,MT point) : rct(rct),
																												  leftdown(leftdown),
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
			FrameBuffer m_updaterFbo;
			Shader m_updaterShader;
			bool m_shaderNeedsUpdate;
			bool m_fboNeedsUpdate;
			Texture m_tex;
			Node *m_root;

			/////////////////////////////////////////////////////////////
			void resizeTex(const fm::vec2s &newSize);

			/////////////////////////////////////////////////////////////
			Node *recursiveInsert(const fm::vec2s &rectSize);
		public:

			/////////////////////////////////////////////////////////////
			TextureAtlasImpl();

			/////////////////////////////////////////////////////////////
			TextureAtlasImpl(const TextureAtlasImpl &copy);

			/////////////////////////////////////////////////////////////
			~TextureAtlasImpl();

			/////////////////////////////////////////////////////////////
			Glyph upload(const fg::Image &img,const fm::vec2 &leftdown);

			/////////////////////////////////////////////////////////////
			Glyph upload(const fg::Texture &tex,const fm::vec2 &leftdown);

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
	
#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	TextureAtlas<MT,Cp>::TextureAtlas(const TextureAtlas &copy) : m_glyphTable(copy.m_glyphTable),
																  m_impl(new priv::TextureAtlasImpl(*(copy.m_impl))),
																  m_comp(copy.m_comp)
	{
		FRONTIER_HEAVYCOPY_NOTE;
	}
#endif
	
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	TextureAtlas<MT,Cp>::TextureAtlas(TextureAtlas &&move) : m_impl(nullptr)
	{
		this->swap(move);
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
	inline Glyph TextureAtlas<MT,Cp>::upload(const fg::Image &img,const MT &point,const fm::vec2 &leftdown)
	{
		return m_glyphTable[point] = m_impl->upload(img,leftdown);
	}

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline Glyph TextureAtlas<MT,Cp>::upload(const fg::Texture &tex,const MT &point,const fm::vec2 &leftdown)
	{
		return m_glyphTable[point] = m_impl->upload(tex,leftdown);
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
		return m_glyphTable.find(point) != m_glyphTable.end();
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
				upload(*points[i].img,points[i].point,points[i].leftdown);
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
			data[i].leftdown = glyph.leftdown;
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

#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp> &TextureAtlas<MT,Cp>::operator=(const TextureAtlas &atlas)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		delete m_impl;
		
		m_glyphTable = atlas.m_glyphTable;
		m_impl       = new priv::TextureAtlasImpl(*(atlas.m_impl));
		m_comp       = atlas.m_comp;
		
		return *this;
	}
#endif

	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	inline TextureAtlas<MT,Cp> &TextureAtlas<MT,Cp>::operator=(TextureAtlas &&atlas)
	{
		this->swap(atlas);
		
		return *this;
	}
		
	/////////////////////////////////////////////////////////////
	template<class MT,class Cp>
	typename TextureAtlas<MT,Cp>::reference TextureAtlas<MT,Cp>::swap(TextureAtlas &texAtl)
	{
		m_glyphTable.swap(texAtl.m_glyphTable);
		
		std::swap(m_impl,texAtl.m_impl);
		std::swap(m_comp,texAtl.m_comp);
		
		return *this;
	}
}
#endif //FRONTIER_TEXTUREATLAS_INL_INCLUDED
