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
#ifndef FRONTIER_IMAGE_HPP_INCLUDED
#define FRONTIER_IMAGE_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/Graphics/Color.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_IMAGE
#include <vector>

namespace fm
{
	template<class>
	class vector2;

	typedef vector2<fm::Size> vec2s;

	template<class>
    class rect;

    typedef rect<fm::Size> rect2s;
}

namespace std
{
    template<typename>
    class char_traits;
	template<typename>
    class allocator;
	template<typename,typename,typename>
	class basic_string;

	typedef basic_string<char,std::char_traits<char>,std::allocator<char> > string;
}

namespace fg
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Class used to hold the pixels of an image
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Image
	{
		std::vector<Color> m_pixels; ///< Hold all the pixels of the image
		fm::Size m_sizeW; ///< Holds the width  of the image
		fm::Size m_sizeH; ///< Holds the height of the image
	public:
		typedef Image &reference;
		typedef const Image &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// The initialized image has a size of (0,0) and doesn't have data
		///
		/////////////////////////////////////////////////////////////
		Image();

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image of given size and fill it with given color
		///
		/// @param width The width of the new image
		/// @param height The height of the new image
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		Image(fm::Size width,fm::Size height,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image of given size and fill it with given color
		///
		/// @param size The size of the new image
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		explicit Image(const fm::vec2s &size,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Copy a whole image
		///
		/// @param copy The source image
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image as a subimage of an existing one
		///
		/// If @a sourceRect has a size of (0,0) it is understood as to copy the
		/// part of the image that is higher and righter than the position of the rect
		///
		/// @param copy The source image
		/// @param sourceRect The rect to be copied
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy,const fm::rect2s &sourceRect);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and fill with given color
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param width The new width
		/// @param height The new height
		/// @param color The color to fill With
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(fm::Size width,fm::Size height,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and fill with given color
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param size The new size
		/// @param color The color to fill With
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(const fm::vec2s &size,const Color &color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and given data
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param width The new width
		/// @param height The new height
		/// @param pixels The data to copy to the image
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(fm::Size width,fm::Size height,const Color *pixels);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and given data
		///
		/// Please note that all changes on this image
		/// before this call will be discarded
		///
		/// @param size The new size
		/// @param pixels The data to copy to the image
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(const fm::vec2s &size,const Color *pixels);

		/////////////////////////////////////////////////////////////
		/// @brief Recreate an image as a copy of an existing one
		///
		/// @param copy The source image
		///
		/////////////////////////////////////////////////////////////
		reference create(const Image &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Recreate an image as a subimage of an existing one
		///
		/// @param copy The source image
		/// @param sourceRect The rect to be copied
		///
		/////////////////////////////////////////////////////////////
		reference create(const Image &copy,const fm::rect2s &sourceRect);

		/////////////////////////////////////////////////////////////
		/// @brief Copy from this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param destination The target image
		/// @param destPos The destination position
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/////////////////////////////////////////////////////////////
		void copyTo(Image &destination,const fm::vec2s &destPos,const fm::rect2s &sourceRect,bool useAlpha = false) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy from this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param destination The target image
		/// @param destX The x dimension of the destination position
		/// @param destY The y dimension of the destination position
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/////////////////////////////////////////////////////////////
		void copyTo(Image &destination,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha = false) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy an image to this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param source The source image
		/// @param destPos The destination position
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copyFrom(const Image &source,const fm::vec2s &destPos,const fm::rect2s &sourceRect,bool useAlpha = false);

		/////////////////////////////////////////////////////////////
		/// @brief Copy an image to this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param source The source image
		/// @param destX The x dimension of the destination position
		/// @param destY The y dimension of the destination position
		/// @param sourceRect The rectangle to be copied
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copyFrom(const Image &source,fm::Size destX,fm::Size destY,const fm::rect2s &sourceRect,bool useAlpha = false);

		/////////////////////////////////////////////////////////////
		/// @brief Copy this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param destination The target image
		/// @param destPos The destination position
		/// @param useAlpha If true then alpha-blending will be used
		///
		/////////////////////////////////////////////////////////////
		void copyTo(Image &destination,const fm::vec2s &destPos,bool useAlpha = false) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param destination The target image
		/// @param destX The x dimension of the destination position
		/// @param destY The y dimension of the destination position
		/// @param useAlpha If true then alpha-blending will be used
		///
		/////////////////////////////////////////////////////////////
		void copyTo(Image &destination,fm::Size destX,fm::Size destY,bool useAlpha = false) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy a whole image to this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param source The source image
		/// @param destPos The destination position
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copyFrom(const Image &source,const fm::vec2s &destPos,bool useAlpha = false);

		/////////////////////////////////////////////////////////////
		/// @brief Copy a whole image to this image
		///
		/// If @a useAplha is true then <a href="http://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending">alpha blending</a> will be processed
		///
		/// @param source The source image
		/// @param destX The x dimension of the destination position
		/// @param destY The y dimension of the destination position
		/// @param useAlpha If true then alpha-blending will be used
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference copyFrom(const Image &source,fm::Size destX,fm::Size destY,bool useAlpha = false);

		/////////////////////////////////////////////////////////////
		/// @brief Load image from a file
		///
		/// Supported types are jpg, jpeg, png, gif, tga
		/// it uses stb_image http://nothings.org
		///
		/// If anything goes wrong this function returns false and
		/// a description of the error is printed to fg_log
		///
		/// @param filename The name of the file to load
		///
		/// @return True if everything went ok
		///
		/////////////////////////////////////////////////////////////
		bool loadFromFile(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Load image from a file that is loaded into memory
		///
		/// Supported types are jpg, jpeg, png, gif, tga
		/// it uses stb_image http://nothings.org
		///
		/// If anything goes wrong this function returns false and
		/// a description of the error is printed to fg_log
		///
		/// @param buffer A pointer to the beginning of the file in memory
		/// @param byteCount The number of bytes in the file
		///
		/// @return True if everything went ok
		///
		/////////////////////////////////////////////////////////////
		bool loadFromMemory(const void *buffer,fm::Size byteCount);

		/////////////////////////////////////////////////////////////
		/// @brief Save image to a file
		///
		/// Supported types are jpg, jpeg, png, gif, tga
		/// it uses stb_image http://nothings.org
		///
		/// If anything goes wrong this function returns false and
		/// a description of the error is printed to fg_log
		///
		/// @param filename The name of the file to save to
		///
		/// @return True if everything went ok
		///
		/////////////////////////////////////////////////////////////
		bool saveToFile(const std::string &filename) const;

		/////////////////////////////////////////////////////////////
		/// @brief Save image to a memory region
		///
		/// Supported types are jpg, jpeg, png, gif, tga
		/// it uses stb_image http://nothings.org
		///
		/// If anything goes wrong this function returns NULL and
		/// a description of the error is printed to fg_log
		///
		/// @param byteCount The number of bytes returned (0 on error)
		/// @param ext The extension of the file
		///
		/// @return A pointer to the data (allocated with operator new)
		///
		/////////////////////////////////////////////////////////////
		unsigned char *saveToMemory(fm::Size &byteCount,const std::string &ext) const;

		/////////////////////////////////////////////////////////////
		/// @brief Resize The image
		///
		/// If anything goes wrong this function returns NULL and
		/// a description of the error is printed to fg_log
		///
		/// @param w The new width
		/// @param h The new height
		///
		/// @return The resized image
		///
		/////////////////////////////////////////////////////////////
		Image scale(fm::Size w,fm::Size h);

		/////////////////////////////////////////////////////////////
		/// @brief Get size of the image
		///
		/// @return The size of the image
		///
		/////////////////////////////////////////////////////////////
		const fm::vec2s &getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change a pixel of the image
		///
		/// No bound check is performed on x and y
		/// because the data of the image is linear in memory if x is begger than
		/// the width of the image the changed pixel will be at (x%width,y+int(x/width))
		///
		/// @param x The x coordinate of the pixel
		/// @param y The y coordinate of the pixel
		/// @param color the new color of the pixel
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setPixel(fm::Size x,fm::Size y,const Color &color);

		/////////////////////////////////////////////////////////////
		/// @brief Change a pixel of the image
		///
		/// No bound check is performed on x and y
		/// because the data of the image is linear in memory if x is begger than
		/// the width of the image the changed pixel will be at (x%width,y+int(x/width))
		///
		/// @param pos The position of the pixel
		/// @param color the new color of the pixel
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference setPixel(const fm::vec2s &pos,const Color &color);

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		///
		/// @param x The x coordinate of the pixel
		/// @param y The y coordinate of the pixel
		///
		/// @return The color of the pixel
		///
		/////////////////////////////////////////////////////////////
		const Color &getPixel(fm::Size x,fm::Size y) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		///
		/// @param x The x coordinate of the pixel
		/// @param y The y coordinate of the pixel
		///
		/// @return The color of the pixel
		///
		/////////////////////////////////////////////////////////////
		Color &getPixel(fm::Size x,fm::Size y);

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		///
		/// @param pos The position of the pixel
		///
		/// @return The color of the pixel
		///
		/////////////////////////////////////////////////////////////
		const Color &getPixel(const fm::vec2s &pos) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a pixel
		///
		/// @param pos The position of the pixel
		///
		/// @return The color of the pixel
		///
		/////////////////////////////////////////////////////////////
		Color &getPixel(const fm::vec2s &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Access the pixels in c-style
		///
		/// @return Pointer to the pixels
		///
		/////////////////////////////////////////////////////////////
		Color *getPixelsPtr();

		/////////////////////////////////////////////////////////////
		/// @brief Access the pixels in c-style
		///
		/// @return Pointer to the pixels
		///
		/////////////////////////////////////////////////////////////
		const Color *getPixelsPtr() const;

		/////////////////////////////////////////////////////////////
		/// @brief Flip the image horizontally
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference flipHorizontally();

		/////////////////////////////////////////////////////////////
		/// @brief Flip the image vertically
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference flipVertically();

		/////////////////////////////////////////////////////////////
		/// @brief Load images from a file that contains more than one
		///
		/// Supports: .ico
		///
		/// @param file The name of the file
		///
		/// @return A vector of images
		///
		/////////////////////////////////////////////////////////////
		static std::vector<Image> loadMultipleImagesFromFile(const std::string &file);

		/////////////////////////////////////////////////////////////
		/// @brief Load images from a file in memory that contains more than one
		///
		/// Supports: .ico
		///
		/// @param data The file in memory
		/// @param byteCount The number of bytes in the file
		///
		/// @return A vector of images
		///
		/////////////////////////////////////////////////////////////
		static std::vector<Image> loadMultipleImagesFromMemory(const fm::Uint8 *data,fm::Size byteCount,const std::string &ext);

		/////////////////////////////////////////////////////////////
		/// @brief Save image(s) to a file
		///
		/// Supports: .ico
		///
		/// @param file The name of the file
		/// @param images The images
		///
		/// @return A vector of images
		///
		/////////////////////////////////////////////////////////////
		static bool saveMultipleImagesToFile(const std::vector<Image> &images,const std::string &file);

		/////////////////////////////////////////////////////////////
		/// @brief Save image(s) to a file in memory
		///
		/// Supports: .ico
		///
		/// @param images The images
		/// @param byteCount The number of bytes written
		/// @param ext The extension
		///
		/// @return The file in memory
		///
		/////////////////////////////////////////////////////////////
		static const fm::Uint8 *saveMultipleImagesToMemory(const std::vector<Image> &images,fm::Size &byteCount,const std::string &ext);
	};
}
#endif

////////////////////////////////////////////////////////////
/// @class fg::Image
/// @ingroup Graphics
///
/// fg::Image is used to load, process and save images
/// although it doesn't have any advanced photo processing function
///
/// Usage example:
/// @code
///
/// fg::Image img;
/// img.create(500,500);
/// for (fm::Size x=0;x<img.getSize().w;x++)
/// 	for (fm::Size y=0;y<img.getSize().h;y++)
/// 		img.setPixel(x,y,(y%50<25 ? (x%50<25) : ((49-(x%50))<25)) ? fg::Color(80,80,80) : fg::Color(200,200,200));
/// img.saveToFile("chessBoard.png");
///
/// @endcode
///
/// @see fg::Color
///
////////////////////////////////////////////////////////////
