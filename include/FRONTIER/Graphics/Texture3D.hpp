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
#ifndef FRONTIER_TEXTURE3D_HPP_INCLUDED
#define FRONTIER_TEXTURE3D_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>

#define FRONTIER_TEXTURE3D

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle OpenGL 3D textures
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Texture3D : public GlObject
	{
	protected:
		fm::vec3s m_size;	  ///< The requested size of the texture
		bool m_isRepeated;	  ///< True if the texture is repeated after its bounds
		bool m_isSmooth;	  ///< If true then linear interpolation is used on magnifying

	public:
		virtual fm::Int32  getInternalFormat() const; ///< Internal format of the texture
		virtual fm::Uint32 getTexRebinding() const;   ///< The rebinding point of the texture
		virtual fm::Uint32 getAttachement() const;    ///< Attachement point of the texture
		virtual fm::Uint32 getTexBinding() const;     ///< Binding point of the texture
		virtual fm::Uint32 getTexTarget() const;      ///< Texture3D target associated with the object
		virtual fm::Uint32 getFormat() const; ///< Pixelformat
		virtual fm::Uint32 getType() const;   ///< Type of the texture

	public:
		typedef Texture3D &reference;
		typedef const Texture3D &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// Leaves the texture uninitialized thus invalid
		///
		/////////////////////////////////////////////////////////////
		Texture3D();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// Copies the whole texture back to client memory
		/// and sends the data to a new texture unit
		/// it is a S-L-O-W operation
		///
		/// If @a copy is invalid then the texture is left invalid
		///
		/// @param copy The texture to be copied
		///
		/////////////////////////////////////////////////////////////
		Texture3D(const Texture3D &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// If @a move is invalid then the texture is left invalid
		///
		/// @param move The texture to be moved
		///
		/////////////////////////////////////////////////////////////
		Texture3D(Texture3D &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Create a 3d OpenGL texture with given size and uninitialized data
		/// 
		/// @param size The requested size
		///
		/////////////////////////////////////////////////////////////
		explicit Texture3D(fm::vec3s size);

		/////////////////////////////////////////////////////////////
		/// @brief Create a 3d OpenGL texture with given size and color
		///
		/// @param size The requested size
		/// @param color The color to fill with
		///
		/////////////////////////////////////////////////////////////
		Texture3D(fm::vec3s size,fg::Color color);

		/////////////////////////////////////////////////////////////
		/// @brief Create a 3d OpenGL texture with given size and data
		///
		/// @param size The requested size
		/// @param color The color to load
		///
		/////////////////////////////////////////////////////////////
		Texture3D(fm::vec3s size,const fg::Color *color);

		/////////////////////////////////////////////////////////////
		/// @brief Create a 3d OpenGL texture from given 3d array
		///
		/// @param size The requested size
		/// @param color The color to load
		///
		/////////////////////////////////////////////////////////////
		template<fm::Size W,fm::Size H,fm::Size D>
		explicit Texture3D(const fg::Color (&color)[W][H][D]);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// Automatically deletes the OpenGL id
		///
		/////////////////////////////////////////////////////////////
		virtual ~Texture3D();

		/////////////////////////////////////////////////////////////
		/// @brief (re)create the texture with given size
		///
		/// If size.w or size.h is 0 or bigger than getMaximumSize()
		/// an error is returned
		///
		/// The content of the texture is undefined after successfully calling this
		/// function and should be filled by fg::Texture3D::update or any other method
		/// before usage
		///
		/// @param size The requested size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		virtual fm::Result create(fm::vec3s size);
		
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
		fm::Result create(fm::vec3s size,fg::Color color);

		/////////////////////////////////////////////////////////////
		/// @brief Create a 3d OpenGL texture from image
		///
		/// This function sends the client-side data (the image)
		/// to OpenGL
		///
		/// @param img The image to send to OpenGL
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		virtual fm::Result loadFromMemory(fm::vec3s size,const fg::Color *data);

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
		reference update(const Color *pixels,fm::vec3s pos,fm::vec3s size);

		/////////////////////////////////////////////////////////////
		/// @brief Copy the content of the texture to client-memory
		///
		/// If available an FBO will be used to retrieve the data
		/// if not glGetTexImage will be used (if available)
		/// if neither of the above then a white image (same size as texture) is returned
		/// 
		/// @param error The error returned
		/// @param pos The position of the subcuboid to copy
		/// @param size The size of the subcuboid to copy (zero means entire texture)
		/// 
		/// @return The copied image allocated with new
		///
		/////////////////////////////////////////////////////////////
		Color *copyToMemory(fm::Result *error = nullptr,fm::vec3s pos = fm::vec3s(),fm::vec3s size = fm::vec3s()) const;

		/////////////////////////////////////////////////////////////
		/// @brief Copy the content of the texture to preallocated client-memory
		///
		/// If available an FBO will be used to retrieve the data
		/// if not glGetTexImage will be used (if available)
		/// if neither of the above then a white fg::Image (same size as texture) is returned
		/// 
		/// @param data The preallocated memory region to copy to
		/// @param pos The position of the subcuboid to copy
		/// @param size The size of the subcuboid to copy (zero means entire texture)
		/// 
		/// @return The result
		///
		/////////////////////////////////////////////////////////////
		fm::Result copyToMemory(Color *data,fm::vec3s pos = fm::vec3s(),fm::vec3s size = fm::vec3s()) const;

		/////////////////////////////////////////////////////////////
		/// @brief Bind the texture for usage
		///
		/// This function uses the GL_TXTURE_3D target
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
		static fm::Result bind(fm::Ref<const Texture3D> texture);

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
		fm::vec3 getSize() const;

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
		reference operator=(const Texture3D &tex) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Assignment operator
		/// 
		/// @param tex The texture to move
		/// 
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference operator=(Texture3D &&tex);
		
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
		reference swap(Texture3D &tex);
	};
}
#endif

#ifndef FROTNIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Texture3D.inl>
#endif
