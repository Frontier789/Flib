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
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Rect.hpp>
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image.h"
#include "jpge/jpge.cpp"
#include <cstring>
#include <string>
namespace fg
{
	/// constructors /////////////////////////////////////////////////////////
	Image::Image() : m_sizeW(0),
					 m_sizeH(0)
	{

	}


	////////////////////////////////////////////////////////////
	Image::Image(fm::Size width,fm::Size height,const Color &color)
	{
		create(width,height,color);
	}


	////////////////////////////////////////////////////////////
	Image::Image(const fm::vec2s &size,const Color &color)
	{
		create(size.w,size.h,color);
	}


	////////////////////////////////////////////////////////////
	Image::Image(const Image &copy)
	{
		create(copy);
	}


	////////////////////////////////////////////////////////////
	Image::Image(const Image &copy,const fm::rect2s &sourceRect)
	{
		create(copy,sourceRect);
	}



	/// functions /////////////////////////////////////////////////////////
	typename Image::reference Image::create(fm::Size width,fm::Size height,const Color &color)
	{
		if (width && height)
		{
			m_sizeW = width;
			m_sizeH = height;
			m_pixels.resize(0),
			m_pixels.resize(width*height,color);
		}
		else
			m_sizeW = 0,
			m_sizeH = 0,
			m_pixels.clear();
		return *this;
	}


    ////////////////////////////////////////////////////////////
	typename Image::reference Image::create(const fm::vec2s &size,const Color &color)
	{
		return create(size.w,size.h,color);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::create(fm::Size width,fm::Size height,const Color *pixels)
	{
		if (width && height)
		{
			m_sizeW = width;
			m_sizeH = height;
			m_pixels = std::vector<Color>((Color *)pixels,((Color *)pixels)+width*height);
		}
		else
			m_sizeW = 0,
			m_sizeH = 0,
			m_pixels.clear();
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::create(const fm::vec2s &size,const Color *pixels)
	{
		return create(size.w,size.h,pixels);
	}


	////////////////////////////////////////////////////////////
    typename Image::reference Image::create(const Image &copy)
	{
		create(copy.getSize());
		copyFrom(copy,0,0,fm::rect2s(0,0,0,0));
		return *this;
	}


	////////////////////////////////////////////////////////////
    typename Image::reference Image::create(const Image &copy,const fm::rect2s &sourceRect)
	{
		create(sourceRect.area() ? sourceRect.size : fm::vec2s(copy.getSize())-sourceRect.pos);
		copyFrom(copy,0,0,sourceRect);
		return *this;
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,const fm::vec2s &destPos,const fm::rect2s &sourceRect,bool useAlpha) const
	{
		destination.copyFrom(*this,destPos,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha) const
	{
		destination.copyFrom(*this,destX,destY,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copyFrom(const Image &source,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha)
	{
		// only copy if the source is valid
		if (!source.m_sizeW || !source.m_sizeH/* || !m_sizeW || !m_sizeH*/)
			return *this;
		
		// copy the source rect so we can adjust it
		fm::rect2s sRect = sourceRect;
		
		// ah! we want to copy the whole image!
		if (!sRect.area())
		{
			sRect.pos(0,0);
			sRect.size(m_sizeW,m_sizeH);
		}
		else
		{
			// do not be out of the source
			if (sRect.size.w + sRect.pos.x > source.m_sizeW) sRect.size.w = source.m_sizeW - sRect.pos.x;
			if (sRect.size.h + sRect.pos.y > source.m_sizeH) sRect.size.h = source.m_sizeH - sRect.pos.y;
		}
		
		// do not be out of the destination
		if (destX + sRect.size.w > m_sizeW) sRect.size.w = m_sizeW - destX;
		if (destY + sRect.size.h > m_sizeH) sRect.size.h = m_sizeH - destY;
		
		// use alpha mixing
		if (useAlpha)
		{
			/// used technique: http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
			/// for EVERY PIXEL -> slow
			Cxy(sRect.size.w,sRect.size.h)
			{
				// get the source pixell
				fm::vec4 src = source.getPixel(sourceRect.pos+fm::vec2s(x,y));
				
				// get the destination pixell
				fm::vec4 dst = getPixel(destX+x,destY+y);
				
				// compute result alpha
				float alpha = src.a+dst.a*(1-src.a);
				
				// avoid division by 0
				if (!alpha)
					setPixel(destX+x,destY+y,fm::vec4(0,0,0,0));
				else
					setPixel(destX+x,destY+y,fm::vec4(fm::vec3(src*src.a+dst*dst.a*(1.f-src.a))/alpha,alpha));
			}
		}
		else
		{
			fm::Size copyBytesPerRow = sRect.size.w*sizeof(Color);
			
			// we can use memcpy which is a lot faster
			if (sRect.size.w == m_sizeW && sRect.size.w == source.m_sizeW)
				std::memcpy(&getPixel(destX,destY),&source.getPixel(sourceRect.pos.x,sourceRect.pos.y), copyBytesPerRow*sRect.size.h);
			else
				C(sRect.size.h)
					std::memcpy(&getPixel(destX,destY+i), &source.getPixel(sourceRect.pos.x,sourceRect.pos.y+i), copyBytesPerRow);
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copyFrom(const Image &source,const fm::vec2s &destPos,const fm::rect2s &sourceRect,bool useAlpha)
	{
		return copyFrom(source,destPos.x,destPos.y,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,const fm::vec2s &destPos,bool useAlpha) const
	{
		destination.copyFrom(*this,destPos,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,fm::Size destX,fm::Size destY,bool useAlpha) const
	{
		destination.copyFrom(*this,destX,destY,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copyFrom(const Image &source,fm::Size destX,fm::Size destY,bool useAlpha)
	{
		return copyFrom(source,destX,destY,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copyFrom(const Image &source,const fm::vec2s &destPos,bool useAlpha)
	{
		return copyFrom(source,destPos.x,destPos.y,fm::rect2s(0,0,0,0),useAlpha);
	}


	////////////////////////////////////////////////////////////
	bool Image::loadFromFile(const std::string &filename)
	{
		// empty current pixels
		m_pixels.clear();

		// ask stbi to load the file
		int width, height, channels;
		unsigned char *ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		
		// if he succeeded
		if (ptr && width && height)
		{
			// copy the data
			m_sizeW = width;
			m_sizeH = height;
			m_pixels.resize(width * height * 4);
			memcpy(&m_pixels[0], ptr, m_pixels.size());
			
			// let stbi free its used memory
			stbi_image_free(ptr);
			
			return true;
		}
		else
		{
			fg_log << "Failed to load image \"" << filename << "\". Reason : " << stbi_failure_reason() << std::endl;
			return false;
		}
	}


	////////////////////////////////////////////////////////////
	const fm::vec2s &Image::getSize() const
	{
		return *((fm::vec2s*)&m_sizeW);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::setPixel(fm::Size x,fm::Size y,const Color &color)
	{
		m_pixels[x+y*m_sizeW] = color;
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::setPixel(const fm::vec2s &pos,const Color &color)
	{
		return setPixel(pos.x,pos.y,color);
	}


	////////////////////////////////////////////////////////////
	const Color &Image::getPixel(fm::Size x,fm::Size y) const
	{
		return m_pixels[x+y*m_sizeW];
	}


	////////////////////////////////////////////////////////////
	const Color &Image::getPixel(const fm::vec2s &pos) const
	{
		return getPixel(pos.x,pos.y);
	}


	////////////////////////////////////////////////////////////
	Color &Image::getPixel(fm::Size x,fm::Size y)
	{
		return m_pixels[x+y*m_sizeW];
	}


	////////////////////////////////////////////////////////////
	Color &Image::getPixel(const fm::vec2s &pos)
	{
		return getPixel(pos.x,pos.y);
	}


	////////////////////////////////////////////////////////////
	Color *Image::getPixelsPtr()
	{
		return &m_pixels[0];
	}


	////////////////////////////////////////////////////////////
	const Color *Image::getPixelsPtr() const
	{
		return &m_pixels[0];
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::flipHorizontally()
	{
		Cx(m_sizeW/2.f)
			Cy(m_sizeH)
				std::swap(m_pixels[x+y*m_sizeW],m_pixels[(m_sizeW-x-1)+y*m_sizeW]);
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::flipVertically()
	{
		Cx(m_sizeW)
			Cy(m_sizeH/2.f)
				std::swap(m_pixels[x+y*m_sizeW],m_pixels[x+(m_sizeH-y-1)*m_sizeW]);
		return *this;
	}


	////////////////////////////////////////////////////////////
	bool Image::saveToFile(const std::string &filename)
	{
		// if the image is valid
		if (!m_pixels.empty() && (m_sizeW > 0) && (m_sizeH > 0))
		{
			// find the extension
			int dotPos = filename.find_last_of(".");
			std::string extension = filename.substr(dotPos+1,filename.size()-dotPos);
			
			// lowercase
			C(extension.length())
				extension[i] = (extension[i]>='A' && extension[i]<='Z') ? (extension[i]-'A'+'a') : extension[i];

			if (extension == "bmp")
			{
				// BMP format
				if (stbi_write_bmp(filename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0]))
					return true;
			}
			else if (extension == "tga")
			{
				// TGA format
				if (stbi_write_tga(filename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0]))
					return true;
			}
			else if(extension == "png")
			{
				// PNG format
				if (stbi_write_png(filename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0],0))
					return true;
			}
			else if(extension == "jpg" || extension == "jpeg") // its a bit more messy
			{
				bool success = true;
				jpge::cfile_stream dst_stream;
				if (!dst_stream.open(filename.c_str()))
					fg_log << "Couldn't write " << filename << "." << std::endl,
					success = false;
				else
				{
					jpge::jpeg_encoder dst_image;
					if (!dst_image.init(&dst_stream,m_sizeW,m_sizeH,4,jpge::params()))
						success = false;
					else
					{
						for (fm::Size pass_index = 0; pass_index < dst_image.get_total_passes(); pass_index++)
						{
							C(m_sizeH)
							{
								const unsigned char *pBuf = (unsigned char *)(&m_pixels[0]) + (fm::Size)(i*m_sizeW*4);
								if (!dst_image.process_scanline(pBuf))
									success = false,
									i = m_sizeH+1,
									pass_index = dst_image.get_total_passes() + 1;
							}
							if (!dst_image.process_scanline(NULL))
									success = false,
									pass_index = dst_image.get_total_passes() + 1;
						}
						if (success)
						{
							dst_image.deinit();
							success = dst_stream.close();
						}
					}
				}
				if (success)
					return true;
			}
			else
			{
				std::string pngfilename=filename+".png";
				
				// Unknown extension so we output it as png
				fg_log << "Unknown extension \"" << extension <<"\" changed output name to "<<pngfilename<<std::endl;

				if (stbi_write_png(pngfilename.c_str(), m_sizeW, m_sizeH, 4, &m_pixels[0],0))
					return true;
			}
		}
		else
		{
			fg_log << filename << " couldn't be saved because it's empty." << std::endl;
			return false;
		}

		fg_log << "Failed to save image \"" << filename << "\"" << std::endl;
		return false;
	}
}
