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
#ifndef FRONTIER_TEXTURECONVOLUTION_HPP_INCLUDED
#define FRONTIER_TEXTURECONVOLUTION_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Result.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#define FRONTIER_TEXTURECONVOLUTION
#include <vector>

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to apply a convolution to textures
	/// 
	/// Linear convolutions calculate the convolution matrix as
	/// the product of the linear coefficients
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API TextureConvolution
	{
		std::vector<float> m_data; ///< The convolution matrix
		fm::Result m_shaderResult; ///< Result of the shader compilation
		FrameBuffer m_fboSecond;   ///< The second framebuffer
		ShaderManager m_shader;    ///< The shader used
		FrameBuffer m_fboFirst;    ///< The first framebuffer
		fm::vec2s m_matrixSize;    ///< The size of the matrix
		DrawData m_drawData; ///< The draw data used
		Texture m_texture;   ///< The temporary texture used
		Texture *m_target;   ///< The target texture
		bool m_linear;       ///< Stores whether the convolution is linear
		
		void setupShader();   ///< Internal function
		void setupFBOs();     ///< Internal function
		void normalizeData(); ///< Internal function
		
	public:
		typedef TextureConvolution &reference;
		typedef const TextureConvolution &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		TextureConvolution();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~TextureConvolution() = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param move The object to move
		/// 
		/////////////////////////////////////////////////////////////
		TextureConvolution(TextureConvolution &&move);
		
		/////////////////////////////////////////////////////////////
		/// @brief Move operator
		/// 
		/// @param move The object to move
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		TextureConvolution &operator=(TextureConvolution &&move);
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap two objects
		/// 
		/// @param conv The object to swap with
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		TextureConvolution &swap(TextureConvolution &conv);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a linear convolution
		/// 
		/// @param coefs The coefficients of the convolution
		/// @param target The target texture
		/// @param normalize Scale the coefficients to sum to 1
		/// 
		/////////////////////////////////////////////////////////////
		template<fm::Size N,class T>
		TextureConvolution(const T (&coefs)[N],fm::Ref<Texture> target = nullptr,bool normalize = false);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a 2D convolution
		/// 
		/// @param coefs The coefficients of the convolution
		/// @param target The target texture
		/// @param normalize Scale the coefficients to sum to 1
		/// 
		/////////////////////////////////////////////////////////////
		template<fm::Size W,fm::Size H,class T>
		TextureConvolution(const T (&coefs)[W][H],fm::Ref<Texture> target = nullptr,bool normalize = false);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the target texture of the convolution
		/// 
		/// @param target The new target
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		reference setData(const T *ptr,fm::vec2s kernelSize,fm::Ref<Texture> target,bool normalize,bool linear);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the target texture of the convolution
		/// 
		/// @param target The new target
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setTarget(fm::Ref<Texture> target);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the target texture of the convolution
		/// 
		/// @return The target
		/// 
		/////////////////////////////////////////////////////////////
		Texture *getTarget() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a linear convolution
		/// 
		/// sets @a newTarget as target
		/// 
		/// @param newTarget The texture to apply to
		/// @param applyCount The number of times to apply
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference applyTo(Texture &newTarget,fm::Size applyCount = 1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct a linear convolution
		/// 
		/// sets @a texture as target
		/// 
		/// @param texture The texture to apply to
		/// @param applyCount The number of times to apply
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference applyToTarget(fm::Size applyCount = 1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Check whether convolution appliers are available
		/// 
		/// @return True iff convolution appliers are available
		/// 
		/////////////////////////////////////////////////////////////
		static bool isAvailable();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the result from shader compilation
		/// 
		/// @return The result
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result getShaderResult() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the result from shader compilation
		/// 
		/// @return The result
		/// 
		/////////////////////////////////////////////////////////////
		static TextureConvolution getGaussianBlurrer(fm::Size kernelSize = 7,double sigma = 1.0);
	};
}
#endif // FRONTIER_TEXTURECONVOLUTION_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/TextureConvolution.inl>
#endif

