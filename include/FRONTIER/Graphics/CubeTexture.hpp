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
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Result.hpp>

namespace fm
{
	template<class>
	class vector3;

	typedef vector3<float> vec3;
}

namespace fg
{
	class CubeTexture;

	class CubeTextureFace : public fg::Texture
	{
		/////////////////////////////////////////////////////////////
		virtual fm::Uint32 getTexRebinding() const; ///< Internal function
		virtual fm::Uint32 getTexBinding() const; ///< Internal function
		virtual fm::Uint32 getTexTarget() const; ///< Internal function

		CubeTexture &m_cubeTex;
		unsigned int m_faceId;

	public:
		typedef CubeTextureFace &reference;
		typedef const CubeTextureFace &const_reference;

		/////////////////////////////////////////////////////////////
		CubeTextureFace(CubeTexture &cubeTex,unsigned int faceId);

		/////////////////////////////////////////////////////////////
		virtual ~CubeTextureFace();

		/////////////////////////////////////////////////////////////
		fm::Result create(fm::Size width,fm::Size height);

		/////////////////////////////////////////////////////////////
		Texture::reference setRepeated(bool repeat);

		/////////////////////////////////////////////////////////////
		Texture::reference setSmooth(bool smooth);
	};

	class CubeTexture : public fg::GlObject
	{
		virtual fm::Int32  getInternalFormat() const; ///< Internal function
		virtual fm::Uint32 getFormat() const; ///< Internal function
		virtual fm::Uint32 getType() const;   ///< Internal function

		fm::Size m_size;
		bool m_isRepeated;
		bool m_isSmooth;

	public:
		typedef CubeTexture &reference;
		typedef const CubeTexture &const_reference;

		/////////////////////////////////////////////////////////////
		CubeTexture();

		/////////////////////////////////////////////////////////////
		virtual ~CubeTexture();

		/////////////////////////////////////////////////////////////
		explicit CubeTexture(fm::Size size);

		/////////////////////////////////////////////////////////////
		fm::Result create(fm::Size size);

		/////////////////////////////////////////////////////////////
		fm::Size getSize() const;

		/////////////////////////////////////////////////////////////
		CubeTextureFace getFace(fm::Size index);

		/////////////////////////////////////////////////////////////
		CubeTextureFace operator[](fm::Size index);

		/////////////////////////////////////////////////////////////
		CubeTextureFace getFace(const fm::vec3 &normal);

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
		fm::Result bind() const;

		/////////////////////////////////////////////////////////////
		static fm::Result bind(const CubeTexture *texture);

		/////////////////////////////////////////////////////////////
		static fm::Result bind(const CubeTexture &texture);

		/////////////////////////////////////////////////////////////
		/// @brief Get the maximum size in any dimension of a texture
		///
		/// @return Maximum size of a texture
		///
		/////////////////////////////////////////////////////////////
		static fm::Size getMaximumSize();
	};
}

