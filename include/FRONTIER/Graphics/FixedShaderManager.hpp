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
#ifndef FRONTIER_FIXEDSHADERMANAGER_HPP_INCLUDED
#define FRONTIER_FIXEDSHADERMANAGER_HPP_INCLUDED

#include <FRONTIER/Graphics/ShaderManager.hpp>

#define FRONTIER_FIXEDSHADERMANAGER

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Shader manager that utilises the fixed pipline rendering of OpenGL (to be used with version prior to 3.0)
	///
	/////////////////////////////////////////////////////////////
    class FixedShaderManager : public fg::ShaderManager
    {
		fm::Result prepareDraw(const fg::DrawData &data); ///< Internal function used to prepare a drawing operation
		const fg::Texture *m_activeTex; ///< Holds the active texture

    public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
        FixedShaderManager();

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
        ~FixedShaderManager();
        
		/////////////////////////////////////////////////////////////
		/// @brief Assign all standard shader matrix uniform names
		/// 
		/// @param projMat The name of the projection matrix uniform
		/// @param modelMat The name of the model matrix
		/// @param viewMat The name of the view matrix uniform
		/// @param normalMat The name of the normal matrix
		/// @param colorMat The name of the color matrix
		/// @param texUVMat The name of the texture position matrix
		/// @param plyPos The name of the player position uniform
		/// @param plyView The name of the player view direction uniform
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        ShaderManager &setUniformNames(const std::string &projMat,const std::string &modelMat,const std::string &viewMat = "",const std::string &normalMat = "",const std::string &colorMat = "",const std::string &texUVMat = "",const std::string &plyPos = "",const std::string &plyView = "");
		
		/////////////////////////////////////////////////////////////
		/// @brief Activate a texture
		/// 
		/// @param tex The texture to use
		/// @param texIndex The texture slot to use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        ShaderManager &useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex = 0);
        
		/////////////////////////////////////////////////////////////
		/// @brief Update variables of the shader
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result update();
        
		/////////////////////////////////////////////////////////////
		/// @brief Draw multiple indexsets of a drawdata
		/// 
		/// @param data The drawdata to use
		/// @param indexBeg The index of the first drawcall to use
		/// @param indexCount The number of the drawcalls to use
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result draw(const fg::DrawData &data,fm::Size indexBeg = 0,fm::Size indexCount = 1);
    };
}

#endif // FRONTIER_FIXEDSHADERMANAGER_HPP_INCLUDED

