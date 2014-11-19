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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_RENDERSTATES_HPP_INCLUDED
#define FRONTIER_RENDERSTATES_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/Matrix.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_RENDERSTATES
namespace fg
{
	class Texture;
	class Shader;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to hold the texture, the shader and the transformation matrix used when drawing
	///
	/////////////////////////////////////////////////////////////
	class RenderStates
	{
	public:
		const Texture *texture; ///< The texture
		Shader *shader;         ///< The shader
		fm::mat4 transform;     ///< The transformation matrix

		
		RenderStates(const Texture *texture = 0,Shader *shader = 0,const fm::mat4 &transform = fm::mat4());
		RenderStates(const Texture &texture,Shader *shader,const fm::mat4 &transform = fm::mat4());
		RenderStates(const Texture *texture,Shader &shader,const fm::mat4 &transform = fm::mat4());
		RenderStates(const Texture &texture,Shader &shader,const fm::mat4 &transform = fm::mat4());
		RenderStates(const Texture &texture);
		RenderStates(Shader &shader);
		RenderStates(const fm::mat4 &transform);
	};
}
#endif
