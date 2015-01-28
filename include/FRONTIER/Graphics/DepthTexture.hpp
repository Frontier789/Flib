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
#ifndef FRONTIER_DEPTHTEXTURE_HPP_INCLUDED
#define FRONTIER_DEPTHTEXTURE_HPP_INCLUDED
#include <FRONTIER/Graphics/Texture.hpp>
#define FRONTIER_DEPTHTEXTURE

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief A special texture used to hold depth values
	///
	/// A depth texture treats its pixels as if they were one unsigned integer each
	///
	/// @ingroup Graphics
	/// @see fg::Texture
	/////////////////////////////////////////////////////////////
	class FRONTIER_API DepthTexture : public Texture
	{
		fm::Int32  getInternalFormat() const; ///< Internal function
		fm::Uint32 getAttachement() const; ///< Internal function
		fm::Uint32 getFormat() const; ///< Internal function
		fm::Uint32 getType() const; ///< Internal function
	public:
		typedef DepthTexture &reference;
		typedef const DepthTexture &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Leaves the texture uninitialized thus invalid
		///
		/////////////////////////////////////////////////////////////
		DepthTexture();

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
		explicit DepthTexture(const DepthTexture &copy);

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
		explicit DepthTexture(const Image &img);

		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the texture in a humanly understandable way
		///
		/// The nearest object would get the color @a znear and the furthest would be @a zfar
		///
		/// @param znear The color of the near clipping plane
		/// @param zfar  The color of the far  clipping plane
		///
		/////////////////////////////////////////////////////////////
		fg::Image copyConvertToImage(fg::Color znear = fg::Color::Black,fg::Color zfar = fg::Color::White) const;
	};
}
#endif
