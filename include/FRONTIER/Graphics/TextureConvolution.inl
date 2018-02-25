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
#ifndef FRONTIER_TEXTURECONVOLUTION_INL_INCLUDED
#define FRONTIER_TEXTURECONVOLUTION_INL_INCLUDED

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<fm::Size N,class T>
	inline TextureConvolution::TextureConvolution(const T (&coefs)[N],
												  fm::Ref<Texture> target,
												  bool normalize)
	{
		setData(&coefs[0],fm::vec2s(N,1),target,normalize,true);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline TextureConvolution::TextureConvolution(std::initializer_list<T> coefs,
												  fm::Ref<Texture> target,
												  bool normalize)
	{
		setData(coefs.begin(),fm::vec2s(coefs.size(),1),target,normalize,true);
	}
		
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,class T>
	inline TextureConvolution::TextureConvolution(const T (&coefs)[W][H],
												  fm::Ref<Texture> target,
												  bool normalize)
	{
		setData(&coefs[0][0],fm::vec2s(W,H),target,normalize,false);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline TextureConvolution::TextureConvolution(std::initializer_list<std::initializer_list<T>> coefs,
												  fm::Ref<Texture> target,
												  bool normalize)
	{
		setData(coefs.begin(),fm::vec2s(coefs.size(),coefs.begin()->size()),target,normalize,true);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline TextureConvolution::reference TextureConvolution::setData(const T *ptr,
																	 fm::vec2s kernelSize,
																	 fm::Ref<Texture> target,
																	 bool normalize,
																	 bool linear)
	{
		m_data       = std::vector<float>(ptr,ptr+kernelSize.area());
		m_matrixSize = kernelSize;
		m_target     = target;
		
		if (normalize) normalizeData();
		
		m_linear = linear;
		
		setupShader();
		setupFBOs();
		
		return *this;
	}
}
#endif // FRONTIER_TEXTURECONVOLUTION_INCLUDED

