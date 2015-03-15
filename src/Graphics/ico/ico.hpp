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
#ifndef FRONTIER_ICO_HPP_INCLUDED
#define FRONTIER_ICO_HPP_INCLUDED

#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Image.hpp>

namespace Ico
{
	class Entry
	{
	public:
		fm::Uint8  width;        // Width of the image
		fm::Uint8  height;       // Height of the image (times 2)
		fm::Uint8  colorCount;   // Number of colors in image (0 if >= 8*bpp)
		fm::Uint8  reserved;     // Reserved
		fm::Uint16 planes;       // Color Planes
		fm::Uint16 bpp;          // Bits per pixel
		fm::Uint32 byteCount;    // how many bytes in this resource?
		fm::Uint32 offsetInFile; // where in the file is this image
	};

	class IconHeader
	{
	public:
		fm::Uint16 reserved; // Reserved
		fm::Uint16 type;     // resource type (1 for icons)
		fm::Uint16 count;    // number of images
	};


	class BitmapHeader
	{
	public:
		fm::Uint32 size;
		fm::Uint32 width;
		fm::Uint32 height;
		fm::Uint16 planes;
		fm::Uint16 bitCount;
		fm::Uint32 compression;
		fm::Uint32 sizeImage;
		fm::Uint32 xPelsPerMeter;
		fm::Uint32 yPelsPerMeter;
		fm::Uint32 clrUsed;
		fm::Uint32 clrImportant;
	};

	template<class T>
	void readBuffer(T &target,const fm::Uint8 *&ptr)
	{
		target = *((T*)ptr);
		ptr+=sizeof(T)/sizeof(fm::Uint8);
	}
	
	void writeImages(std::ostream &out,fg::Image const* const* imgs,fm::Size imgCount)
	{
		// write the icon header
		IconHeader iconHead;
		iconHead.reserved = 0;
		iconHead.type     = 1;
		iconHead.count    = imgCount;
		out.write((char*)&iconHead,sizeof(iconHead));
		
		// calculate the used storage by the header and the entries
		fm::Size offsetInFile = sizeof(iconHead)+sizeof(Entry)*iconHead.count;
		
		// store the png files in memory until writing them
		std::vector<unsigned char*> pngSourcesP;
		std::vector<fm::Size> pngSourcesS;
		fm::Size pngCounter=0;
		
		for (fm::Size i=0;i<iconHead.count;i++)
		{
			// write as png if its too big for bmp
			bool png = false;
			if (imgs[i]->getSize().w>=256 || imgs[i]->getSize().h>=256)
			{
				png = true;
				
				// create space for the data
				pngSourcesP.push_back(NULL);
				pngSourcesS.push_back(0);
				
				// write png source
				pngSourcesP[pngCounter] = imgs[i]->saveToMemory(pngSourcesS[pngCounter],"png");
				pngCounter++;
			}
			
			// fill out entry
			Entry currentEntry;
			currentEntry.width  = imgs[i]->getSize().w;
			currentEntry.height = imgs[i]->getSize().h;
			currentEntry.colorCount = 0;
			currentEntry.reserved = 0;
			currentEntry.planes = 1;
			currentEntry.bpp = 32;
			
			// if png then the size of the png otherwise (bmp) 32bpp for w*h +40 (header)
			currentEntry.byteCount = png ? pngSourcesS[pngCounter-1] : sizeof(fg::Color)*imgs[i]->getSize().w*imgs[i]->getSize().h+40;
			currentEntry.offsetInFile = offsetInFile;
			
			// write the entry
			out.write((char*)&currentEntry,sizeof(currentEntry));
			
			// increment offset
			offsetInFile += currentEntry.byteCount;
		}
		
		// write bmps and pngs
		pngCounter = 0;
		for (fm::Size i=0;i<iconHead.count;i++)
		{
			// write as png if its too big for bmp
			if (imgs[i]->getSize().w>=256 || imgs[i]->getSize().h>=256)
			{
				// write what the allocated pngs
				out.write((char*)pngSourcesP[pngCounter],pngSourcesS[pngCounter]);
				
				// and deallocate it
				delete[] pngSourcesP[pngCounter];
				pngCounter++;
				continue;
			}
			
			// otherwise write as bmp
			BitmapHeader bmpH;
			
			// fill bmp header
			bmpH.size = 40;
			bmpH.width = imgs[i]->getSize().w;
			bmpH.height = imgs[i]->getSize().h*2;
			bmpH.planes = 1;
			bmpH.bitCount = 32;
			bmpH.compression = 0;
			bmpH.sizeImage = 0;
			bmpH.xPelsPerMeter = 0;
			bmpH.yPelsPerMeter = 0;
			bmpH.clrUsed = 0;
			bmpH.clrImportant = 0;
			
			// write bmp header
			out.write((char*)&bmpH,sizeof(bmpH));
			
			// write raw color data
			for (fm::Size y=0;y<imgs[i]->getSize().h;y++)
				for (fm::Size x=0;x<imgs[i]->getSize().w;x++)
				{
					fg::Color c = imgs[i]->getPixel(x,imgs[i]->getSize().h-y-1);
					std::swap(c.r,c.b);
					out.write((char*)&c,sizeof(c));
				}
		}
	}
	
	
	void extractImage(const fm::Uint8 *buffer,Entry &entry,fm::Size &realBitsCount,fg::Image &ret);
	
	std::vector<fg::Image> getImages(const fm::Uint8 *buffer,fm::Size byteCount,std::string &error)
	{
		(void)byteCount;
		
		const fm::Uint8 *bufPtr = buffer;

		std::vector<fg::Image> ret;

		IconHeader iconHead;
		readBuffer<IconHeader>(iconHead,bufPtr);

		// only process correctly encoded files
		if (iconHead.reserved != 0)
		{
			error = "Incorrectly encoded ico";
			return ret;
		}
		if (iconHead.type != 1)
		{
			error = "File is not icon";
			return ret;
		}
		if (iconHead.count == 0)
		{
			error = "File contains 0 icons";
			return ret;
		}

		ret.resize(iconHead.count);
		
		fm::Size realBitsCount = 8;

		for (fm::Size i=0;i<iconHead.count;i++)
		{
			Entry currentEntry;
			readBuffer<Entry>(currentEntry,bufPtr);

			extractImage(buffer,currentEntry,realBitsCount,ret[i]);
		}

		return ret;
	}
	
	fg::Color readRGB(const void *ptr)
	{
		fg::Color ret;
		ret.r = *(((const fm::Uint8*)ptr)+0);
		ret.g = *(((const fm::Uint8*)ptr)+1);
		ret.b = *(((const fm::Uint8*)ptr)+2);
		return ret;
	}

	void extractImage(const fm::Uint8 *buffer,Entry &entry,fm::Size &realBitsCount,fg::Image &ret)
	{/**
		fg::fg_log << "found icon with " << std::endl;
		fg::fg_log << "width = " << (int)entry.width << std::endl;
		fg::fg_log << "height = " << (int)entry.height  << std::endl;
		fg::fg_log << "colorCount = " << (int)entry.colorCount << std::endl;
		fg::fg_log << "reserved   = " << (int)entry.reserved   << std::endl;
		fg::fg_log << "planes     = " << (int)entry.planes     << std::endl;
		fg::fg_log << "bpp        = " << (int)entry.bpp        << std::endl;
		fg::fg_log << "byteCount  = " << (int)entry.byteCount  << std::endl;
		fg::fg_log << "offsetInFile = " << (int)entry.offsetInFile  << std::endl;*/
		
		const fm::Uint8 *begin = buffer+entry.offsetInFile;
		
		// find out if its a png
		if (*(begin+0) == 0x89 && 
			*(begin+1) == 0x50 && 
			*(begin+2) == 0x4E && 
			*(begin+3) == 0x47 && 
			*(begin+4) == 0x0D && 
			*(begin+5) == 0x0A && 
			*(begin+6) == 0x1A && 
			*(begin+7) == 0x0A)
		{
			ret.loadFromMemory(begin,entry.byteCount);
			return;
		}
		
		
		BitmapHeader bmpHeader = *((BitmapHeader*)begin);
		
		fm::Uint16 width = /*bmpHeader.width*/entry.width ? entry.width : 256;
		fm::Uint16 height = /*bmpHeader.height*/entry.height ? entry.height : 256;
		/**
		fg::fg_log << "w1 = " << width  << std::endl;
		fg::fg_log << "h1 = " << height << std::endl;
*/
		ret.create(width,height);

		realBitsCount = (bmpHeader.bitCount ? bmpHeader.bitCount : realBitsCount);
		/**
		fg::fg_log << "bitCount = " << realBitsCount << std::endl;
*/
		const fm::Uint8 *cursor = buffer+entry.offsetInFile+40;

		// rgba + vertical swap
		if (realBitsCount >= 32)
		{
			Cxy(width,height)
			{
				fm::Size index = 4*(x + (height-y-1) * width);
				fg::Color c = *(fg::Color*)(cursor+index);
				std::swap(c.r,c.b); /// BGRA -> RGBA
				ret.setPixel(x,y,c);
			}
		}

		if (realBitsCount == 24)
		{
			Cxy(width,height)
			{
				fm::Size index = 4*(x + (height-y-1) * width);
				fg::Color c = readRGB(cursor+index);
				std::swap(c.r,c.b); /// BGR -> RGB
				c.a = 255;
				ret.setPixel(x,y,c);
			}
		}

		if (realBitsCount == 8)  /// 256 colors
		{
			// 256 color table
			const fm::Uint8 *colors = (const fm::Uint8 *)cursor;
			cursor += 256 * 4;

			Cxy(width,height)
			{
				fm::Size index = 4*cursor[(x + (height - y - 1) * width)];
				fg::Color c = readRGB(colors+index);
				std::swap(c.r,c.b); /// BGR -> RGB
				c.a = 255;
				ret.setPixel(x,y,c);
			}
		}

		if (realBitsCount == 4)  /// 16 colors
		{
			// 16 color table
			const fm::Uint8 *colors = (const fm::Uint8 *)cursor;
			cursor += 16 * 4;

			Cxy(width,height)
			{
				fm::Size shift = (x + (height - y - 1) * width);
				fm::Size index = cursor[shift / 2];
				if (shift % 2 == 0)
					index = (index >> 4) & 0xF;
				else
					index = index & 0xF;
				index *= 4;

				fg::Color c = readRGB(colors+index);
				std::swap(c.r,c.b); /// BGR -> RGB
				c.a = 255;
				ret.setPixel(x,y,c);
			}
		}

		if (realBitsCount == 1)  /// 2 colors
		{
			// 2 color table
			const fm::Uint8 *colors = (const fm::Uint8 *)cursor;
			cursor += 2 * 4;

			/** clamp to 32 bit boundary, see (http://www.daubnet.com/en/file-format-ico **/
			fm::Size bound = width;
			while (bound % 32 != 0) bound++;

			Cxy(width,height)
			{
				fm::Size shift = (x + (height - y - 1) * bound);
				fm::Size index = cursor[shift / 8];

				index = (index >> (7 - (x % 8))) & 0x01;
				index *= 4;

				fg::Color c = readRGB(colors+index);
				std::swap(c.r,c.b); /// BGR -> RGB
				c.a = 255;
				ret.setPixel(x,y,c);
			}
		}

		// Process AND mask (1 BIT MASK)
		if ((realBitsCount < 32) && (height != bmpHeader.height))
		{
			fm::Size  shift;
			fm::Uint8 bit;
			fm::Size  mask;

			/** clamp to 32 bit boundary, see (http://www.daubnet.com/en/file-format-ico **/
			fm::Size bound = width * realBitsCount;
			while (bound % 32 != 0) bound++;
			cursor += bound * height / 8;

			bound = width;
			while (bound % 32 != 0) bound++;

			Cxy(width,height)
			{
				bit = 7 - (x % 8);
				shift = (x + (height - y - 1) * bound) / 8;
				mask = (0x01 & ((fm::Uint8)cursor[shift] >> bit));

				fg::Color &c = ret.getPixel(x,y);
				c.a *= 1 - mask;
			}
		}
	}
}

#endif // FRONTIER_ICO_HPP_INCLUDED
