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
#ifndef FRONTIER_FLOATTEXTURE_HPP_INCLUDED
#define FRONTIER_FLOATTEXTURE_HPP_INCLUDED
#include <FRONTIER/Graphics/Texture.hpp>
#define FRONTIER_FLOATTEXTURE

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief A special texture that stores the data in floating point representation
	///
	/// @ingroup Graphics
	/// @see fg::Texture
	/////////////////////////////////////////////////////////////
	class FRONTIER_API FloatTexture : public Texture
	{
		fm::Int32 getInternalFormat() const; ///< Internal function
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
		explicit FloatTexture(const FloatTexture &copy);

		/////////////////////////////////////////////////////////////
		/// @brief Create a OpenGL texture from image
		///
		/// This function sends the client-side data (the image)
		/// to OpenGL
		///
		/// If @a img has an invalid size (w or h is 0) or its width or height
		/// is bigger than getMaximumSize() then an error is prompted to fg_log
		/// and the texture is leaved invalid
		///
		/// @param img The image to send to OpenGL
		///
		/////////////////////////////////////////////////////////////
		explicit FloatTexture(const Image &img);
	};
}
#endif
