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
#include <FRONTIER/System/Result.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/util/API.h>
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

		/////////////////////////////////////////////////////////////
		/// @brief Load an image from a file
		///
		/// @param filename The name of the file
		///
		/////////////////////////////////////////////////////////////
		Image(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Copy a whole image
		/// 
		/// Operates in linear time to the number os texels
		/// 
		/// @param copy The source image
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Move an image
		/// 
		/// Operates in constant time
		/// 
		/// @param move The source image
		///
		/////////////////////////////////////////////////////////////
		Image(Image &&move);
		
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
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a part of the image
		/// 
		/// Copies the part of the image
		/// PArameters are clamped to the original image bounds if needed
		/// 
		/// @param pos The corner of the subimage
		/// @param size the size of the requested subimage
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		Image getSubImage(fm::vec2s pos,fm::vec2s size) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Load image from a file
		///
		/// Supported types are jpg, jpeg, png, gif, tga, bmp
		/// this function uses stb_image http://nothings.org
		/// 
		/// On faliure preserves original state
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
		/// Supported types are jpg, jpeg, png, gif, tga, bmp
		/// this function uses stb_image http://nothings.org
		/// 
		/// On faliure preserves original state
		///
		/// @param buffer A pointer to the beginning of the file in memory
		/// @param byteCount The number of bytes in the file
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromMemory(const void *buffer,fm::Size byteCount);

		/////////////////////////////////////////////////////////////
		/// @brief Save image to a file
		///
		/// @param filename The name of the file to save to
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result saveToFile(const std::string &filename) const;

		/////////////////////////////////////////////////////////////
		/// @brief Save image to a memory region
		///
		/// Supported types are jpg, jpeg, png, gif, tga, bmp
		/// this function uses stb_image http://nothings.org
		///
		/// @param pointer The pointer to the data (allocated using new[])
		/// @param byteCount The number of bytes returned (0 on error)
		/// @param ext The extension of the file
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result saveToMemory(fm::Uint8 *&pointer,fm::Size &byteCount,const std::string &ext = "png") const;

		/////////////////////////////////////////////////////////////
		/// @brief Resize The image
		/// 
		/// Resizing an empty image results in completely white image
		///
		/// @param size The new size
		/// @param linearFilter Iff true linear filtering is used (nearest neighbour otherwise)
		///
		/// @return The resized image
		///
		/////////////////////////////////////////////////////////////
		Image scale(fm::vec2s size,bool linearFilter = true);

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
		
		/////////////////////////////////////////////////////////////
		/// @brief Assign operator
		/// 
		/// @param img The image to overwrite this with
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(const Image &img);
		
		/////////////////////////////////////////////////////////////
		/// @brief Assign operator
		/// 
		/// @param img The image to swap this with
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(Image &&img);
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrive pointer to the beginning of a row of the image
		/// 
		/// @param index The index of the row
		/// 
		/// @return Pointer to the row
		///
		/////////////////////////////////////////////////////////////
		Color *operator[](fm::Size index);
		
		/////////////////////////////////////////////////////////////
		/// @brief Retrive pointer to the beginning of a row of the image
		/// 
		/// @param index The index of the row
		/// 
		/// @return Pointer to the row
		///
		/////////////////////////////////////////////////////////////
		const Color *operator[](fm::Size index) const;
	};
}
#endif