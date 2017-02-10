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
#include <FRONTIER/Graphics/CubeTexture.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/Graphics/Color.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <fstream>

namespace fg
{
	namespace priv
	{
		std::string getShaderTypeName(GLenum type)
		{
			if (type == GL_VERTEX_SHADER)          return "VertexShader";
			if (type == GL_GEOMETRY_SHADER)        return "GeometryShader";
			if (type == GL_FRAGMENT_SHADER)        return "FragmentShader";
			// if (type == GL_COMPUTE_SHADER)         return "ComputeShader";
			if (type == GL_TESS_CONTROL_SHADER)    return "TessControlShader";
			if (type == GL_TESS_EVALUATION_SHADER) return "TessEvalShader";
			
			return "UnknownShader";
		}
	}
	
	/// compile shader objects /////////////////////////////////////////////////////////
    fm::Result Shader::compileSubShader(const std::string &text,unsigned int type,unsigned int &ret)
    {
		// ask GL for an id
        unsigned int s = glCreateShader(type);
		if (!s)
			return fm::Result("GLError",fm::Result::OPFailed,"InitFailed","glCreateShader",__FILE__,__LINE__,fm::toString(type).str(),priv::getShaderTypeName(type));
		
		// make text acceptable for gl
		const char *c_text = text.c_str();
		
		fm::Result res;
		
		// send shader source to GL
		res += glCheck(glShaderSource(s,1,&c_text,NULL));
		res += glCheck(glCompileShader(s));

		// check compilation result
		int success;
		res += glCheck(glGetShaderiv(s, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			// get needed size for error log
			int logSize;
			res += glCheck(glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logSize));

			// allocate memory for log
			char *logData;
			logData = new char[logSize];

			// retrieve error log
			res += glCheck(glGetShaderInfoLog(s, logSize, NULL, &logData[0]));
			
			res += fm::Result("GLSLError",fm::Result::OPFailed,"CompileFailed","compileSubShader",__FILE__,__LINE__,fm::toString(type).str(),logData);
			ret = 0;
			
			// free allocated memory
			delete[] logData;
			return res;
		}
		ret = s;
		
		return res;
    }


	////////////////////////////////////////////////////////////
    fm::Result Shader::compileSubShaderFromFile(const std::string &file,unsigned int type,unsigned int &ret)
    {
		// open file
        std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
		if (in)
		{
			// read whole file
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();

			// compile
			return compileSubShader(contents,type,ret);
		}
		
		return fm::Result("IOError",fm::Result::OPFailed,"FileNotFound","compileSubShaderFromFile",__FILE__,__LINE__,file);
    }

    ////////////////////////////////////////////////////////////
	void Shader::init()
	{
		if (!getGlId())
			getGlId() = glCreateProgram();
	}

    ////////////////////////////////////////////////////////////
	fm::Result Shader::link()
	{
		// init GlId
		init();
		if (!getGlId())
			return fm::Result("GLError",fm::Result::OPFailed,"InitFailed","link",__FILE__,__LINE__);

		// reset containers
		clearData();
		
		fm::Result res;
		
		// attach subshaders
		C(m_subShaders.size())
			res += glCheck(glAttachShader(getGlId(),m_subShaders[i]));

		// check for compilation errors
		int success;
		res += glCheck(glLinkProgram(getGlId()));
		res += glCheck(glGetProgramiv(getGlId(), GL_LINK_STATUS, &success));
		if (success)
		{
			// on success validate status
			res += glCheck(glValidateProgram(getGlId()));
			res += glCheck(glGetProgramiv(getGlId(), GL_VALIDATE_STATUS, &success));
		}

		// on error print description
		if (!success)
		{
			// get needed size for error log
			int logSize;
			res += glCheck(glGetProgramiv(getGlId(), GL_INFO_LOG_LENGTH, &logSize));

			// allocate memory for log
			char *logData;
			logData = new char[logSize];

			// retrieve error log
			res += glCheck(glGetProgramInfoLog(getGlId(), logSize, NULL, &logData[0]));
			res += fm::Result("GLSLError",fm::Result::OPFailed,"LinkFailed","link",__FILE__,__LINE__,logData);

			// free allocated memory
			delete[] logData;
			return res;
		}
    	
    	// generate default vao if available
		if (::priv::so_loader.getProcAddr("glGenVertexArrays"))
		{
			if (glIsVertexArray(m_defVao))
				glDeleteVertexArrays(1,&m_defVao);
			
			GLint program;
			res += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			res += glCheck(glUseProgram(getGlId()));

			glGenVertexArrays(1, &m_defVao);
    		glBindVertexArray(m_defVao);

			res += glCheck(glUseProgram(program));
		}

		return res;
	}

	////////////////////////////////////////////////////////////
	void Shader::clearData()
	{
		m_texCounter = 0;
		m_textures.clear();
	    m_attribs.clear();
		m_uniforms.clear();
	}

	/// constructor /////////////////////////////////////////////////////////
	Shader::Shader() : m_texCounter(0),
					   m_blendMode(fg::Alpha)
	{

	}


	////////////////////////////////////////////////////////////
	Shader::~Shader()
	{
		if (getGlId() && glIsProgram(getGlId()))
		{
			freeSubShaders();
			glDeleteProgram(getGlId());
			
			if (::priv::so_loader.getProcAddr("glGenVertexArrays"))
			{
				if (glIsVertexArray(m_defVao))
					glDeleteVertexArrays(1,&m_defVao);
			}
		}
	}

	/////////////////////////////////////////////////////////////
	Shader::Shader(Shader &&shader)
	{
		shader.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	Shader &Shader::operator=(Shader &&shader)
	{
		return this->swap(shader);
	}

	/////////////////////////////////////////////////////////////
	Shader &Shader::swap(Shader &shader)
	{
		m_subShaders.swap(shader.m_subShaders);
		m_uniforms.swap(shader.m_uniforms);
		m_textures.swap(shader.m_textures);
		m_attribs.swap(shader.m_attribs);
		
		std::swap(m_texCounter,shader.m_texCounter);
		std::swap(m_defVao,shader.m_defVao);
		
		std::swap(getGlId(),shader.getGlId());
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::freeSubShaders()
	{
		fm::Result res;
		
		C(m_subShaders.size())
			if (glIsShader(m_subShaders[i]))
			{
				res += glCheck(glDetachShader(getGlId(),m_subShaders[i]));
				res += glCheck(glDeleteShader(m_subShaders[i]));
			}
		
		return res;
	}

	/// functions /////////////////////////////////////////////////////////
	fm::Result Shader::loadFromFiles(const std::string *files,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
			return fm::Result("GLError",fm::Result::OPFailed,"InitFailed","link",__FILE__,__LINE__);

		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(count);
		
		fm::Result res;
		C(count)
		{
			res += compileSubShaderFromFile(files[i],types[i],m_subShaders[i]);
		}
		
		if (!res) return res;
		
		return link();
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromMemory(const std::string *data,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
			return fm::Result("GLError",fm::Result::OPFailed,"InitFailed","link",__FILE__,__LINE__);
		
		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(count);
		
		fm::Result res;
		C(count)
		{
			res += compileSubShader(data[i],types[i],m_subShaders[i]);
		}
		
		if (!res) return res;
		
		return link();
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromFiles(const std::string &vertexShaderFile,const std::string &fragmentShaderFile)
	{
		std::string files[] = {vertexShaderFile,fragmentShaderFile};
		unsigned int types[] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};

		return loadFromFiles(files,types,2);
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromFiles(const std::string &vertexShaderFile,const std::string &geometryShaderFile,const std::string &fragmentShaderFile)
	{
		std::string files[] = {vertexShaderFile,geometryShaderFile,fragmentShaderFile};
		unsigned int types[] = {GL_VERTEX_SHADER,GL_GEOMETRY_SHADER,GL_FRAGMENT_SHADER};

		return loadFromFiles(files,types,3);
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromMemory(const std::string &vertexShaderData,const std::string &fragmentShaderData)
	{
		std::string data[] = {vertexShaderData,fragmentShaderData};
		unsigned int types[] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};

		return loadFromMemory(data,types,2);
	}


	////////////////////////////////////////////////////////////
	void Shader::bind() const
	{
		glUseProgram(getGlId());
	}


	////////////////////////////////////////////////////////////
	void Shader::bind(fm::Ref<const Shader> program)
	{
		const Shader *ptr = program;
		
		if (ptr)
			ptr->bind();
		else 
			glUseProgram(0);
	}

	////////////////////////////////////////////////////////////
	int Shader::getUniformLocation(const std::string &name)
	{
		if (!getGlId())
			return -1;

		int location = hasUniform(name)-1;

		return location;
	}

	////////////////////////////////////////////////////////////
	int Shader::hasUniform(const std::string &name)
	{
		if (!getGlId())
			return 0;

		// check if already in dictionary
		std::map<std::string, int>::const_iterator it = m_uniforms.find(name);
		if (it != m_uniforms.end())
			return it->second+1;

		// get location from GL
		int location = glGetUniformLocation(getGlId(),name.c_str());

		// insert into dictionary
		if (location != -1)
			m_uniforms.insert(std::make_pair(name, location));

		// on error location is -1
		return location + 1;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Shader::enableAttribPointer(const std::string &name,bool enable)
	{
		int location = getAttribLocation(name);

		if (location == -1)
			return fm::Result("GLError",fm::Result::OPFailed,"AttrNotFound","enableAttribPointer",__FILE__,__LINE__,name);

		if (enable)
			return glCheck(glEnableVertexAttribArray(location));
		else
			return glCheck(glDisableVertexAttribArray(location));
	}


	////////////////////////////////////////////////////////////
	int Shader::getAttribLocation(const std::string &name)
	{
		if (!getGlId())
			return -1;

		int location = hasAttribute(name)-1;

		return location;
	}

	////////////////////////////////////////////////////////////
	int Shader::hasAttribute(const std::string &name)
	{
		if (!getGlId())
			return 0;

		// check if already in dictionary
		std::map<std::string, int>::const_iterator it = m_attribs.find(name);
		if (it != m_attribs.end())
			return it->second+1;

		// get location from GL
		int location = glGetAttribLocation(getGlId(),name.c_str());

		// insert into dictionary
		if (location != -1)
			m_attribs.insert(std::make_pair(name, location));

		// on error location is -1
		return location + 1;
	}

	#define CREATE_SET_UNIFORM(argList,callFunc)                                          \
	fm::Result Shader::setUniform argList                                                 \
	{                                                                                     \
		fm::Result res;                                                                   \
		if (getGlId())                                                                    \
		{                                                                                 \
			GLint program;                                                                \
			res += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));                   \
			if ((GLuint)program != getGlId())                                             \
				res += glCheck(glUseProgram(getGlId()));                                  \
                                                                                          \
			int location = getUniformLocation(name);                                      \
			if (location != -1)                                                           \
				res += glCheck(callFunc);                                                 \
			else                                                                          \
				res += fm::Result("GLSLError",fm::Result::OPFailed,                       \
								  "UniformNotFound","setUniform",__FILE__,__LINE__,name); \
                                                                                          \
			if ((GLuint)program != getGlId())                                             \
				res += glCheck(glUseProgram(program));                                    \
		}                                                                                 \
		return res;                                                                       \
	}

	CREATE_SET_UNIFORM((const std::string &name,int x),glUniform1i(location,x))
	CREATE_SET_UNIFORM((const std::string &name,float x),glUniform1f(location,x))

	CREATE_SET_UNIFORM((const std::string &name,const fm::vec2 &v),glUniform2f(location,v.x,v.y))
	CREATE_SET_UNIFORM((const std::string &name,const fm::vec3 &v),glUniform3f(location,v.x,v.y,v.z))
	CREATE_SET_UNIFORM((const std::string &name,const fm::vec4 &v),glUniform4f(location,v.x,v.y,v.z,v.w))

	CREATE_SET_UNIFORM((const std::string &name,const fg::Color &c),glUniform4f(location,c.r/255.f,c.g/255.f,c.b/255.f,c.a/255.f))
	
	CREATE_SET_UNIFORM((const std::string &name,const fm::mat2 &m),glUniformMatrix2fv(location, 1, GL_FALSE, &(m.transpose())[0][0]))
	CREATE_SET_UNIFORM((const std::string &name,const fm::mat3 &m),glUniformMatrix3fv(location, 1, GL_FALSE, &(m.transpose())[0][0]))
	CREATE_SET_UNIFORM((const std::string &name,const fm::mat4 &m),glUniformMatrix4fv(location, 1, GL_FALSE, &(m.transpose())[0][0]))

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,fm::Ref<const Texture> tex)
	{
		fm::Result res;
		
		if (getGlId())
		{
			GLint program;
			res += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			res += glCheck(glUseProgram(getGlId()));

			// check if the uniform is in the dictionary
			std::map<std::string, TexUniformData >::iterator it = m_textures.find(name);
			if (it != m_textures.end())
			{
				// activate slot
				res += glCheck(glActiveTexture(GL_TEXTURE0+it->second.slot));
				res += glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
				it->second.act_id = tex ? tex->getGlId() : 0;
			}
			else
			{
				// get uniform location
				int location = glGetUniformLocation(getGlId(),name.c_str());

				if (location != -1)
				{
					// add to dictionary
					m_textures.insert(std::make_pair(name, TexUniformData(location,m_texCounter,tex ? tex->getGlId() : 0)));

					// assign id to texture slot
					res += glCheck(glUniform1i(location, m_texCounter));

					// activate slot
					res += glCheck(glActiveTexture(GL_TEXTURE0+m_texCounter));
					res += glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
					m_texCounter++;
				}
				else
					res += fm::Result("GLSLError",fm::Result::OPFailed,"UniformNotFound","setUniform",__FILE__,__LINE__,name);
			}
			
			res += glCheck(glActiveTexture(GL_TEXTURE0));
			res += glCheck(glUseProgram(program));
		}
		
		return res;
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,fm::Ref<const CubeTexture> tex)
	{
		fm::Result res;
			
		if (getGlId())
		{
			GLint program;
			res += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			res += glCheck(glUseProgram(getGlId()));

			// check if the uniform is in the dictionary
			std::map<std::string, TexUniformData >::iterator it = m_textures.find(name);
			if (it != m_textures.end())
			{
				// activate slot
				res += glCheck(glActiveTexture(GL_TEXTURE0+it->second.slot));
				res += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,tex ? tex->getGlId() : 0));
				it->second.act_id = tex ? tex->getGlId() : 0;
			}
			else
			{
				// get uniform location
				int location = glGetUniformLocation(getGlId(),name.c_str());

				if (location != -1)
				{
					// add to dictionary
					m_textures.insert(std::make_pair(name, TexUniformData(location,m_texCounter,tex ? tex->getGlId() : 0)));

					// assign id to texture slot
					res += glCheck(glUniform1i(location, m_texCounter));

					// activate slot
					res += glCheck(glActiveTexture(GL_TEXTURE0+m_texCounter));
					res += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,tex ? tex->getGlId() : 0));
					m_texCounter++;
				}
				else
					res += fm::Result("GLSLError",fm::Result::OPFailed,"UniformNotFound","setUniform",__FILE__,__LINE__,name);

				res += glCheck(glActiveTexture(GL_TEXTURE0));
				res += glCheck(glUseProgram(program));
			}
		}
		
		return res;
	}

	////////////////////////////////////////////////////////////
    fm::Result Shader::setAttribPointer(const std::string &name,unsigned int components,unsigned long type,bool normalize,const void *pointer,unsigned int stride)
    {
		fm::Result res;
		
		if (getGlId())
		{
			int location = getAttribLocation(name);

			if (location != -1)
			{
				enableAttribPointer(name,true);
				res += glCheck(glVertexAttribPointer(location,components,type,normalize,stride,pointer));
			}
		}
		return res;
    }

	////////////////////////////////////////////////////////////
    bool Shader::isAvailable()
    {
		const char *ptr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		
		if (!ptr) return false;
		
		return (glGetError() == GL_NO_ERROR && *ptr);
    }

	/////////////////////////////////////////////////////////////
	void Shader::setBlendMode(fg::BlendMode mode)
	{
		m_blendMode = mode;
		
		if (mode == fg::Overwrite)
			glDisable(GL_BLEND);
			
		if (mode == fg::Additive)
			glEnable(GL_BLEND),
			glBlendFunc(GL_ONE,GL_ONE);
			
		if (mode == fg::Alpha)
			glEnable(GL_BLEND),
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
