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
#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <fstream>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "stb_image/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"
/*
#include "ico/ico.hpp"
*/
#include "jpge/jpge.cpp"


namespace fg
{
	/////////////////////////////////////////////////////////////
	Image::Image() : m_texels(nullptr)
	{
		
	}

	/////////////////////////////////////////////////////////////
	Image::~Image()
	{
		delete m_texels;
	}

	/////////////////////////////////////////////////////////////
	Image::Image(fm::vec2s size,Color color) : m_texels(nullptr)
	{
		create(size,color);
	}
	
	/////////////////////////////////////////////////////////////
	Image::Image(const std::string &filename)
	{
		loadFromFile(filename);
	}
	
#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Image::Image(const Image &copy) : m_texels(nullptr)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		(*this) = copy;
	}
#endif

	/////////////////////////////////////////////////////////////
	Image::Image(Image &&move) : m_texels(nullptr)
	{
		move.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	Image::reference Image::create(fm::vec2s size,Color color)
	{
		if (size != m_size)
		{
			m_size = size;
			
			delete m_texels;
			m_texels = new Color[m_size.area()];
		}
		
		Cv(m_size)
			m_texels[p.x * m_size.h + p.y] = color;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Image::reference Image::create(const fg::Image &img,fm::vec2s pos,fm::vec2s size)
	{
		img.getSubImage(pos,size).swap(*this);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Image::reference Image::create(fm::vec2s size,const Color *texels)
	{
		if (size != m_size)
		{
			m_size = size;
			
			delete m_texels;
			m_texels = new Color[m_size.area()];
		}
		
		memcpy(m_texels,texels,m_size.area() * sizeof(*texels));
		
		return *this;
	}


	std::string getExtension(const std::string &text)
	{
		int dotPos = text.find_last_of(".");
		std::string ret = text.substr(dotPos+1,text.length()-dotPos);

		// lowercase
		C(ret.length())
			ret[i] = (ret[i]>='A' && ret[i]<='Z') ? (ret[i]-'A'+'a') : ret[i];

		return ret;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Image::saveToFile(const std::string &filename) const
	{
		// if the image is valid
		if (m_size.area())
		{
			// get extension
			std::string extension = getExtension(filename);

			int error = 0;

			if (extension == "bmp")
			{
				// BMP format
				if (!stbi_write_bmp(filename.c_str(), m_size.w, m_size.h, 4, m_texels))
					error = 1;
			}
			else if (extension == "tga")
			{
				// TGA format
				if (!stbi_write_tga(filename.c_str(), m_size.w, m_size.h, 4, m_texels))
					error = 1;
			}
			else if (extension == "jpg" || extension == "jpeg") // its a bit more messy
			{
				if (!jpge::compress_image_to_jpeg_file(filename.c_str(), m_size.w, m_size.h, 4, (unsigned char *)m_texels))
					error = 2;
			}
			else
			{
				std::string pngfilename = filename;

				if (extension != "png") // unkown extension
					pngfilename += ".png";

				// PNG format
				if (!stbi_write_png(pngfilename.c_str(), m_size.w, m_size.h, 4, m_texels,0))
					error = 1;
			}
			
			if (error == 0)
				return fm::Result();

			if (error == 1)
				return fm::Result("STBIError",fm::Result::OPFailed,"stbi_failure","saveToFile",__FILE__,__LINE__,filename,std::string(stbi_failure_reason()));

			if (error == 2)
				return fm::Result("IOError",fm::Result::OPFailed,"FileNotWritable","saveToFile",__FILE__,__LINE__,filename);
		}

		return fm::Result();
	}
	

	/////////////////////////////////////////////////////////////
	fm::Result Image::saveToMemory(fm::Uint8 *&pointer,fm::Size &byteCount,const std::string &ext) const
	{
		// if the image is valid
		if (m_size.area())
		{
			unsigned char *(*writer)(const unsigned char*,int,int,int,int*) = 0;
			
			// get extension
			std::string extension = getExtension(ext);
			
			if (extension == "bmp") writer = &stbi_write_bmp_mem;
			else if (extension == "tga") writer = &stbi_write_tga_mem;
			else if (extension == "jpg" || extension == "jpeg") writer = &jpge::jpeg_write_mem;
			else
			{
				writer = &stbi_write_png_to_mem_auto;
			}
			
			int bytesWritten = 0;
			pointer = writer((const unsigned char*)m_texels,m_size.w,m_size.h,4,&bytesWritten);
			byteCount = bytesWritten;
		}
		
		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	Image Image::getSubImage(fm::vec2s pos,fm::vec2s size) const
	{
		Image ret;
		
		if (pos.x >= size.w || pos.y >= getSize().h)
			return ret;
		
		size.w = std::min(size.w,getSize().w - pos.x);
		size.h = std::min(size.h,getSize().h - pos.y);
		
		ret.create(size);
		
		if (size.w == getSize().w)
		{
			std::memcpy(ret.getTexelsPtr(),getTexelsPtr() + getSize().w * pos.y,sizeof(Color)*getSize().area());
		}
		else
		{
			C(size.h)
			{
				std::memcpy(ret.getTexelsPtr() + size.w * i,getTexelsPtr() + getSize().w * (pos.y + i) + pos.x,sizeof(Color)*size.w);
			}
		}
		
		return ret;
	}

	/////////////////////////////////////////////////////////////
	Image Image::scale(fm::vec2s size,bool linearFilter)
	{
		Image ret;
		ret.create(size);

		if (linearFilter)
		{
			// use stb's image resize
			if (!stbir_resize_uint8((unsigned char*)m_texels,m_size.w,m_size.h,0,
									(unsigned char*)ret.m_texels,size.w,size.h,0,
									4))
				return Image();
		}
		else
		{
			// simple nearest neighbour interpolation
			fm::vec2 ratio = fm::vec2(m_size)/size;

			Cv(size)
				ret.set(p,at(p*ratio));
			
		}


		return ret;
	}

	////////////////////////////////////////////////////////////
	fm::Result Image::loadFromFile(const std::string &filename)
	{
		// ask stbi to load the file
		int width, height, channels;
		unsigned char *ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		// if it succeeded
		if (ptr && width && height)
		{
			// empty current data
			delete[] m_texels;
			
			// copy the data
			m_size = fm::vec2s(width,height);
			m_texels = new Color[width*height];
			
			std::memcpy(m_texels, ptr, m_size.area()*sizeof(*m_texels));

			// let stbi free its used memory
			stbi_image_free(ptr);

			return fm::Result();
		}
		return fm::Result("IOError",fm::Result::OPFailed,"FileNotFound","loadFromFile",__FILE__,__LINE__,filename);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Image::loadFromMemory(const void *buffer,fm::Size byteCount)
	{
		// ask stbi to load the file
		int width, height, channels;
		unsigned char *ptr = stbi_load_from_memory((const unsigned char*)buffer, byteCount, &width, &height, &channels, STBI_rgb_alpha);

		// if he succeeded
		if (ptr && width && height)
		{
			// empty current data
			delete[] m_texels;
			
			// copy the data
			m_size = fm::vec2s(width,height);
			m_texels = new Color[width*height];
			
			std::memcpy(m_texels, ptr, m_size.area()*sizeof(*m_texels));

			// let stbi free its used memory
			stbi_image_free(ptr);

			return fm::Result();
		}
		return fm::Result("STBIError",fm::Result::OPFailed,"stbi_failure","loadFromMemory",__FILE__,__LINE__,std::string(stbi_failure_reason()));
	}
	
	/////////////////////////////////////////////////////////////
	Image::reference Image::swap(Image &img)
	{
		std::swap(m_texels,img.m_texels);
		std::swap(m_size,img.m_size);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2s Image::getSize() const
	{
		return m_size;
	}

	/////////////////////////////////////////////////////////////
	Color &Image::at(fm::vec2s pos)
	{
		return getTexel(pos);
	}

	/////////////////////////////////////////////////////////////
	const Color &Image::at(fm::vec2s pos) const
	{
		return getTexel(pos);
	}

	/////////////////////////////////////////////////////////////
	Color &Image::getTexel(fm::vec2s pos)
	{
		return m_texels[pos.y * m_size.w + pos.x];
	}

	/////////////////////////////////////////////////////////////
	const Color &Image::getTexel(fm::vec2s pos) const
	{
		return m_texels[pos.y * m_size.w + pos.x];
	}

	/////////////////////////////////////////////////////////////
	Image::reference Image::set(fm::vec2s pos,Color color)
	{
		getTexel(pos) = color;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Image::reference Image::setTexel(fm::vec2s pos,Color color)
	{
		getTexel(pos) = color;
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Color *Image::getPtr()
	{
		return m_texels;
	}

	/////////////////////////////////////////////////////////////
	const Color *Image::getPtr() const
	{
		return m_texels;
	}

	/////////////////////////////////////////////////////////////
	Color *Image::getTexelsPtr()
	{
		return m_texels;
	}

	/////////////////////////////////////////////////////////////
	const Color *Image::getTexelsPtr() const
	{
		return m_texels;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::flipHorizontally()
	{
		Cx(m_size.w/2.f)
			Cy(m_size.h)
				std::swap(at(fm::vec2s(x,y)),at(fm::vec2s(m_size.w-1-x,y)));

		return *this;
	}


	////////////////////////////////////////////////////////////
	Image::reference Image::flipVertically()
	{
		Cx(m_size.w)
			Cy(m_size.h/2.f)
				std::swap(at(fm::vec2s(x,y)),at(fm::vec2s(x,m_size.h-1-y)));

		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Image::reference Image::operator=(Image &&move)
	{
		move.swap(*this);
		
		return (*this);
	}
	
#ifndef FRONTIER_HEAVYCOPY_FORBID
	/////////////////////////////////////////////////////////////
	Image::reference Image::operator=(const Image &copy)
	{
		FRONTIER_HEAVYCOPY_NOTE;
		
		delete[] m_texels;
		m_size = copy.m_size;
		
		m_texels = new Color[m_size.area()];
		
		std::memcpy(m_texels,copy.m_texels,m_size.area() * sizeof(*m_texels));
		
		return (*this);
	}
#endif
}
