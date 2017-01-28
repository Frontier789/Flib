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
#include <FRONTIER/Graphics/DepthTexture.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	fm::Int32  DepthTexture::getInternalFormat() const {return GL_DEPTH_COMPONENT32;}
	fm::Uint32 DepthTexture::getAttachement() const {return GL_DEPTH_ATTACHMENT;}
	fm::Uint32 DepthTexture::getFormat() const {return GL_DEPTH_COMPONENT;}
	fm::Uint32 DepthTexture::getType() const {return GL_UNSIGNED_INT;}

	/// constructor /////////////////////////////////////////////////////////
	DepthTexture::DepthTexture() : Texture::Texture()
	{

	}

	////////////////////////////////////////////////////////////
	DepthTexture::DepthTexture(const DepthTexture &copy) : Texture::Texture(copy)
	{

	}

	////////////////////////////////////////////////////////////
	DepthTexture::DepthTexture(const Image &img) : Texture::Texture(img)
	{

	}
	/*
	float fulldist = (float(*((unsigned int*)(&img.getPixel(x,y)))))

	double ratio = fulldist/((unsigned int)(-1));

	fg::Color c = fg::Color(ratio*255);

	img.setPixel(x,y,c);

*/



	////////////////////////////////////////////////////////////
	fg::Image DepthTexture::copyConvertToImage(fg::Color znear,fg::Color zfar) const
	{
		fg::Image ret = copyToImage();

		Cv(ret.getSize())
		{
			fm::Uint32 *ptr = (fm::Uint32*)(&ret.at(p));
			
			double ratio = (double)(*ptr) / (fm::Uint32)(-1);
			
			fg::Color c = znear*(1.0-ratio) + zfar*ratio;
			
			ret.set(p,c);
		}
		
		return ret;
	}
}
