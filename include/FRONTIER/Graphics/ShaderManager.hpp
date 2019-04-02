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
#ifndef FRONTIER_SHADERMANAGER_HPP_INCLUDED
#define FRONTIER_SHADERMANAGER_HPP_INCLUDED

#include <FRONTIER/Graphics/AssocPoint.hpp>
#include <FRONTIER/System/MatrixStack.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/System/Ref.hpp>

#define FRONTIER_SHADERMANAGER
#include <vector>

namespace fm
{
    template<fm::Size,fm::Size,class> class matrix;
    typedef matrix<4,4,float> mat4;
}

namespace fg
{
    class DrawData;

	/////////////////////////////////////////////////////////////
	/// @brief Class used to manage OpenGL shaders with a camera, matrix stacks and named attributes
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API ShaderManager : public fg::Shader
    {
    public:
		enum UniformState ///< Internal type used to indicate a uniform name's state
		{
			UnknownUni, ///< The uniform name is not known to exist
			MissingUni, ///< The uniform name is known not to exist
			FoundUni    ///< The uniform name is known to exist
		};
		
		std::vector<fm::MatrixStack<4,4,float> > m_stacks; ///< Stores the matrix stacks
		std::map<AssocPoint,std::string> m_assocPoints;    /// Maps assocpoints to shader attribute names
		std::vector<std::string> m_texUseNames; ///< Stores the shader attribute names of the variables that indicate whether a texture is in use
		std::vector<std::string> m_texNames;    ///< Stores the shader attribute names of textures
		std::vector<std::string> m_uniNames;    ///< Stores the shader attribute names of uniforms
		std::vector<UniformState> m_uniState;    ///< Stores the state of the named uniforms
		fm::Camera m_cam; ///< The camera
		fm::Clock m_clk;  ///< The internal clock
        
		virtual fm::Result postProcess(const std::string *data,const unsigned int *types,unsigned int count); ///< Process source of the successfully loaded shader
		virtual fm::Result prepareDraw(const fg::DrawData &data); ///< Internal function used to prepare a drawing operation
		void clearData(); ///< Internal function used to clear all data

    public:
		typedef ShaderManager &reference;
		typedef const ShaderManager &const_reference;
    	
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
        ShaderManager();
        
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param manager The shadermanager to move
		///
		/////////////////////////////////////////////////////////////
		ShaderManager(ShaderManager &&manager);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
        virtual ~ShaderManager();
        
		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		///
		/// @param manager The shadermanager to move
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		ShaderManager &operator=(ShaderManager &&manager);
        
		/////////////////////////////////////////////////////////////
		/// @brief Swap two shader managers
		///
		/// @param manager The shadermanager to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		ShaderManager &swap(ShaderManager &manager);

		/////////////////////////////////////////////////////////////
		/// @brief Assign a shader attribute name with an association point
		/// 
		/// @param attrName The name of the attribute 
		/// @param point The association point
		/// @param overWrite Indicates whether to overwrite the existing assignment
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        ShaderManager &associate(const std::string &attrName,AssocPoint point,bool overWrite = true);
        
		/////////////////////////////////////////////////////////////
		/// @brief Assign all standard shader attribute names
		/// 
		/// @param positionName The name of the position attribute
		/// @param colorName The name of the color attribute
		/// @param texPositionName The name of the texture position attribute
		/// @param normalName The name of the normal attribute
		/// @param tangentName The name of the tangent attribute
		/// @param bitangentName The name of the bitangent attribute
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		ShaderManager &associate(const std::string &positionName,const std::string &colorName,const std::string &texPositionName,const std::string &normalName = "",const std::string &tangentName = "",const std::string &bitangentName = "");
        
		/////////////////////////////////////////////////////////////
		/// @brief Assign all standard shader matrix uniform names
		/// 
		/// @param modelMat The name of the model matrix
		/// @param viewMat The name of the view matrix uniform
		/// @param projMat The name of the projection matrix uniform
		/// @param normalMat The name of the normal matrix
		/// @param colorMat The name of the color matrix
		/// @param texUVMat The name of the texture position matrix
		/// @param plyPos The name of the player position uniform
		/// @param plyView The name of the player view direction uniform
		/// @param time The name of the time uniform
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		virtual ShaderManager &setUniformNames(const std::string &modelMat,const std::string &viewMat,const std::string &projMat,const std::string &normalMat = "",const std::string &colorMat = "",const std::string &texUVMat = "",const std::string &plyPos = "",const std::string &plyView = "",const std::string &time = "");
		
		/////////////////////////////////////////////////////////////
		/// @brief Register a texture's name
		/// 
		/// @param texName The name of the uniform
		/// @param texInUse The name of the uniform (int) to use to indicate whether the texture is in use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        ShaderManager &regTexture(const std::string &texName,const std::string &texInUse = "");
        
		/////////////////////////////////////////////////////////////
		/// @brief Activate a texture
		/// 
		/// @param tex The texture to use
		/// @param texIndex The texture slot to use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        virtual ShaderManager &useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex = 0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the camera of the shader
		/// 
		/// @return the camera
		/// 
		/////////////////////////////////////////////////////////////
		fm::Camera &getCamera();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the camera of the shader
		/// 
		/// @return the camera
		/// 
		/////////////////////////////////////////////////////////////
		const fm::Camera &getCamera() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the clock of the shader
		/// 
		/// @return the clock
		/// 
		/////////////////////////////////////////////////////////////
		fm::Clock &getClock();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the clock of the shader
		/// 
		/// @return the clock
		/// 
		/////////////////////////////////////////////////////////////
		const fm::Clock &getClock() const;

		/////////////////////////////////////////////////////////////
		/// @brief Clear all associations
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        ShaderManager &clearAssociations();

		/////////////////////////////////////////////////////////////
		/// @brief Clear all registered textures
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        ShaderManager &clearTextures();

		/////////////////////////////////////////////////////////////
		/// @brief Update variables of the shader
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::Result update();

		/////////////////////////////////////////////////////////////
		/// @brief Draw all the indexsets of a drawdata
		/// 
		/// @param data The drawdata to use
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
        fm::Result draw(const fg::DrawData &data);

		/////////////////////////////////////////////////////////////
		/// @brief Draw an indexset of a drawdata
		/// 
		/// @param data The drawdata to use
		/// @param indexSet The id of the drawcall in @a data to use
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
        fm::Result draw(const fg::DrawData &data,fm::Size indexSet);

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
        virtual fm::Result draw(const fg::DrawData &data,fm::Size indexBeg,fm::Size indexCount);

		/////////////////////////////////////////////////////////////
		/// @brief Get the model matrix stack
		/// 
		/// @return Reference to the model matrix stack
		/// 
		/////////////////////////////////////////////////////////////
        fm::MatrixStack<4,4,float> &getModelStack();

		/////////////////////////////////////////////////////////////
		/// @brief Get the view matrix stack
		/// 
		/// @return Reference to the view matrix stack
		/// 
		/////////////////////////////////////////////////////////////
        fm::MatrixStack<4,4,float> &getViewStack();

		/////////////////////////////////////////////////////////////
		/// @brief Get the projection matrix stack
		/// 
		/// @return Reference to the projection matrix stack
		/// 
		/////////////////////////////////////////////////////////////
        fm::MatrixStack<4,4,float> &getProjStack();
        
		/////////////////////////////////////////////////////////////
		/// @brief Get the color matrix stack
		/// 
		/// @return Reference to the color matrix stack
		/// 
		/////////////////////////////////////////////////////////////
        fm::MatrixStack<4,4,float> &getColorStack();
        
		/////////////////////////////////////////////////////////////
		/// @brief Get the texture position matrix stack
		/// 
		/// @return Reference to the texture position matrix stack
		/// 
		/////////////////////////////////////////////////////////////
        fm::MatrixStack<4,4,float> &getTexUVStack();
        
		/////////////////////////////////////////////////////////////
		/// @brief Copy stacks from another manager
		/// 
		/// @param source The manager to copy from
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		ShaderManager &copyStacksFrom(const ShaderManager &source);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a ShaderManager capable of drawing simple gui elements
		/// 
		/// @param transf The source transformation to apply
		/// 
		/// @return The ShaderManager or nullptr if shaders are not supported
		/// 
		/////////////////////////////////////////////////////////////
		static ShaderManager *getDefaultShader(fm::Delegate<void,ShaderSource&> transf = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the source of a ShaderManager capable of drawing simple gui elements
		/// 
		/// @param type The type of subshader to get
		/// 
		/// @return The source of given type
		/// 
		/////////////////////////////////////////////////////////////
		static ShaderSource getDefaultShaderSource(ShaderType type);
    };
}

#endif // FRONTIER_SHADERMANAGER_HPP_INCLUDED
