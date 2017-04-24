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
#ifndef FRONTIER_TEXTURE_HPP_INCLUDED
#define FRONTIER_TEXTURE_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/Vector2.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>

#define FRONTIER_TEXTURE

namespace fm
{
	template<fm::Size,fm::Size,class> class matrix;
	template<class> class vector4;
	
	typedef matrix<4,4,float> mat4;
	typedef vector4<float> vec4;
}

namespace fg
{
	class Image;
	class Color;
	
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Class used to handle OpenGL 2D textures
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Texture : public GlObject
	{
	protected:
		fm::vec2s m_realSize; ///< The actul size of the texture
		fm::vec2s m_size;	  ///< The requested size of the texture
		bool m_isRepeated;	  ///< True if the texture is repeated after its bounds
		bool m_isSmooth;	  ///< If true then linear interpolation is used on magnifying

	public:
		virtual fm::Int32  getInternalFormat() const; ///< Internal function
		virtual fm::Uint32 getTexRebinding() const;   ///< Internal function
		virtual fm::Uint32 getAttachement() const;    ///< Internal function
		virtual fm::Uint32 getTexBinding() const;     ///< Internal function
		virtual fm::Uint32 getTexTarget() const;      ///< Internal function
		virtual fm::Uint32 getFormat() const; ///< Internal function
		virtual fm::Uint32 getType() const;   ///< Internal function

	public:
		typedef Texture &reference;
		typedef const Texture &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Leaves the texture uninitialized thus invalid
		///
		/////////////////////////////////////////////////////////////
		Texture();

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
		Texture(const Texture &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// If @a move is invalid then the texture is leaved invalid
		///
		/// @param move The texture to be moved
		///
		/////////////////////////////////////////////////////////////
		Texture(Texture &&move);

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
		Texture(const Image &img);

		/////////////////////////////////////////////////////////////
		/// @brief Create a OpenGL texture from an image in a file
		///
		/// This function sends the client-side data (the image)
		/// to OpenGL
		///
		/// @param filename The file's name
		///
		/////////////////////////////////////////////////////////////
		explicit Texture(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Create a OpenGL texture with given size and uninitialized data
		/// 
		/// @param size The requested size
		///
		/////////////////////////////////////////////////////////////
		explicit Texture(fm::vec2s size);

		/////////////////////////////////////////////////////////////
		/// @brief Create a OpenGL texture with given size and uninitialized data
		///
		/// @param size The requested size
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		explicit Texture(fm::vec2s size,const fm::vec4 &color);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// Automatically deletes the OpenGL id
		///
		/////////////////////////////////////////////////////////////
		virtual ~Texture();

		/////////////////////////////////////////////////////////////
		/// @brief (re)create the texture with given size
		///
		/// If size.w or size.h is 0 or bigger than getMaximumSize()
		/// an error is returned
		///
		/// The content of the texture is undefined after successfully calling this
		/// function and should be filled by fg::Texture::update or any other method
		/// before usage
		///
		/// @param size The requested size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		virtual fm::Result create(fm::vec2s size);
		
		/////////////////////////////////////////////////////////////
		/// @brief (re)create the texture with given size
		///
		/// If size.w or size.h is 0 or bigger than getMaximumSize()
		/// an error is returned
		///
		/// @param size The requested size
		/// @param color The color to fill with
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result create(fm::vec2s size,const fm::vec4 &color);

		/////////////////////////////////////////////////////////////
		/// @brief Create a OpenGL texture from image
		///
		/// This function sends the client-side data (the image)
		/// to OpenGL
		///
		/// If @a img has an invalid size (w or h is 0) or its width or height
		/// is bigger than getMaximumSize() then an error is prompted to fg_log
		/// and the texture is not modified
		///
		/// @param img The image to send to OpenGL
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		virtual fm::Result loadFromImage(const Image &img);

		/////////////////////////////////////////////////////////////
		/// @brief Load a OpenGL texture from a file
		///
		/// This function sends the client-side data (the loaded image)
		/// to OpenGL
		///
		/// This function loads the file with fg::Image::loadFromFile and
		/// sends it to OpenGL using fg::Texture::loadFromImage
		///
		/// If the file is not accessible or has an unsopported extension
		/// or has invalid data then an error is prompted to fg_log and the texture
		/// is not modified
		///
		/// @param filename The name of the image file
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromFile(const std::string &filename);

		/////////////////////////////////////////////////////////////
		/// @brief Change the repeate flag
		///
		/// That flag is used by OpenGL when requesting out-of-bounds
		/// texels of the texture;
		///
		/// @param repeat New value of the repeated flag
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		virtual reference setRepeated(bool repeat);

		/////////////////////////////////////////////////////////////
		/// @brief Change the smoothness flag
		///
		/// That flag is used by OpenGL when requesting a point
		/// that is between two texels
		///
		/// @param smooth New value of the smooth flag
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		virtual reference setSmooth(bool smooth);

		/////////////////////////////////////////////////////////////
		/// @brief Get the state of the repeate flag
		///
		/// @return True if the repeate flag is set
		///
		/////////////////////////////////////////////////////////////
		bool isRepeated() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the state of the smoothness flag
		///
		/// @return True if the smoothness flag is set
		///
		/////////////////////////////////////////////////////////////
		bool isSmooth() const;

		/////////////////////////////////////////////////////////////
		/// @brief Change the content of the texture
		///
		/// If @a pixel is NULL or the texture is invalid then the texture is not modified
		///
		/// @param pixels Pointer to the new color values
		/// @param pos coordinate in the texture where the pixels will be put
		/// @param size Size of the target rectangle
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference update(const Color *pixels,fm::vec2s pos,fm::vec2s size);

		/////////////////////////////////////////////////////////////
		/// @brief Change the content of the texture
		///
		/// @a image will be blit at @a pos
		///
		/// @param image The image to blit
		/// @param pos The position where the image will be blit
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference update(const Image &image,fm::vec2s pos = fm::vec2s());

		/////////////////////////////////////////////////////////////
		/// @brief Copy the content of the texture to client-memory
		///
		/// If available an FBO will be used to retrieve the data
		/// if not glGetTexImage will be used (if available)
		/// if neither of the above then a white fg::Image (same size as texture) is returned
		/// 
		/// @param error The error returned
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		Image copyToImage(fm::Result *error = nullptr) const;

		/////////////////////////////////////////////////////////////
		/// @brief Bind the texture for usage
		///
		/// This function uses the GL_TXTURE_2D target
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result bind() const;

		/////////////////////////////////////////////////////////////
		/// @brief Bind a texture for usage
		///
		/// This function uses the GL_TXTURE_2D target
		/// If @a texture is NULL the actually bound texture will be unbound
		///
		/// @param texture The texture to be bound (or NULL)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(fm::Ref<const Texture> texture);

		/////////////////////////////////////////////////////////////
		/// @brief Get the pixel-to-unit transformation for the texture
		///
		/// The texture is always 1x1 unit in size and left-up is 0;0
		///
		/// @return The transformation matrix
		///
		/////////////////////////////////////////////////////////////
		fm::matrix<4,4,float> getPixToUnitMatrix() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the maximum width (same as max height) of a texture
		///
		/// @return Maximum size of a texture
		///
		/////////////////////////////////////////////////////////////
		static fm::Size getMaximumSize();

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the texture
		///
		/// @return The size of the texture
		///
		/////////////////////////////////////////////////////////////
		fm::vec2 getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the real size of the texture
		///
		/// @return The real size of the texture
		///
		/////////////////////////////////////////////////////////////
		fm::vec2 getRealSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Find out if the texture is valid
		///
		/// @return true iff the texture is valid
		///
		/////////////////////////////////////////////////////////////
		operator bool() const;

		/////////////////////////////////////////////////////////////
		/// @brief Assignment operator
		/// 
		/// @param tex The texture to copy
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(const Texture &tex) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Assignment operator
		/// 
		/// @param tex The texture to move
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(Texture &&tex);
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of the two objects
		///
		/// swaps in constant time
		///
		/// @param tex The object to swap with
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference swap(Texture &tex);

	private:
		/////////////////////////////////////////////////////////////
		/// @brief Convert size to an acceptable size
		///
		/// This function is private and used internally
		///
		/// @param size size
		///
		/// @return Real size
		///
		/////////////////////////////////////////////////////////////
		static fm::vec2s getValidSize(fm::vec2s size);
	};
}
#endif
