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

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/type_traits/Enable_if.hpp>
#include <FRONTIER/System/StorageOrder.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>

#define FRONTIER_SHADER
#include <vector>
#include <string>
#include <map>

namespace fm
{
	template<class,class,class,class>
	class vertex;

	template<class> class vector2;
	template<class> class vector3;
	template<class> class vector4;

	typedef vector2<float> vec2;
	typedef vector3<float> vec3;
	typedef vector4<float> vec4;

	template<fm::Size W,fm::Size H,class T>
	class matrix;

	typedef matrix<3,3,float> mat3;
	typedef matrix<4,4,float> mat4;
}

namespace fg
{
	class CubeTexture;
	class Texture;
	class Color;

	/////////////////////////////////////////////////////////////
	///
	/// 	@brief Class used to handle OpenGL shader programs in language <a href="http://en.wikipedia.org/wiki/GLSL">GLSL</a>
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Shader : public fm::NonCopyable, public GlObject
	{
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

		std::vector<unsigned int> m_subShaders;  ///< The id of the shaders that builds up the shader program
		std::map<std::string, int> m_uniforms;   ///< The "id" of the uniforms and their name
		std::map<std::string, int> m_attribs;    ///< The "id" of the attributes and their name
		std::map<std::string, TexUniformData > m_textures; ///< The state and name of the texture uniforms
		fm::Size m_texCounter; ///< The counter used when activating texture slots
		fm::Uint32 m_defVao;   ///< Default vao for core profiles
		fm::Result link(); ///< Internal function used to link the compiled shaders to the shader program
		void init(); ///< Internal function used at setup
		void freeSubShaders(); ///< Internal function used at clean-up
		virtual void clearData(); ///< clear uniforms, attributes, textures etc

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
		/////////////////////////////////////////////////////////////
		void enableAttribPointer(const std::string &name,bool enable = true);

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
		/// @brief Set the pointer data associated with two attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param posName The name of the position attribute
		/// @param clrName The name of the color attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
		fm::Result setAttribPointer(const std::string &posName,
								    const std::string &clrName,const fm::vertex<pt,ct,tpt,nt> *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with three attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param posName The name of the position attribute
		/// @param clrName The name of the color attribute
		/// @param texPosName The name of the texture position attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
		fm::Result setAttribPointer(const std::string &posName,
								    const std::string &clrName,
								    const std::string &texPosName,const fm::vertex<pt,ct,tpt,nt> *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with four attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param posName The name of the position attribute
		/// @param clrName The name of the color attribute
		/// @param texPosName The name of the texture position attribute
		/// @param normName The name os the normal attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
		fm::Result setAttribPointer(const std::string &posName,
								    const std::string &clrName,
								    const std::string &texPosName,
								    const std::string &normName,const fm::vertex<pt,ct,tpt,nt> *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one attributes
		///
		/// If the shader program is invalid no error will be prompted
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
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setAttribPointer(const std::string &name,const T *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec2 attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fm::vec2
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setAttribPointer(const std::string &name,const fm::vector2<T> *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec3 attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fm::vec3
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setAttribPointer(const std::string &name,const fm::vector3<T> *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec4 attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fm::vec4
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setAttribPointer(const std::string &name,const fm::vector4<T> *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec4 attributes
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// After successfully calling this function
		/// a call to glDrawArrays or glDrawElements
		/// with this shader program being bound will use
		/// this data
		///
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fg::Color
		/// @param stride Byte offset between the beginning of two attributes (0 means tightly packed)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setAttribPointer(const std::string &name,const fg::Color *pointer,unsigned int stride = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a float uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,float x);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a vec2 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,float x,float y);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a vec3 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,float x,float y,float z);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a vec4 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		/// @param w The w value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,float x,float y,float z,float w);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an int uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,int x);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an ivec2 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,int x,int y);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an ivec3 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,int x,int y,int z);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an ivec4 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		/// @param w The w value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,int x,int y,int z,int w);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec2 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
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
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
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
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::vec4 &v);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat3 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<fm::MATRIX::StorageOrder storeOrder>
		fm::Result setUniform(const std::string &name,const fm::mat3 &m);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat3 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		/// @param storeOrder If RowMajor then the matrix will be transposed before sending
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::mat3 &m,fm::MATRIX::StorageOrder storeOrder = fm::MATRIX::RowMajor);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat4 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<fm::MATRIX::StorageOrder storeOrder>
		fm::Result setUniform(const std::string &name,const fm::mat4 &m);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat4 uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		/// @param storeOrder If RowMajor then the matrix will be transposed before sending
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const fm::mat4 &m,fm::MATRIX::StorageOrder storeOrder = fm::MATRIX::RowMajor);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of color uniform (vec4)
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
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
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
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
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform (NULL means unbound)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const Texture *tex);

		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a samplerCube uniform
		///
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
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
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not
		/// correspond to an active uniform in
		/// the shader program an error is prompted to
		/// fg_log
		///
		/// @param name The name of the uniform
		/// @param tex The value of the uniform (NULL means unbound)
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setUniform(const std::string &name,const CubeTexture *tex);

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
        static void bind(const Shader *program);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a shader program for usage
		///
		/// After this, a call to glDrawArrays,
		/// glDrawElements or fg::draw
		/// will use this shader program
		///
		/// @param program The program to be bound
		///
		/////////////////////////////////////////////////////////////
        static void bind(const Shader &program);

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

	};

}
#endif // FRONTIER_SHADER_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Shader.inl>
#endif

////////////////////////////////////////////////////////////
/// @class fg::Shader
/// @ingroup Graphics
///
/// fg::Shader class can be used to create and handle OpenGL shader programs
/// and offers related subroutines (such as compileSubShader)
///
/// Sample vertex shader (vert.glsl):
/// @code
/// #version 130
///
/// attribute vec2 in_pos;
///
/// varying vec2 va_pos;
///
/// void main()
/// {
/// 	gl_Position = vec4(in_pos,0.0,1.0);
/// 	va_pos = in_pos;
/// 	if (va_pos.x<0.0) va_pos.x = va_pos.x*-1.0;
/// 	if (va_pos.y<0.0) va_pos.y = va_pos.y*-1.0;
/// }
/// @endcode
///
///
/// Sample fragment shader (frag.glsl):
/// @code
/// #version 130
///
/// uniform float u_blue;
///
/// varying vec2 va_pos;
///
/// void main()
/// {
/// 	gl_FragColor = vec4(va_pos,u_blue,1.0);
/// }
/// @endcode
///
/// Usage example:
/// @code
///
/// fg::Shader shader;
/// shader.loadFromFiles("vert.glsl","frag.glsl");
/// shader.setUniform("u_blue",1.0);
///
/// fm::vec2 verts[]={fm::vec2(-.5,-.5),
/// 				  fm::vec2( .5,-.5),
/// 				  fm::vec2(-.5, .5),
/// 				  fm::vec2( .5, .5)};
///
/// for (std::size_t loop=0;true;loop++)
/// {
/// 	if (windowIsClosed)
/// 		break;
///
/// 	float sinVal = std::sin(fm::deg(loop));
/// 	sinVal = sinVal*.5+.5 // clamp the sin val to [0;1]
///
/// 	shader.setUniform("u_blue",sinVal);
/// 	shader.bind();
///
/// 	shader.setAttribPointer("in_pos",verts);
/// 	glClear(GL_COLOR_BUFFER_BIT);
/// 	glDrawArrays(fg::TriangleStrip,0,4);
/// 	displayWindow()
/// }
/// return 0;
///
///
/// @endcode
///
/// @see fg::FrameBuffer
/// @see fg::Texture
///
////////////////////////////////////////////////////////////
