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

#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Rect.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/Result.hpp>
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
		/// @param copy The source image
		///
		/////////////////////////////////////////////////////////////
		Image(const Image &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move a whole image
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
		/// @brief Create the image from an other image
		/// 
		/// @param img The image t copy from
		/// @param pos The position of the rectangle to copy from the other image
		/// @param size The size of the rectangle to copy from the other image
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference create(const fg::Image &img,fm::vec2s pos,fm::vec2s size);

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
		/// @brief Load the image from a file
		/// 
		/// @param filename The file to load
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromFile(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Load the image from a file loaded int memory
		/// 
		/// @param buffer The buffer holding the data
		/// @param byteCount The number of bytes the data consists of
		///
		/// @return reference to itself
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
		/// Supported types are jpg, jpeg, png, gif, tga
		/// it uses stb_image http://nothings.org
		///
		/// If anything goes wrong this function returns NULL
		///
		/// @param byteCount The number of bytes returned (0 on error)
		/// @param ext The extension of the file
		/// @param pointer A pointer to the data (allocated with operator new[])
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result saveToMemory(fm::Uint8 *&pointer,fm::Size &byteCount,const std::string &ext) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy a region of the image
		///
		/// @param pos The upper-left corner of the region
		/// @param size The size of the region in texels
		///
		/// @return The copied image
		///
		/////////////////////////////////////////////////////////////
		Image getSubImage(fm::vec2s pos,fm::vec2s size) const;

		/////////////////////////////////////////////////////////////
		/// @brief Resize The image
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
		fm::vec2s getSize() const;
		
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
		Color &getTexel(fm::vec2s pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the color of a texel
		///
		/// @param pos The position of a texel
		///
		/// @return The color of the texel
		///
		/////////////////////////////////////////////////////////////
		const Color &getTexel(fm::vec2s pos) const;
		
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
		/// @brief Get the color of a texel
		///
		/// @param pos The position of a texel
		///
		/// @return The color of the texel
		///
		/////////////////////////////////////////////////////////////
		reference setTexel(fm::vec2s pos,Color color);

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
		/// @brief Access the texels in c-style
		///
		/// @return Pointer to the texels
		///
		/////////////////////////////////////////////////////////////
		Color *getTexelsPtr();

		/////////////////////////////////////////////////////////////
		/// @brief Access the texels in c-style
		///
		/// @return Pointer to the texels
		///
		/////////////////////////////////////////////////////////////
		const Color *getTexelsPtr() const;

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
		/// @brief Copy an image
		///
		/// @param copy The image to copy
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(const Image &copy) FRONTIER_HEAVYCOPY_QUALIFIER;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move an image
		///
		/// @param move The image to move
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(Image &&move);
		
	};
}
#endif