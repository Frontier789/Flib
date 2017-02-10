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
#include <FRONTIER/Graphics/FloatTexture.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	fm::Int32  FloatTexture::getInternalFormat() const {return GL_RGBA32F;}

	/// constructor /////////////////////////////////////////////////////////
	FloatTexture::FloatTexture() : Texture::Texture()
	{

	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	FloatTexture::FloatTexture(const FloatTexture &copy) : Texture::Texture(copy)
	{
		
	}
#endif

	////////////////////////////////////////////////////////////
	FloatTexture::FloatTexture(FloatTexture &&move)
	{
		((Texture*)this)->swap(move);
	}

#ifndef FRONTIER_HEAVYCOPY_FORBID
	////////////////////////////////////////////////////////////
	FloatTexture &FloatTexture::operator=(const FloatTexture &copy)
	{
		*((Texture*)this) = (const Texture &)copy;
		
		return *this;
	}
#endif

	////////////////////////////////////////////////////////////
	FloatTexture &FloatTexture::operator=(FloatTexture &&move)
	{
		((Texture*)this)->swap(move);
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	FloatTexture::FloatTexture(const Image &img) : Texture::Texture(img)
	{

	}
}
