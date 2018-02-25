////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_FLOATTEXTURE_HPP_INCLUDED
#define FRONTIER_FLOATTEXTURE_HPP_INCLUDED
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Rect.hpp>
#define FRONTIER_FLOATTEXTURE

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief A special texture that stores the data in floating point representation
	///
	/// @ingroup Graphics
	/// @see fg::Texture
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API FloatTexture : public Texture
	{
	public:
		fm::Int32 getInternalFormat() const; ///< Internal format of the texture
	
	public:
		typedef FloatTexture &reference;
		typedef const FloatTexture &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Leaves the texture uninitialized thus invalid
		///
		/////////////////////////////////////////////////////////////
		FloatTexture();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// Copies the whole texture back to client memory
		/// and sends the data to a new texture unit
		/// it is a S-L-O-W operation
		///
		/// If @a copy is invalid then the texture is leaved invalid
		///
		/// @param copy The texture to be copied
		///
		/////////////////////////////////////////////////////////////
		FloatTexture(const FloatTexture &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param move The texture to move
		///
		/////////////////////////////////////////////////////////////
		FloatTexture(FloatTexture &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		///
		/// Copies the whole texture back to client memory
		/// and sends the data to a new texture unit
		/// it is a S-L-O-W operation
		///
		/// If @a copy is invalid then the texture is leaved invalid
		///
		/// @param copy The texture to be copied
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		FloatTexture &operator=(const FloatTexture &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		///
		/// @param move The texture to move
		///
		/////////////////////////////////////////////////////////////
		FloatTexture &operator=(FloatTexture &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Create a OpenGL texture from image
		///
		/// This function sends the client-side data (the image)
		/// to OpenGL
		///
		/// @param img The image to send to OpenGL
		///
		/////////////////////////////////////////////////////////////
		explicit FloatTexture(const Image &img);

		/////////////////////////////////////////////////////////////
		/// @brief Load the data back to cpu from the texture
		///
		/// This function retrieves the server-side data (the texture)
		/// from OpenGL
		///
		/// @param ptr Pointer to the array where the data is to be stored
		/// @param region The subrectangle to retrieve data from (0 size means maximal size)
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result copyToArray(float *ptr,fm::rect2s region = fm::rect2s());

		/////////////////////////////////////////////////////////////
		/// @brief Create a floating point OpenGL texture from cpu memory data
		///
		/// This function sends the client-side data (the array)
		/// to OpenGL
		///
		/// @param ptr Pointer to the data
		/// @param size The size of the texture to create
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromArray(const float *ptr,fm::vec2s size);

		/////////////////////////////////////////////////////////////
		/// @brief Change the content of the texture
		///
		/// If @a data is NULL or the texture is invalid then the texture is not modified
		///
		/// @param data Pointer to the new data
		/// @param pos coordinate in the texture where the pixels will be put
		/// @param size Size of the target rectangle
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference update(const float *data,fm::vec2s pos,fm::vec2s size);
	};
}
#endif
