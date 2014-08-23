#ifndef FRONTIER_SHADER_HPP_INCLUDED
#define FRONTIER_SHADER_HPP_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>

#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Config.hpp>

#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_SHADER
#include <vector>
#include <string>
#include <map>

namespace fm
{
	template<class posType,class clrType,class texPosType,class normType> 
	class vertex;
}

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Class used to handle OpenGL shader programs in language <a href="http://en.wikipedia.org/wiki/GLSL">GLSL</a>
	/// 
	/////////////////////////////////////////////////////////////
	class Shader : public fm::NonCopyable, public GlObject
	{
		/////////////////////////////////////////////////////////////
		/// @brief Class used to store data about uniform textures
		/// 
		/// Uniforms are "global" variables in GLSL   
		/// they can only be modified by the application (not from shader program)
		/// 
		/// This class is used iternally and you shouldn't worry about it
		/// 
		/////////////////////////////////////////////////////////////
	    class TexUniformData
	    {
	    public:
			int location; ///< The "id" of the uniform
			int slot;	  ///< The assigned texture slot to the uniform
			int act_id;   ///< The id the actually bound texture
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/// Again, this is iternal stuff
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
		unsigned int m_texCount; ///< The number of bound textures
		bool link(); ///< Iternal function used to link the compiled shaders to the shader program
		void init(); ///< Iternal function used at setup
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
        ~Shader();

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from files
		/// 
		/// If at least on of the files are unreadable or 
		/// contain an invalid shader source an error will 
		/// be prompted to fg_log and the shader program becomes invalid
		/// 
		/// @param files A pointer to the names of files to be compiled
		/// @param types A pointer to the types of shaders
		/// @param count The number of shaders in the shader program
		/// 
		/// @return True if no error occured
		/// 
		/////////////////////////////////////////////////////////////
        bool loadFromFiles(const std::string *files,const unsigned int *types,unsigned int count);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from files
		/// 
		/// If at least on of the two files are unreadable or 
		/// contain an invalid shader source an error will 
		/// be prompted to fg_log and the shader program becomes invalid
		/// 
		/// It is assumed that the first file contains the source
		/// of the vertex shader while the second one contains the source of 
		/// the fragment shader
		/// 
		/// @param vertexShaderFile Name of the file that contains the vertex shader source
		/// @param fragmentShaderFile Name of the file that contains the fragment shader source
		/// 
		/// @return True if no error occured
		/// 
		/////////////////////////////////////////////////////////////
        bool loadFromFiles(const std::string &vertexShaderFile,const std::string &fragmentShaderFile);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from sources stored in memory
		/// 
		/// If at least on of the sources contains an 
		/// invalid shader source an error will 
		/// be prompted to fg_log and the shader program becomes invalid
		/// 
		/// @param data A pointer to the sources to be compiled
		/// @param types A pointer to the types of shaders
		/// @param count The number of shaders in the shader program
		/// 
		/// @return True if no error occured
		/// 
		/////////////////////////////////////////////////////////////
        bool loadFromMemory(const std::string *data,const unsigned int *types,unsigned int count);

		/////////////////////////////////////////////////////////////
		/// @brief Load the shader program from sources stored in memory
		/// 
		/// If at least on of the sources contains an 
		/// invalid shader source an error will 
		/// be prompted to fg_log and the shader program becomes invalid
		/// 
		/// It is assumed that the first string contains the source
		/// of the vertex shader while the second one contains the source of 
		/// the fragment shader
		/// 
		/// @param vertexShaderData The source of vertex shader
		/// @param fragmentShaderData The source of fragment shader
		/// 
		/// @return True if no error occured
		/// 
		/////////////////////////////////////////////////////////////
        bool loadFromMemory(const std::string &vertexShaderData,const std::string &fragmentShaderData);

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
		/// If the shader program is invalid or @a name does not 
		/// correspond to an active uniform in the shader program 
		/// an error is prompted to fg_log and -1 is returned
		/// 
		/// This function is mostly used iternally
		/// 
		/// @param name The name of the uniform to find
		/// 
		/// @return -1 on error, the location otherwise
		/// 
		/////////////////////////////////////////////////////////////
        int getUniformLocation(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Find the "id" of an attribute 
		/// 
		/// If the shader program is invalid or @a name does not 
		/// correspond to an active attribute in the shader program 
		/// an error is prompted to fg_log and -1 is returned
		/// 
		/// This function is mostly used iternally
		/// 
		/// @param name The name of the attribute to find
		/// 
		/// @return -1 on error, the location otherwise
		/// 
		/////////////////////////////////////////////////////////////
        int getAttribLocation(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with an attribute
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a posName does not correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param posName The name of the position attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
        reference setAttribPointer(const std::string &posName,const fm::vertex<pt,ct,tpt,nt> *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with two attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if either of @a posName or @a clrName does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param posName The name of the position attribute
		/// @param clrName The name of the color attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
        reference setAttribPointer(const std::string &posName,const std::string &clrName,const fm::vertex<pt,ct,tpt,nt> *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with three attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if either of @a posName or @a clrName or @a texPosName does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param posName The name of the position attribute
		/// @param clrName The name of the color attribute
		/// @param texPosName The name of the texture position attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
        reference setAttribPointer(const std::string &posName,const std::string &clrName,const std::string &texPosName,const fm::vertex<pt,ct,tpt,nt> *pointer);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with four attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if either of @a posName or @a clrName or @a texPosName or @a normName does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param posName The name of the position attribute
		/// @param clrName The name of the color attribute
		/// @param texPosName The name of the texture position attribute
		/// @param normName The name os the normal attribute
		/// @param pointer A pointer to the data stored in fm::vertex
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        template<class pt,class ct,class tpt,class nt>
        reference setAttribPointer(const std::string &posName,const std::string &clrName,const std::string &texPosName,const std::string &normName,const fm::vertex<pt,ct,tpt,nt> *pointer);

		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// This function is mostly used iternally
		/// 
		/// @param name The name of the attribute
		/// @param dimensions The number of components in the attribute
		/// @param type The type of data component (e.g. GL_FLOAT)
		/// @param normalize Specify if the data should be normalized when accessed
		/// @param pointer A pointer to the data
		/// @param stride Byte offset between the beginning of two attributes
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        reference setAttribPointer(const std::string &name,unsigned int dimensions,unsigned long type,bool normalize,const void *pointer,unsigned int stride=0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec2 attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fm::vec2
		/// @param stride Byte offset between the beginning of two attributes
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        reference setAttribPointer(const std::string &name,const fm::vec2 *pointer,unsigned int stride=0);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec3 attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fm::vec3
		/// @param stride Byte offset between the beginning of two attributes
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        reference setAttribPointer(const std::string &name,const fm::vec3 *pointer,unsigned int stride=0);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the pointer data associated with one vec4 attributes
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active attribute in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// After successfully calling this function
		/// a call to glDrawArrays, glDrawElements or fg::draw family
		/// with this shader program being bound will use
		/// this data
		/// 
		/// @param name The name of the attribute
		/// @param pointer A pointer to the data stored in fm::vec4
		/// @param stride Byte offset between the beginning of two attributes
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
        reference setAttribPointer(const std::string &name,const fm::vec4 *pointer,unsigned int stride=0);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a float uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,float x);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a vec2 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,float x,float y);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a vec3 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,float x,float y,float z);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of a vec4 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		/// @param w The w value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,float x,float y,float z,float w);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an int uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,int x);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an ivec2 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,int x,int y);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an ivec3 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,int x,int y,int z);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an ivec4 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param x The x value of the uniform
		/// @param y The y value of the uniform
		/// @param z The z value of the uniform
		/// @param w The w value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,int x,int y,int z,int w);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec2 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const fm::vec2 &v);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec3 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const fm::vec3 &v);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an vec4 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param v The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const fm::vec4 &v);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat3 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const fm::mat3 &m);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an mat4 uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param m The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const fm::mat4 &m);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an sampler2D uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param tex The value of the uniform
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const Texture &tex);
        
		/////////////////////////////////////////////////////////////
		/// @brief Set the value of an sampler2D uniform
		/// 
		/// If the shader program is invalid no error will be prompted
		/// and the shader program will not be modified
		///
		/// if @a name does not 
		/// correspond to an active uniform in 
		/// the shader program an error is prompted to 
		/// fg_log and -1 is returned
		/// 
		/// @param name The name of the uniform
		/// @param tex The value of the uniform (NULL means unbound)
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setUniform(const std::string &name,const Texture *tex);

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
		/// If the shader couldn't be created (glCreateShader failed)
		/// or the source couldn't be compiled then an error is prompted
		/// to fg_log and 0 is returned
		/// 
		/// If the returned value is not 0 it is a 
		/// valid OpenGL shader and can be attached to a shader program
		/// and it should be deleted using glDeleteShader by the caller
		/// 
		/// This function is used iternally and
		/// you should only call this if you know
		/// exactly what you are doing
		/// 
		/// @param text The source of the shader
		/// @param type The type of the shader
		/// 
		/// @return The id of the created shader (0 on error)
		/// 
		/////////////////////////////////////////////////////////////
        static unsigned int compileSubShader(const std::string &text,unsigned int type);

		/////////////////////////////////////////////////////////////
		/// @brief Compile a shader from a file
		/// 
		/// If the shader couldn't be created (glCreateShader failed)
		/// or the source in the file couldn't be compiled then an error is prompted
		/// to fg_log and 0 is returned
		/// 
		/// If the returned value is not 0 it is a 
		/// valid OpenGL shader and can be attached to a shader program
		/// and it should be deleted using glDeleteShader by the caller
		/// 
		/// This function is used iternally and
		/// you should only call this if you know
		/// exactly what you are doing
		/// 
		/// @param file The name of the file that contains the source of the shader
		/// @param type The type of the shader
		/// 
		/// @return The id of the created shader (0 on error)
		/// 
		/////////////////////////////////////////////////////////////
        static unsigned int compileSubShaderFromFile(const std::string &file,unsigned int type);

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