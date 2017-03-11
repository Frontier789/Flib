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
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>

namespace fm
{
	template<class>
	class vector3;

	typedef vector3<float> vec3;
}

namespace fg
{
	class CubeTexture;

	/////////////////////////////////////////////////////////////
	/// @brief A special texture used to hold one face of a cube texture
	///
	/// @ingroup Graphics
	/// @see fg::Texture
	/////////////////////////////////////////////////////////////
	class CubeTextureFace : public fg::Texture
	{
		virtual fm::Uint32 getTexRebinding() const; ///< Internal function
		virtual fm::Uint32 getTexBinding() const; ///< Internal function
		virtual fm::Uint32 getTexTarget() const; ///< Internal function

		CubeTexture &m_cubeTex; ///< Reference to the owner cube texture
		unsigned int m_faceId;  ///< The id of this face
		
		fm::Result copyFace(const CubeTexture &cubeTex,unsigned int face); ///< Internal function
		
	public:
		typedef CubeTextureFace &reference;
		typedef const CubeTextureFace &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Construct the cubeface
		/// 
		/// @param cubeTex The cube texture that owns this face
		/// @param faceId The id of this face
		/// 
		/////////////////////////////////////////////////////////////
		CubeTextureFace(CubeTexture &cubeTex,unsigned int faceId);

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// Does not actually copy data only reference
		/// 
		/// @param face The cube face to copy
		/// 
		/////////////////////////////////////////////////////////////
		CubeTextureFace(const CubeTextureFace &face);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		virtual ~CubeTextureFace();

		/////////////////////////////////////////////////////////////
		/// @brief (re)create the texture with given size
		///
		/// @param size The requested size
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result create(fm::vec2s size);

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
		Texture::reference setRepeated(bool repeat);

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
		Texture::reference setSmooth(bool smooth);
		
		friend class CubeTexture;
	};

	/////////////////////////////////////////////////////////////
	/// @brief A six sided OpenGL texture 
	///
	/////////////////////////////////////////////////////////////
	class CubeTexture : public fg::GlObject
	{
		virtual fm::Int32  getInternalFormat() const; ///< Internal function
		virtual fm::Uint32 getFormat() const; ///< Internal function
		virtual fm::Uint32 getType() const;   ///< Internal function

		fm::Size m_size;   ///< The size of the cube's edge
		bool m_isRepeated; ///< True if the texture is repeated after its bounds
		bool m_isSmooth;   ///< If true then linear interpolation is used on magnifying

	public:
		typedef CubeTexture &reference;
		typedef const CubeTexture &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default construtor
		///
		/////////////////////////////////////////////////////////////
		CubeTexture();

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
		CubeTexture(const CubeTexture &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param move The texture to move
		///
		/////////////////////////////////////////////////////////////
		CubeTexture(CubeTexture &&move);

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
		CubeTexture &operator=(const CubeTexture &copy) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		///
		/// @param move The texture to move
		///
		/////////////////////////////////////////////////////////////
		CubeTexture &operator=(CubeTexture &&move);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two cube textures
		///
		/// @param cubeTex The texture to swap with
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		CubeTexture &swap(CubeTexture &cubeTex);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~CubeTexture();

		/////////////////////////////////////////////////////////////
		/// @brief Construtor with given size
		/// 
		/// @param size The size to create with
		/// 
		/////////////////////////////////////////////////////////////
		explicit CubeTexture(fm::Size size);

		/////////////////////////////////////////////////////////////
		/// @brief Construtor with given size
		/// 
		/// @param size The size to create with
		/// 
		/// @return The error-state of the function
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result create(fm::Size size);

		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the cube texture
		/// 
		/// @return The size of the cube texture
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get a reference to one of the cube's six faces
		/// 
		/// @param index The index of the face
		/// 
		/// @return The face of the cube
		/// 
		/////////////////////////////////////////////////////////////
		CubeTextureFace getFace(fm::Size index);

		/////////////////////////////////////////////////////////////
		/// @brief Get a reference to one of the cube's six faces
		/// 
		/// @param index The index of the face
		/// 
		/// @return The face of the cube
		/// 
		/////////////////////////////////////////////////////////////
		CubeTextureFace operator[](fm::Size index);

		/////////////////////////////////////////////////////////////
		/// @brief Get a reference to one of the cube's six faces
		/// 
		/// The face is selected based on the normal direction
		/// 
		/// @param normal The normal of the face
		/// 
		/// @return The face of the cube
		/// 
		/////////////////////////////////////////////////////////////
		CubeTextureFace getFace(const fm::vec3 &normal);

		/////////////////////////////////////////////////////////////
		/// @brief Get a reference to one of the cube's six faces
		/// 
		/// The face is selected based on the normal direction
		/// 
		/// @param normal The normal of the face
		/// 
		/// @return The face of the cube
		/// 
		/////////////////////////////////////////////////////////////
		CubeTextureFace operator[](const fm::vec3 &normal);

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
		reference setRepeated(bool repeat);

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
		reference setSmooth(bool smooth);

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
		/// @brief Bind the texture for usage
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result bind() const;

		/////////////////////////////////////////////////////////////
		/// @brief Bind the texture for usage
		///
		/// @param texture The texture
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(fm::Ref<const CubeTexture> texture);

		/////////////////////////////////////////////////////////////
		/// @brief Get the maximum size in any dimension of a texture
		///
		/// @return Maximum size of a texture
		///
		/////////////////////////////////////////////////////////////
		static fm::Size getMaximumSize();
	};
}

