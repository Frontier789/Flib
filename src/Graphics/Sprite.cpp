////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)				///
///																	///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			 ///
/// condition that this disclaimer is not modified/removed.			///
/// You may not misclaim the origin of this software.				  ///
///																	///
/// If you use this software in your program/project a				 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							///
///																	///
/// You should have received a copy of GNU GPL with this software	  ///
///																	///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/RenderStates.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/Sprite.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vertex.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/Config.hpp>
namespace fg
{
	/// constructors /////////////////////////////////////////////////////////
	Sprite::Sprite() : m_texture(0),
					   m_color(fm::vec4::White)
	{
		setColor(m_color);
	}


	////////////////////////////////////////////////////////////
	Sprite::Sprite(Sprite::const_reference copy) : m_texture(copy.m_texture),
												   m_textureRect(copy.m_textureRect),
												   m_color(copy.m_color)
	{
		C(4)
			m_vertices[i] = copy.m_vertices[i];
	}


	////////////////////////////////////////////////////////////
	Sprite::Sprite(const Texture &texture) : m_texture(&texture),
											 m_color(fm::vec4::White)
	{
		setTextureRect(fm::rect2s(fm::vec2s(),texture.getSize()));
		setColor(m_color);
	}


	////////////////////////////////////////////////////////////
	Sprite::Sprite(const Texture &texture,const fm::rect2s &textureRect,bool accountTexPosInPos) : m_texture(&texture),
																								   m_color(fm::vec4::White)
	{
		setTextureRect(textureRect,accountTexPosInPos);
		setColor(m_color);
	}


	/// functions /////////////////////////////////////////////////////////
	Sprite::reference Sprite::setColor(const fm::vec4 &color)
	{
		m_color = color;
		
		C(4)
			m_vertices[i].clr = m_color;
		
		return *this;
	}


	////////////////////////////////////////////////////////////
	Sprite::reference Sprite::setTexture(const Texture &texture,bool resetTextureRect)
	{
		return setTexture(&texture,resetTextureRect);
	}


	////////////////////////////////////////////////////////////
	Sprite::reference Sprite::setTexture(const Texture *texture,bool resetTextureRect)
	{
		m_texture = texture;
		if (resetTextureRect)
			setTextureRect(fm::rect2s(fm::vec2s(),texture->getSize()));
		return *this;
	}


	////////////////////////////////////////////////////////////
	Sprite::reference Sprite::setTextureRect(fm::rect2s textureRect,bool accountTexPosInPos)
	{
		Cx(2)Cy(2)
			m_vertices[2*x+y].pos    = fm::vec2(x,y)*textureRect.size + (accountTexPosInPos ? textureRect.pos : fm::vec2s()),
			m_vertices[2*x+y].texPos = fm::vec2(x,y)*textureRect.size + textureRect.pos;
		m_textureRect = textureRect;
		return *this;
	}


	////////////////////////////////////////////////////////////
	const fm::vec4 &Sprite::getColor() const
	{
		return m_color;
	}


	////////////////////////////////////////////////////////////
	const Texture *Sprite::getTexture() const
	{
		return m_texture;
	}


	////////////////////////////////////////////////////////////
	const fm::rect2s &Sprite::getTextureRect() const
	{
		return m_textureRect;
	}


	////////////////////////////////////////////////////////////
	void Sprite::draw(const fg::RenderStates &states) const
	{
		fg::RenderStates states2 = states;
		//states2.transform = states.transform * getTransform();
		states2.texture = m_texture;
		fg::draw(m_vertices,fg::TriangleStrip,states2);
	}


	/// operators /////////////////////////////////////////////////////////
	fm::vertex2f &Sprite::operator[](unsigned int index)
	{
		return m_vertices[index];
	}


	////////////////////////////////////////////////////////////
	const fm::vertex2f &Sprite::operator[](unsigned int index) const
	{
		return m_vertices[index];
	}
}
