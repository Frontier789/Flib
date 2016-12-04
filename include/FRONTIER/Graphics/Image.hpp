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
#ifndef FRONTIER_IMAGE_HPP_INCLUDED
#define FRONTIER_IMAGE_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/Graphics/Color.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/util/API.h>
/*
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Error.hpp>
*/
#define FRONTIER_IMAGE
#include <string>

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold the texels of an image
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Image
	{
		Color *m_texels;  ///< Hold all the texels of the image
		fm::vec2s m_size; ///< Holds the size of the image
	public:
		typedef Image &reference;
		typedef const Image &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// The initialized image will hava a size of (0,0) and won't have any data
		///
		/////////////////////////////////////////////////////////////
		Image();

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~Image();

		/////////////////////////////////////////////////////////////
		/// @brief Construct an image of given size and fill it with given color
		///
		/// @param size The size of the new image
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		Image(fm::vec2s size,Color color = Color::White);
/*
		/////////////////////////////////////////////////////////////
		/// @brief Load an image from a file
		///
		/// @param filename The name of the file
		///
		/////////////////////////////////////////////////////////////
		Image(const std::string &filename);
*/
		/////////////////////////////////////////////////////////////
		/// @brief Copy a whole image
		///
		/// @param copy The source image
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Move a whole image
		///
		/// @param move The source image
		///
		/////////////////////////////////////////////////////////////
		Image(Image &&move);
/*
		/////////////////////////////////////////////////////////////
		/// @brief Construct an image as a subimage of an existing one
		///
		/// If @a sourceRect has a size of (0,0) it is interpreted as
		/// copy.getSize() - sourceRect.pos
		///
		/// @param copy The source image
		/// @param sourceRect The rect to be copied
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy,const fm::rect2s &sourceRect);
*/
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
		reference create(fm::vec2s size,Color color = Color::White);

		/////////////////////////////////////////////////////////////
		/// @brief Create the image with given size and given data
		/// 
		/// @param size The new size
		/// @param texels The data to copy to the image
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(fm::vec2s size,const Color *texels);
/*
		/////////////////////////////////////////////////////////////
		/// @brief Recreate an image as a subimage of an existing one
		///
		/// @param copy The source image
		/// @param sourceRect The rect to be copied
		///
		/// @return reference to itself
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
		void copyTo(Image &destination,fm::vec2s destPos,const fm::rect2s &sourceRect,bool useAlpha = false) const;

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
		reference copyFrom(const Image &source,fm::vec2s destPos,const fm::rect2s &sourceRect,bool useAlpha = false);

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
		void copyTo(Image &destination,fm::vec2s destPos,bool useAlpha = false) const;

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
		reference copyFrom(const Image &source,fm::vec2s destPos,bool useAlpha = false);

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
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromFile(const std::string &filename);

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
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromMemory(const void *buffer,fm::Size byteCount);
*/
		/////////////////////////////////////////////////////////////
		/// @brief Save image to a file
		///
		/// @param filename The name of the file to save to
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		void saveToFile(const std::string &filename) const;
/*
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
		/// @return A pointer to the data (allocated with operator new[])
		///
		/////////////////////////////////////////////////////////////
		fm::Uint8 *saveToMemory(fm::Size &byteCount,const std::string &ext) const;

		/////////////////////////////////////////////////////////////
		/// @brief Resize The image
		///
		/// If anything goes wrong this function returns NULL and
		/// a description of the error is printed to fg_log
		///
		/// @param w The new width
		/// @param h The new height
		/// @param linearFilter Iff true linear filtering is used (nearest neighbour otherwise)
		///
		/// @return The resized image
		///
		/////////////////////////////////////////////////////////////
		Image scale(fm::Size w,fm::Size h,bool linearFilter = true);

		/////////////////////////////////////////////////////////////
		/// @brief Resize The image
		///
		/// If anything goes wrong this function returns NULL and
		/// a description of the error is printed to fg_log
		///
		/// @param size The new size
		/// @param linearFilter Iff true linear filtering is used (nearest neighbour otherwise)
		///
		/// @return The resized image
		///
		/////////////////////////////////////////////////////////////
		Image scale(fm::vec2s size,bool linearFilter = true);
*/
		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of two images
		///
		/// swaps in constant time
		///
		/// @param img The image to swap with
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference swap(Image &img);

		/////////////////////////////////////////////////////////////
		/// @brief Get size of the image
		///
		/// @return The size of the image
		///
		/////////////////////////////////////////////////////////////
		fm::vec2s size() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a texel
		///
		/// @param pos The position of a texel
		///
		/// @return The color of the texel
		///
		/////////////////////////////////////////////////////////////
		Color &at(fm::vec2s pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a texel
		///
		/// @param pos The position of a texel
		///
		/// @return The color of the texel
		///
		/////////////////////////////////////////////////////////////
		const Color &at(fm::vec2s pos) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a texel
		///
		/// @param pos The position of a texel
		///
		/// @return The color of the texel
		///
		/////////////////////////////////////////////////////////////
		reference set(fm::vec2s pos,Color color);

		/////////////////////////////////////////////////////////////
		/// @brief Access the texels in c-style
		///
		/// @return Pointer to the texels
		///
		/////////////////////////////////////////////////////////////
		Color *getPtr();

		/////////////////////////////////////////////////////////////
		/// @brief Access the texels in c-style
		///
		/// @return Pointer to the texels
		///
		/////////////////////////////////////////////////////////////
		const Color *getPtr() const;

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
/*
		/////////////////////////////////////////////////////////////
		/// @brief Load images from a file that contains more than one
		///
		/// Supports: .ico
		///
		/// @param file The name of the file
		/// @param error Returns the error (if any) that happened
		///
		/// @return A vector of images
		///
		/////////////////////////////////////////////////////////////
		static std::vector<Image> loadMultipleImagesFromFile(const std::string &file,fm::Error *error = fm::nullPtr);

		/////////////////////////////////////////////////////////////
		/// @brief Load images from a file in memory that contains more than one
		///
		/// Supports: .ico
		///
		/// @param data The file in memory
		/// @param byteCount The number of bytes in the file
		/// @param ext The extension (file type) of the file in memory
		/// @param error Returns the error (if any) that happened
		///
		/// @return A vector of images
		///
		/////////////////////////////////////////////////////////////
		static std::vector<Image> loadMultipleImagesFromMemory(const fm::Uint8 *data,fm::Size byteCount,const std::string &ext,fm::Error *error = fm::nullPtr);

		/////////////////////////////////////////////////////////////
		/// @brief Save image(s) to a file
		///
		/// Supports: .ico
		///
		/// @param images The images
		/// @param imageCount The number of images
		/// @param file The name of the file
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result saveMultipleImagesToFile(Image const* const* images,fm::Size imageCount,const std::string &file);

		/////////////////////////////////////////////////////////////
		/// @brief Save image(s) to a file in memory
		///
		/// Supports: .ico
		/// Returns allocated memory in @a memory
		/// Uses operator[] new for allocation
		///
		/// @param images The images
		/// @param imageCount The number of images
		/// @param memory The written memory
		/// @param ext The extension
		///
		/// @return The number of bytes written
		///
		/////////////////////////////////////////////////////////////
		static fm::Size saveMultipleImagesToMemory(Image const* const* images,fm::Size imageCount,fm::Uint8 *(&memory),const std::string &ext);
	*/
	};
}
#endif