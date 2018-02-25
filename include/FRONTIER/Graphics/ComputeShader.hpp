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
#ifndef FRONTIER_COMPUTESHADER_HPP_INCLUDED
#define FRONTIER_COMPUTESHADER_HPP_INCLUDED
#include <FRONTIER/Graphics/FloatTexture.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#define FRONTIER_COMPUTESHADER

namespace fg
{
	class ComputeShader : public GlObject
	{
	public:
		Shader m_shader; ///< Internal shader
		
	public:
        typedef ComputeShader &reference;
        typedef const ComputeShader &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// After this call the Shader is uninitialized (invalid)
		///
		/////////////////////////////////////////////////////////////
        ComputeShader();

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param shader The shader to move
		///
		/////////////////////////////////////////////////////////////
		ComputeShader(ComputeShader &&shader);

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		///
		/// @param shader The shader to move
		///
		/////////////////////////////////////////////////////////////
		ComputeShader &operator=(ComputeShader &&shader);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two shaders
		///
		/// @param shader The shader to swap with
		///
		/////////////////////////////////////////////////////////////
		ComputeShader &swap(ComputeShader &shader);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// Automatically frees OpenGL id
		///
		/////////////////////////////////////////////////////////////
        virtual ~ComputeShader();

		/////////////////////////////////////////////////////////////
		/// @brief Load the compute shader program a file
		///
		/// @param file The name of the file to load from
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromFile(const std::string &file);

		/////////////////////////////////////////////////////////////
		/// @brief Load the compute shader program from source stored in memory
		///
		/// @param data The surce of the shader
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromMemory(const std::string &data);
		
		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from source stored in memory
		///
		/// @param data The sources to be compiled
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result loadFromMemory(const fg::ShaderSource &data);

		/////////////////////////////////////////////////////////////
		/// @brief Bind the shader program for usage
		///
		/////////////////////////////////////////////////////////////
        void bind() const;

		/////////////////////////////////////////////////////////////
		/// @brief Find the "id" of a uniform
		///
		/// This function is mostly used internally
		///
		/// @param name The name of the uniform to find
		///
		/// @return -1 on error, the location otherwise
		///
		/////////////////////////////////////////////////////////////
        int getUniformLocation(const std::string &name);
        
		/////////////////////////////////////////////////////////////
		/// @brief Fill the necessary data about uniforms
		///
		/// This function is used internally
		///
		/////////////////////////////////////////////////////////////
        void fillUniformData();

		/////////////////////////////////////////////////////////////
		/// @brief Check if a uniform of a given name is present
		///
		/// @param name The name of the uniform to find
		///
		/// @return 0 on error, the location+1 otherwise
		///
		/////////////////////////////////////////////////////////////
        int hasUniform(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		/// 
		/// This call is forwarded to the internal shader object
		/// And then type checked
		/// 
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setUniform(const std::string &name,const T &x);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an image2D uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// Binding as an image is useful when using imageLoad / imageStore
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		/// @param sampler Decides whether the texture shall be bound as a sampler or as an image
		/// 
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const Texture &tex,bool sampler = false);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an image2D uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// Binding as an image is useful when using imageLoad / imageStore
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		/// @param sampler Decides whether the texture shall be bound as a sampler or as an image
		/// 
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const Texture *tex,bool sampler = false);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an image2D uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// Binding as an image is useful when using imageLoad / imageStore
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		/// @param sampler Decides whether the texture shall be bound as a sampler or as an image
		/// 
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const FloatTexture &tex,bool sampler = false);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an image2D uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// Binding as an image is useful when using imageLoad / imageStore
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		/// @param sampler Decides whether the texture shall be bound as a sampler or as an image
		/// 
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const FloatTexture *tex,bool sampler = false);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an array of uniforms
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		/// 
		/// The elements od the input array are forwarded one by one as name + "[" + 0..N-1 + "]"
		/// 
		/// @param name The name of the uniform
		/// @param values The values
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size N>
		fm::Result setUniform(const std::string &name,const T (&values)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a compute shader program for usage
		///
		/// If @a program is NULL the current
		/// shader program will be unbound
		///
		/// @param program The program to be bound
		///
		/////////////////////////////////////////////////////////////
        static void bind(fm::Ref<const ComputeShader> program);

		/////////////////////////////////////////////////////////////
		/// @brief Find out if compute shaders are available
		///
		/// You should check for availability before using
		/// compute shaders.
		///
		/// @return True if compute shaders are available
		///
		/////////////////////////////////////////////////////////////
        static bool isAvailable();

		/////////////////////////////////////////////////////////////
		/// @brief Run a function on all uniforms
		/// 
		/// @param func The function to call
		/// 
		/////////////////////////////////////////////////////////////
		void forEachUniform(fm::Delegate<void,std::string,Shader::UniformData> func) const;

		/////////////////////////////////////////////////////////////
		/// @brief Check if the shader is successfully loaded 
		/// 
		/// @return True iff the shader is ready to use
		/// 
		/////////////////////////////////////////////////////////////
		bool isLoaded() const;

		/////////////////////////////////////////////////////////////
		/// @brief run The shader once with given work group dimensions
		/// 
		/// Same as dispatch
		/// 
		/// @param workGroupDims The dimensions of the workgroup layout
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result run(fm::vec3s workGroupDims) const;

		/////////////////////////////////////////////////////////////
		/// @brief run The shader once with given work group dimensions
		/// 
		/// Same as run
		/// 
		/// @param workGroupDims The dimensions of the workgroup layout
		/// 
		/// @return The result of the operation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result dispatch(fm::vec3s workGroupDims) const;
	};
}
#endif // FRONTIER_SHADER_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/ComputeShader.inl>
#endif
