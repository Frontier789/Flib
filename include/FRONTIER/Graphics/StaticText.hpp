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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_STATICTEXT_HPP_INCLUDED
#define FRONTIER_STATICTEXT_HPP_INCLUDED
#include <FRONTIER/System/Transformable.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Color.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/System/Vertex.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_STATICTEXT
#include <string>
#include <vector>
namespace fg
{
	class Font;
	
	class StaticText : public Drawable, public fm::Transformable, public fm::NonCopyable
	{
	private:
		const fg::Font *m_font;
		const fg::Texture *m_texture;
		std::string m_text;
		std::vector<fm::vertex2f> m_vertices;
		std::vector<unsigned int> m_indices;
		fg::Color m_color;
		fm::vec2 m_boundings;
		unsigned int m_align;
		unsigned int m_characterSize;
		bool m_kerningEnabled;
		void buildVertices();
	public:
		typedef StaticText &reference;
		typedef const StaticText &const_reference;

		class Align
		{
		public:
			enum {
				Left   = 0,
				Center = 1,
				Right  = 2
			};
		};

		StaticText();
		StaticText(const fg::Font &font,const std::string &text,const fg::Color &color = fg::Color::White,unsigned int characterSize = 64,unsigned int align = Align::Left,bool enableKerning = false);
		void setText(const std::string &text);
		void setFont(const fg::Font &font);
		void setColor(const fg::Color &color);
		void setAlign(unsigned int align);
		void setCharacterSize(unsigned int size);
		void enableKerning(bool enable = true);
		const fm::vec2 &getBoundings() const;
		const std::string &getText() const;
		const fg::Font *getFont() const;
		const fg::Color &getColor() const;
		unsigned int getAlign() const;
		unsigned int getCharacterSize() const;
		bool isKerningEnabled() const;

		virtual void draw(const fg::Texture *texture,const fg::Shader *shader) const;
	};
}


#endif //FRONTIER_STATICTEXT_HPP_INCLUDED
