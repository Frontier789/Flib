////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <fstream>
 
namespace fg
{
	/// compile shader objects /////////////////////////////////////////////////////////
    unsigned int Shader::compileSubShader(const std::string &text,unsigned int type)
    {
		// ask GL for an id
        unsigned int s = glCreateShader(type);
		if (!s)
		{
			fg_log<<"Couldn't create shader type "<<type<<std::endl;
			return 0;
		}
		
		// make text acceptable for gl
		const char *c_text = text.c_str();
		
		// send shader source to GL
		glCheck(glShaderSource(s,1,&c_text,NULL));
		glCheck(glCompileShader(s));
		
		// check compilation result
		int success;
		glCheck(glGetShaderiv(s, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			// get needed size for error log
			int logSize;
			glCheck(glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logSize));
			
			// allocate memory for log
			char *logData;
			logData = new char[logSize];
			
			// retrieve error log
			glCheck(glGetShaderInfoLog(s, logSize, NULL, &logData[0]));
			fg_log<<"Error compiling shader type "<<type<<" : "<<logData<<std::endl;
			
			// free allocated memory
			delete[] logData;
			return 0;
		}
		return s;
    }


	////////////////////////////////////////////////////////////
    unsigned int Shader::compileSubShaderFromFile(const std::string &file,unsigned int type)
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
			return compileSubShader(contents,type);
		}
		fg_log<<"file "<<file<<" couldn't be read"<<std::endl;
		return 0;
    }
    
    ////////////////////////////////////////////////////////////
	void Shader::init()
	{
		if (!getGlId())
		{
			getGlId() = glCreateProgram();
			if (!getGlId())
				fg_log<<"couldn't create shader program"<<std::endl;			
		}
	}
    
    ////////////////////////////////////////////////////////////
	bool Shader::link()
	{
		// init GlId
		init();
		if (!getGlId())
			return false;
		
		// reset containers
		m_texCounter = 0;
		m_textures.clear();
	    m_attribs.clear();
		m_uniforms.clear();
		C(m_subShaders.size())
			glCheck(glAttachShader(getGlId(),m_subShaders[i]));
		
		// check for compilation errors
		int success;
		glCheck(glLinkProgram(getGlId()));
		glCheck(glGetProgramiv(getGlId(), GL_LINK_STATUS, &success));
		if (success)
		{
			// on success validate status
			glCheck(glValidateProgram(getGlId()));
			glCheck(glGetProgramiv(getGlId(), GL_VALIDATE_STATUS, &success));
		}
		
		// on error print description
		if (!success)
		{
			// get needed size for error log
			int logSize;
			glCheck(glGetProgramiv(getGlId(), GL_INFO_LOG_LENGTH, &logSize));
			
			// allocate memory for log
			char *logData;
			logData = new char[logSize];
			
			// retrieve error log
			glCheck(glGetProgramInfoLog(getGlId(), logSize, NULL, &logData[0]));
			fg_log<<"Error compiling shader program: "<<logData<<std::endl;
			
			// free allocated memory
			delete[] logData;
			return false;
		}
		
		return true;
	}

	/// constructor /////////////////////////////////////////////////////////
	Shader::Shader() : m_texCounter(0)
	{
		
	}


	/// destructor /////////////////////////////////////////////////////////
	Shader::~Shader()
	{
		if (glIsProgram(getGlId()))
		{
			freeSubShaders();
			glCheck(glDeleteProgram(getGlId()));
		}
	}
	
	////////////////////////////////////////////////////////////
	void Shader::freeSubShaders()
	{
		C(m_subShaders.size())
			if (glIsShader(m_subShaders[i]))
				glCheck(glDetachShader(getGlId(),m_subShaders[i])),
				glCheck(glDeleteShader(m_subShaders[i]));
	}

	/// functions /////////////////////////////////////////////////////////
	bool Shader::loadFromFiles(const std::string *files,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
		{
			fg_log<<"couldn't create shader program"<<std::endl;
			return false;
		}
		
		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(count);
		C(count)
		{
			m_subShaders[i] = compileSubShaderFromFile(files[i],types[i]);
			if (!m_subShaders[i])
				return false;
		}
			
		
		return link();
	}


	////////////////////////////////////////////////////////////
	bool Shader::loadFromMemory(const std::string *data,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
		{
			fg_log<<"couldn't create shader program"<<std::endl;
			return false;
		}
		
		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(count);
		C(count)
		{
			m_subShaders[i] = compileSubShader(data[i],types[i]);
			if (!m_subShaders[i])
				return false;
		}
		
		return link();
	}


	////////////////////////////////////////////////////////////
	bool Shader::loadFromFiles(const std::string &vertexShaderFile,const std::string &fragmentShaderFile)
	{
		std::string files[] = {vertexShaderFile,fragmentShaderFile};
		unsigned int types[] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};
		
		return loadFromFiles(files,types,2);
	}


	////////////////////////////////////////////////////////////
	bool Shader::loadFromMemory(const std::string &vertexShaderFile,const std::string &fragmentShaderFile)
	{
		std::string files[] = {vertexShaderFile,fragmentShaderFile};
		unsigned int types[] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};
		
		return loadFromMemory(files,types,2);
	}


	////////////////////////////////////////////////////////////
	void Shader::bind() const
	{
		glCheck(glUseProgram(getGlId()));
	}


	////////////////////////////////////////////////////////////
	void Shader::bind(const Shader *program)
	{
		if (program)
			program->bind();
		else glCheck(glUseProgram(0));
	}


	////////////////////////////////////////////////////////////
	void Shader::bind(const Shader &program)
	{
        program.bind();
	}

	////////////////////////////////////////////////////////////
	int Shader::getUniformLocation(const std::string &name)
	{
		if (!getGlId())
		{
			fg_log << "Error: trying to get location of \""<<name<<"\" uniform from unitialized shader" << std::endl;
			return -1;
		}

		int location = hasUniform(name)-1;
		
		// check for error
		if (location == -1)
			fg_log << "Couldn't find uniform "<<name<<" in shader."<<std::endl;

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
		return location+1;
	}
	
	/////////////////////////////////////////////////////////////
	void Shader::enableAttribPointer(const std::string &name,bool enable)
	{
		int location = getAttribLocation(name);
		
		if (location == -1)
			return;
			
		if (enable)
			glCheck(glEnableVertexAttribArray(location));
		else
			glCheck(glDisableVertexAttribArray(location));
	}


	////////////////////////////////////////////////////////////
	int Shader::getAttribLocation(const std::string &name)
	{
		if (!getGlId())
		{
			fg_log << "Error: trying to get location of \""<<name<<"\" attribute from unitialized shader" << std::endl;
			return -1;
		}

		int location = hasAttribute(name)-1;
		
		// check for error
		if (location == -1)
			fg_log << "Couldn't find attribute "<<name<<" in shader."<<std::endl;

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
		return location+1;
	}

	#define CREATE_SET_UNIFORM(argList,callFunc)                       \
	Shader::reference Shader::setUniform argList                       \
	{                                                                  \
		if (getGlId())                                                 \
		{                                                              \
			int program;                                               \
			glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));       \
			glCheck(glUseProgram(getGlId()));                          \
                                                                       \
			int location = getUniformLocation(name);                   \
			if (location != -1)                                        \
				glCheck(callFunc);                                     \
                                                                       \
			glCheck(glUseProgram(program));                            \
		}                                                              \
		return *this;                                                  \
	}
	
	CREATE_SET_UNIFORM((const std::string &name,float x),glUniform1f(location,x))
	CREATE_SET_UNIFORM((const std::string &name,float x,float y),glUniform2f(location,x,y))
	CREATE_SET_UNIFORM((const std::string &name,float x,float y,float z),glUniform3f(location,x,y,z))
	CREATE_SET_UNIFORM((const std::string &name,float x,float y,float z,float w),glUniform4f(location,x,y,z,w))
	
	CREATE_SET_UNIFORM((const std::string &name,int x),glUniform1i(location,x))
	CREATE_SET_UNIFORM((const std::string &name,int x,int y),glUniform2i(location,x,y))
	CREATE_SET_UNIFORM((const std::string &name,int x,int y,int z),glUniform3i(location,x,y,z))
	CREATE_SET_UNIFORM((const std::string &name,int x,int y,int z,int w),glUniform4i(location,x,y,z,w))
	
	CREATE_SET_UNIFORM((const std::string &name,const fm::vec2 &v),glUniform2f(location,v.x,v.y))
	CREATE_SET_UNIFORM((const std::string &name,const fm::vec3 &v),glUniform3f(location,v.x,v.y,v.z))
	CREATE_SET_UNIFORM((const std::string &name,const fm::vec4 &v),glUniform4f(location,v.x,v.y,v.z,v.w))

	CREATE_SET_UNIFORM((const std::string &name,const fg::Color &c),glUniform4f(location,c.r/255.f,c.g/255.f,c.b/255.f,c.a/255.f))
	               
	////////////////////////////////////////////////////////////       
	Shader::reference Shader::setUniform(const std::string &name,const fm::mat3 &m,fm::MATRIX::StorageOrder storeOrder)
	{
		if (getGlId())
		{
			int program;
			glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			glCheck(glUseProgram(getGlId()));
			
			// get location of the uniform
			int location = getUniformLocation(name);
			if (location != -1)
			{
				// transpose the matrix if needed and upload
				if (storeOrder==fm::MATRIX::RowMajor)
					glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, &(m.transpose())[0][0]));
				else
					glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]));
			}
			
			glCheck(glUseProgram(program));
		}
		return *this;
	}
	
	////////////////////////////////////////////////////////////       
	Shader::reference Shader::setUniform(const std::string &name,const fm::mat4 &m,fm::MATRIX::StorageOrder storeOrder)
	{
		if (getGlId())
		{
			int program;
			glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			glCheck(glUseProgram(getGlId()));

			// get location of the uniform
			int location = getUniformLocation(name);
			if (location != -1)
			{
				// transpose the matrix if needed and upload
				if (storeOrder==fm::MATRIX::RowMajor)
					glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, &(m.transpose())[0][0]));
				else
					glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]));
			}
			
			glCheck(glUseProgram(program));
		}
		return *this;
	}
	

	////////////////////////////////////////////////////////////
	Shader::reference Shader::setUniform(const std::string &name,const Texture &tex)
	{
		return setUniform(name,&tex);
	}


	////////////////////////////////////////////////////////////
	Shader::reference Shader::setUniform(const std::string &name,const Texture *tex)
	{
		if (getGlId())
		{
			int program;
			glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			glCheck(glUseProgram(getGlId()));
			
			// check if the uniform is in the dictionary
			std::map<std::string, TexUniformData >::iterator it = m_textures.find(name);
			if (it != m_textures.end())
			{
				// activate slot
				glCheck(glActiveTexture(GL_TEXTURE0+it->second.slot));
				glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
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
					glCheck(glUniform1i(location, m_texCounter));
					
					// activate slot
					glCheck(glActiveTexture(GL_TEXTURE0+m_texCounter));
					glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
					m_texCounter++;
				}
				else
					fg_log << "Couldn't find texture "<<name<<" in shader."<<std::endl;

				glCheck(glActiveTexture(GL_TEXTURE0));
				glCheck(glUseProgram(program));

			}
		}
		return *this;
	}


	////////////////////////////////////////////////////////////
    Shader::reference Shader::setAttribPointer(const std::string &name,unsigned int components,unsigned long type,bool normalize,const void *pointer,unsigned int stride)
    {
		if (getGlId())
		{
			int location = getAttribLocation(name);
			
			if (location!=-1)
				enableAttribPointer(name,true),
				glCheck(glVertexAttribPointer(location,components,type,normalize,stride,pointer));
		}
		return *this;
    }
	
	/////////////////////////////////////////////////////////////
	Shader::reference Shader::setAttribPointer(const std::string &name,const fg::Color *pointer,unsigned int stride)
	{
		return setAttribPointer(name,fg::Color::components,fg::is_GLDataType<fg::Color::component_type>::enumVal,false,pointer,stride);
	}

	////////////////////////////////////////////////////////////
    bool Shader::isAvailable()
    {
		std::string glslVer = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		return (glGetError() == GL_NO_ERROR && glslVer.size()>0);
    }
}
