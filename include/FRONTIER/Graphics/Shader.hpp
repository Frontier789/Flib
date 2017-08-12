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
#ifndef FRONTIER_SHADER_HPP_INCLUDED
#define FRONTIER_SHADER_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>

#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/Delegate.hpp>

#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>

#define FRONTIER_SHADER_ATTRIB_POINTER_ENABLED_BIT (fm::Size(1))
#define FRONTIER_SHADER_UNIFORM_ASSOCIATED_BIT (fm::Size(2))
#define FRONTIER_SHADER_ATTRIB_ASSOCIATED_BIT  (fm::Size(2))

#define FRONTIER_SHADER
#include <type_traits>
#include <vector>
#include <string>
#include <map>

namespace fm
{
	template<class> class vector2;
	template<class> class vector3;
	template<class> class vector4;

	typedef vector2<float> vec2;
	typedef vector3<float> vec3;
	typedef vector4<float> vec4;
	typedef vector2<int> vec2i;
	typedef vector3<int> vec3i;
	typedef vector4<int> vec4i;

	template<fm::Size W,fm::Size H,class T>
	class matrix;

	typedef matrix<2,2,float> mat2;
	typedef matrix<3,3,float> mat3;
	typedef matrix<4,4,float> mat4;
}

namespace fg
{
	class CubeTexture;
	class Texture;
	class Color;

	/////////////////////////////////////////////////////////////
	/// @brief Encodes types of blending
	///
	/// By default blending is not used (same as Overwrite)
	///
	/////////////////////////////////////////////////////////////
	enum BlendMode {
		Overwrite, ///< Destination = Input
		Additive,  ///< Destination = Destination + Input
		Alpha      ///< Destination = Destination*(1-Input.a) + Input*Input.a
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle OpenGL shader programs in language <a href="http://en.wikipedia.org/wiki/GLSL">GLSL</a>
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Shader : public fm::NonCopyable, public GlObject
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Pod type for describing a shader input's properties (like uniform or attribute)
		///
		/////////////////////////////////////////////////////////////
		class InputData
		{
		public:
			fm::Size flags; ///< Extra flags 
			fm::Size type;  ///< The type of the shader input
			fm::Size size;  ///< The number of array elements in the shader input
			int location;   ///< The id of the shader input
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/// @param type type
			/// @param size size
			/// @param location location
			///
			/////////////////////////////////////////////////////////////
			InputData(fm::Size type=0,fm::Size size=0,int location=0);
			
			/////////////////////////////////////////////////////////////
			/// @brief Check if the shader input is an array
			///
			/// @return True iff the shader input is an array
			///
			/////////////////////////////////////////////////////////////
			bool isArray() const;
			
			/////////////////////////////////////////////////////////////
			/// @brief Check if the shader input is an associated one meaning the shader manager manages it
			///
			/// @return True iff the shader input is an associated one
			///
			/////////////////////////////////////////////////////////////
			bool isAssociated() const;
		};
		
		typedef InputData UniformData; ///< Uniform data
		typedef InputData AttribData;  ///< Attribute data
		
	protected:
		/////////////////////////////////////////////////////////////
		/// @brief Class used to store data about uniform textures
		///
		/// Uniforms are "global" variables in GLSL
		/// they can only be modified by the application (not from shader program)
		///
		/// This class is used internally and you shouldn't worry about it
		///
		/////////////////////////////////////////////////////////////
	    class FRONTIER_API TexUniformData
	    {
	    public:
			int location; ///< The "id" of the uniform
			int slot;	  ///< The assigned texture slot to the uniform
			int act_id;   ///< The id the actually bound texture

			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/// Again, this is internal stuff
			///
			/// @param location location
			/// @param slot slot
			/// @param act_id act_id
			///
			/////////////////////////////////////////////////////////////
			TexUniformData(int location=0,int slot=0,int act_id=0);
	    };
		
		std::map<std::string, TexUniformData > m_textures; ///< The state and name of the texture uniforms
		std::map<std::string, AttribData> m_attributes;    ///< The data of the uniforms
		std::map<std::string, UniformData> m_uniforms;     ///< The data of the uniforms
		std::vector<unsigned int> m_subShaders; ///< The id of the shaders that builds up the shader program
		fm::Size m_texCounter; ///< The counter used when activating texture slots
		fm::Uint32 m_defVao;   ///< Default vao for core profiles
		BlendMode m_blendMode; ///< The used blending
		bool m_hasInstancing;  ///< Stores whether the shader can use instancing
		fm::Result link();     ///< Internal function used to link the compiled shaders to the shader program
		void init();           ///< Internal function used at setup
		fm::Result freeSubShaders(); ///< Internal function used at clean-up
		virtual void clearData();    ///< clear uniforms, attributes, textures etc
		virtual fm::Result postProcess(const std::string *data,const unsigned int *types,unsigned int count); ///< Process source of the successfully loaded shader

    public:
        typedef Shader &reference;
        typedef const Shader &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// After this call the Shader is uninitialized (invalid)
		///
		/////////////////////////////////////////////////////////////
        Shader();

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param shader The shader to move
		///
		/////////////////////////////////////////////////////////////
		Shader(Shader &&shader);

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		///
		/// @param shader The shader to move
		///
		/////////////////////////////////////////////////////////////
		Shader &operator=(Shader &&shader);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two shaders
		///
		/// @param shader The shader to swap with
		///
		/////////////////////////////////////////////////////////////
		Shader &swap(Shader &shader);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// Automatically frees OpenGL id
		///
		/////////////////////////////////////////////////////////////
        virtual ~Shader();

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from files
		///
		/// @param files A pointer to the names of files to be compiled
		/// @param types A pointer to the types of shaders
		/// @param count The number of shaders in the shader program
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromFiles(const std::string *files,const unsigned int *types,unsigned int count);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from files
		///
		/// It is assumed that the first file contains the source
		/// of the vertex shader while the second one contains the source of
		/// the fragment shader
		///
		/// @param vertexShaderFile Name of the file that contains the vertex shader source
		/// @param fragmentShaderFile Name of the file that contains the fragment shader source
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromFiles(const std::string &vertexShaderFile,const std::string &fragmentShaderFile);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from files
		///
		/// It is assumed that the first file contains the source
		/// of the vertex shader while the second one contains the source of
		/// the fragment shader
		///
		/// @param vertexShaderFile Name of the file that contains the vertex shader source
		/// @param geometryShaderFile Name of the file that contains the geometry shader source
		/// @param fragmentShaderFile Name of the file that contains the fragment shader source
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromFiles(const std::string &vertexShaderFile,const std::string &geometryShaderFile,const std::string &fragmentShaderFile);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from sources stored in memory
		///
		/// @param data A pointer to the sources to be compiled
		/// @param types A pointer to the types of shaders
		/// @param count The number of shaders in the shader program
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromMemory(const std::string *data,const unsigned int *types,unsigned int count);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from sources stored in memory
		///
		/// It is assumed that the first string contains the source
		/// of the vertex shader while the second one contains the source of
		/// the fragment shader
		///
		/// @param vertexShaderData The source of vertex shader
		/// @param fragmentShaderData The source of fragment shader
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result loadFromMemory(const std::string &vertexShaderData,const std::string &fragmentShaderData);

		/////////////////////////////////////////////////////////////
		/// @brief Bind the shader program for usage
		///
		/// Please note that an invalid shader
		/// program shouldn't be bound
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
		/// @brief Fill the necessary data about uniforms and attributes
		///
		/// This function is used internally
		///
		/////////////////////////////////////////////////////////////
        void fillAttribAndUniformData();

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
		/// @brief Enable/disable a given attribute pointer
		///
		/// @param name The name of the attribute
		/// @param enable True to enable false to disable
		/// 
		/// @return The error-state of the function
		/// 
		/////////////////////////////////////////////////////////////
		fm::Result enableAttribPointer(const std::string &name,bool enable = true);

		/////////////////////////////////////////////////////////////
		/// @brief Find the "id" of an attribute
		///
		/// This function is mostly used internally
		///
		/// @param name The name of the attribute to find
		///
		/// @return -1 on error, the location otherwise
		///
		/////////////////////////////////////////////////////////////
        int getAttribLocation(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Check if an attribute of a given name is present
		///
		/// @param name The name of the attribute to find
		///
		/// @return 0 on error, the location+1 otherwise
		///
		/////////////////////////////////////////////////////////////
        int hasAttribute(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one attributes
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// This function is mostly used internally
		///
		/// @param name The name of the attribute
		/// @param components The number of components in the attribute
		/// @param type The type of data component (e.g. GL_FLOAT)
		/// @param normalize Specify if the data should be normalized when accessed
		/// @param pointer A pointer to the data
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        fm::Result setAttribPointer(const std::string &name,unsigned int components,unsigned long type,bool normalize,const void *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with a one dimensional attribute
		///
		/// If the shader program is invalid no error will be returned
		/// and the shader program will not be modified
		///
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T,class = typename std::enable_if<fg::Is_GLDataType<T>::value>::type>
		fm::Result setAttribPointer(const std::string &name,const T *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one multidimensional attribute
		///
		/// If the shader program is invalid no error will be returned
		/// and the shader program will not be modified
		///	
		///	The T::components will be used to determine the number of components 
		/// And T::component_type to determine the type
		///	
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T,class = int,class = typename std::enable_if<!fg::Is_GLDataType<T>::value>::type>
		fm::Result setAttribPointer(const std::string &name,const T *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an int uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,int x);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec2i uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec2i &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec3i uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec3i &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec4i uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec4i &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a float uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,float x);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec2 uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec2 &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec3 uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec3 &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec4 uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec4 &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat2 uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::mat2 &m);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat3 uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::mat3 &m);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat4 uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::mat4 &m);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of color uniform (vec4)
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param c The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const Color &c);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a sampler2D uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const Texture &tex);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a sampler2D uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const Texture *tex);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a samplerCube uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const CubeTexture &tex);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a samplerCube uniform
		///
		/// If the shader program is invalid no error will be returnes
		/// and the shader program will not be modified
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const CubeTexture *tex);

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
		/// @brief Bind a shader program for usage
		///
		/// If @a program is NULL the current
		/// shader program will be unbound
		///
		/// After this, a call to glDrawArrays,
		/// glDrawElements or fg::draw
		/// will use this shader program
		///
		/// @param program The program to be bound
		///
		/////////////////////////////////////////////////////////////
        static void bind(fm::Ref<const Shader> program);

		/////////////////////////////////////////////////////////////
		/// @brief Find out if shaders are available
		///
		/// You should check for availability before using
		/// shaders.
		///
		/// @return True if shaders are available
		///
		/////////////////////////////////////////////////////////////
        static bool isAvailable();

		/////////////////////////////////////////////////////////////
		/// @brief Compile a shader from source
		///
		/// If the returned value is not 0 it is a
		/// valid OpenGL shader and can be attached to a shader program
		/// and it should be deleted using glDeleteShader by the caller
		///
		/// This function is used internally and
		/// you should only call this if you know
		/// exactly what you are doing
		///
		/// @param text The source of the shader
		/// @param type The type of the shader
		/// @param ret The id of the created subshader is returned
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        static fm::Result compileSubShader(const std::string &text,unsigned int type,unsigned int &ret);

		/////////////////////////////////////////////////////////////
		/// @brief Compile a shader from a file
		///
		/// If the returned value is not 0 it is a
		/// valid OpenGL shader and can be attached to a shader program
		/// and it should be deleted using glDeleteShader by the caller
		///
		/// This function is used internally and
		/// you should only call this if you know
		/// exactly what you are doing
		///
		/// @param file The name of the file that contains the source of the shader
		/// @param type The type of the shader
		/// @param ret The id of the created subshader is returned
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        static fm::Result compileSubShaderFromFile(const std::string &file,unsigned int type,unsigned int &ret);

		/////////////////////////////////////////////////////////////
		/// @brief Set the blending mode of the shader
		///
		/// @param mode The new blending mode
		///
		/////////////////////////////////////////////////////////////
		void setBlendMode(fg::BlendMode mode);

		/////////////////////////////////////////////////////////////
		/// @brief Check if instancing is available
		/// 
		/// @return True iff instancing is available
		/// 
		/////////////////////////////////////////////////////////////
		bool instancingAvailable() const;

		/////////////////////////////////////////////////////////////
		/// @brief Run a function on all uniforms
		/// 
		/// @param func The function to call
		/// 
		/////////////////////////////////////////////////////////////
		void forAllUniforms(fm::Delegate<void,std::string,UniformData> func) const;

		/////////////////////////////////////////////////////////////
		/// @brief Run a function on all attributes
		/// 
		/// @param func The function to call
		/// 
		/////////////////////////////////////////////////////////////
		void forAllAttribs(fm::Delegate<void,std::string,AttribData> func) const;
	};

}
#endif // FRONTIER_SHADER_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Shader.inl>
#endif
