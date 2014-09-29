//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#include <FRONTIER/Graphics/Image.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/Config.hpp>
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image.h"
#include "jpge/jpge.cpp"
#include <cstring>
namespace fg
{
	/// constructors /////////////////////////////////////////////////////////
	Image::Image() : m_size(0,0)
	{

	}


	////////////////////////////////////////////////////////////
	Image::Image(std::size_t width,std::size_t height,const Color &color)
	{
		create(width,height,color);
	}


	////////////////////////////////////////////////////////////
	Image::Image(const fm::vec2s &size,const Color &color)
	{
		create(size,color);
	}


	////////////////////////////////////////////////////////////
	Image::Image(const Image &copy,const fm::rect2u sourceRect)
	{
		create(sourceRect.area() ? sourceRect.size : fm::vec2s(copy.getSize())-sourceRect.pos);
		copyFrom(copy,0,0,sourceRect);
	}



	/// functions /////////////////////////////////////////////////////////
	typename Image::reference Image::create(std::size_t width,std::size_t height,const Color &color)
	{
		if (width && height)
		{
			m_size(width,height);
			m_pixels.resize(0),
			m_pixels.resize(width*height,color);
		}
		else
			m_size(0,0),
			m_pixels.clear();
		return *this;
	}


    ////////////////////////////////////////////////////////////
	typename Image::reference Image::create(const fm::vec2s &size,const Color &color)
	{
		return create(size.w,size.h,color);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::create(std::size_t width,std::size_t height,const Color *pixels)
	{
		if (width && height)
		{
			m_size(width,height);
			m_pixels = std::vector<Color>((Color *)pixels,((Color *)pixels)+width*height);
		}
		else
			m_size(0,0),
			m_pixels.clear();
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::create(const fm::vec2s &size,const Color *pixels)
	{
		return create(size.w,size.h,pixels);
	}


	////////////////////////////////////////////////////////////
    typename Image::reference Image::create(const Image &copy,const fm::rect2u sourceRect)
	{
		create(sourceRect.area() ? sourceRect.size : fm::vec2s(copy.getSize())-sourceRect.pos);
		copyFrom(copy,0,0,sourceRect);
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copy(const Image &source,std::size_t destX,std::size_t destY,const fm::rect2u &sourceRect,bool useAlpha)
	{
		if (!source.m_size.w || !source.m_size.h || !m_size.w || !m_size.h)
			return *this;

		fm::rect2u sRect = sourceRect;

		if (!sRect.size.w || !sRect.size.h)
		{
			sRect.pos.x  = 0;
			sRect.pos.y  = 0;
			sRect.size.w = source.m_size.w;
			sRect.size.h = source.m_size.h;
		}
		else
		{
			if (sRect.pos.x  < 0) sRect.pos.x = 0;
			if (sRect.pos.y  < 0) sRect.pos.y = 0;
			if (sRect.size.w > source.m_size.x) sRect.size.w = source.m_size.x;
			if (sRect.size.h > source.m_size.y) sRect.size.h = source.m_size.y;
		}

		std::size_t width  = sRect.size.w;
		std::size_t height = sRect.size.h;
		if (destX + width  > m_size.x) width  = m_size.x - destX;
		if (destY + height > m_size.y) height = m_size.y - destY;


		if (width<=0 || height<=0)
			return *this;


		if (useAlpha)
		{
			Cx(width)
			{
				Cy(height)
				{
					fm::vec4 src = source.getPixel(sourceRect.pos+fm::vec2s(x,y));
					fm::vec4 dst = getPixel(destX+x,destY+y);
					float alpha = src.a+dst.a*(1-src.a);
					setPixel(destX+x,destY+y,fm::vec4(fm::vec3(src*src.a+dst*dst.a*(1.f-src.a))/alpha,alpha));
				}
			}
		}
		else
		{
			C(height)
				std::memcpy(&getPixel(destX,destY+i), &source.getPixel(sourceRect.pos.x,sourceRect.pos.y+i), width*sizeof(Color));
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copy(const Image &source,const fm::vec2s &destPos,const fm::rect2u &sourceRect,bool useAlpha)
	{
		return this->copy(source,destPos.x,destPos.y,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,const fm::vec2s &destPos,const fm::rect2u &sourceRect,bool useAlpha) const
	{
		destination.copyFrom(*this,destPos,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	void Image::copyTo(Image &destination,std::size_t destX,std::size_t destY,const fm::rect2u &sourceRect,bool useAlpha) const
	{
		destination.copyFrom(*this,destX,destY,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copyFrom(const Image &source,const fm::vec2s &destPos,const fm::rect2u &sourceRect,bool useAlpha)
	{
		return this->copy(source,destPos,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::copyFrom(const Image &source,std::size_t destX,std::size_t destY,const fm::rect2u &sourceRect,bool useAlpha)
	{
		return this->copy(source,destX,destY,sourceRect,useAlpha);
	}


	////////////////////////////////////////////////////////////
	bool Image::loadFromFile(const std::string &filename)
	{
		m_pixels.clear();


		int width, height, channels;
		unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (ptr && width && height)
		{
			m_size(width,height);
			m_pixels.resize(width * height * 4);
			memcpy(&m_pixels[0], ptr, m_pixels.size());
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
	fm::vec2 Image::getSize() const
	{
		return m_size;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::setPixel(std::size_t x,std::size_t y,const Color &color)
	{
		m_pixels[x+y*m_size.w] = color;
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::setPixel(const fm::vec2s &pos,const Color &color)
	{
		return setPixel(pos.x,pos.y,color);
	}


	////////////////////////////////////////////////////////////
	const Color &Image::getPixel(std::size_t x,std::size_t y) const
	{
		return m_pixels[x+y*m_size.w];
	}


	////////////////////////////////////////////////////////////
	const Color &Image::getPixel(const fm::vec2s &pos) const
	{
		return getPixel(pos.x,pos.y);
	}


	////////////////////////////////////////////////////////////
	Color &Image::getPixel(std::size_t x,std::size_t y)
	{
		return m_pixels[x+y*m_size.w];
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
		Cx(m_size.w/2.f)
			Cy(m_size.h)
				std::swap(m_pixels[x+y*m_size.w],m_pixels[(m_size.w-x-1)+y*m_size.w]);
		return *this;
	}


	////////////////////////////////////////////////////////////
	typename Image::reference Image::flipVertically()
	{
		Cx(m_size.w)
			Cy(m_size.h/2.f)
				std::swap(m_pixels[x+y*m_size.w],m_pixels[x+(m_size.h-y-1)*m_size.w]);
		return *this;
	}


	////////////////////////////////////////////////////////////
	bool Image::saveToFile(const std::string &filename)
	{
		if (!m_pixels.empty() && (m_size.x > 0) && (m_size.y > 0))
		{
			int dotPos = filename.find_last_of(".");
			std::string extension = filename.substr(dotPos+1,filename.size()-dotPos);

			C(extension.length())
				extension[i] = (extension[i]>='A' && extension[i]<='Z') ? (extension[i]-'A'+'a') : extension[i];

			if (extension == "bmp")
			{
				// BMP format
				if (stbi_write_bmp(filename.c_str(), m_size.w, m_size.h, 4, &m_pixels[0]))
					return true;
			}
			else if (extension == "tga")
			{
				// TGA format
				if (stbi_write_tga(filename.c_str(), m_size.w, m_size.h, 4, &m_pixels[0]))
					return true;
			}
			else if(extension == "png")
			{
				// PNG format
				if (stbi_write_png(filename.c_str(), m_size.w, m_size.h, 4, &m_pixels[0],0))
					return true;
			}
			else if(extension == "jpg" || extension == "jpeg")
			{
				bool success = true;
				jpge::cfile_stream dst_stream;
				if (!dst_stream.open(filename.c_str()))
					fg_log << "Couldn't write " << filename << "." << std::endl,
					success = false;
				else
				{
					jpge::jpeg_encoder dst_image;
					if (!dst_image.init(&dst_stream,m_size.w,m_size.h,4,jpge::params()))
						success = false;
					else
					{
						for (std::size_t pass_index = 0; pass_index < dst_image.get_total_passes(); pass_index++)
						{
							C(m_size.h)
							{
								const unsigned char *pBuf = (unsigned char *)(&m_pixels[0]) + (std::size_t)(i*m_size.w*4);
								if (!dst_image.process_scanline(pBuf))
									success = false,
									i = m_size.h+1,
									pass_index = dst_image.get_total_passes() +1;
							}
							if (!dst_image.process_scanline(NULL))
									success = false,
									pass_index = dst_image.get_total_passes() +1;
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
				//Unknown extension so we output it as png
				fg_log << "Unknown extension \"" << extension <<"\" changed output name to "<<pngfilename<<std::endl;

				if (stbi_write_png(pngfilename.c_str(), m_size.w, m_size.h, 4, &m_pixels[0],0))
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
