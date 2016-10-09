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
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <fstream>

namespace fg
{
	/// compile shader objects /////////////////////////////////////////////////////////
    fm::Result Shader::compileSubShader(const std::string &text,unsigned int type,unsigned int &ret)
    {
		// ask GL for an id
        unsigned int s = glCreateShader(type);
		if (!s)
			return fm::Error("GLError","glCreateShader","Couldn't create shader type "+fm::toString(type).str(),"glCreateShader",__FILE__,__LINE__);
		
		// make text acceptable for gl
		const char *c_text = text.c_str();
		
		fm::Error err;
		
		// send shader source to GL
		err += glCheck(glShaderSource(s,1,&c_text,NULL));
		err += glCheck(glCompileShader(s));

		// check compilation result
		int success;
		err += glCheck(glGetShaderiv(s, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			// get needed size for error log
			int logSize;
			err += glCheck(glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logSize));

			// allocate memory for log
			char *logData;
			logData = new char[logSize];

			// retrieve error log
			err += glCheck(glGetShaderInfoLog(s, logSize, NULL, &logData[0]));
			
			err += fm::Error("GLSLError","glCompileShader","Error compiling shader type "+fm::toString(type).str()+" : "+logData,"compileSubShader",__FILE__,__LINE__);
			ret = 0;
			
			// free allocated memory
			delete[] logData;
			return err;
		}
		ret = s;
		
		return err;
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
		
		return fm::Error("IOError","FileNotFound",file+" couldn't be opened for reading","compileSubShaderFromFile",__FILE__,__LINE__);
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
			return fm::Error("ShaderError","CalledWith0ID","Shader::link called with 0 id","link",__FILE__,__LINE__);

		// reset containers
		clearData();
		
		fm::Error err;
		
		// attach subshaders
		C(m_subShaders.size())
			err += glCheck(glAttachShader(getGlId(),m_subShaders[i]));

		// check for compilation errors
		int success;
		err += glCheck(glLinkProgram(getGlId()));
		err += glCheck(glGetProgramiv(getGlId(), GL_LINK_STATUS, &success));
		if (success)
		{
			// on success validate status
			err += glCheck(glValidateProgram(getGlId()));
			err += glCheck(glGetProgramiv(getGlId(), GL_VALIDATE_STATUS, &success));
		}

		// on error print description
		if (!success)
		{
			// get needed size for error log
			int logSize;
			err += glCheck(glGetProgramiv(getGlId(), GL_INFO_LOG_LENGTH, &logSize));

			// allocate memory for log
			char *logData;
			logData = new char[logSize];

			// retrieve error log
			err += glCheck(glGetProgramInfoLog(getGlId(), logSize, NULL, &logData[0]));
			err += fm::Error("GLSLError","glLinkProgram","Error linking shader"+fm::String(": ")+logData,"link",__FILE__,__LINE__);

			// free allocated memory
			delete[] logData;
			return err;
		}
    	
    	// generate default vao if available
		if (::priv::so_loader.getProcAddr("glGenVertexArrays"))
		{
			if (glIsVertexArray(m_defVao))
				glDeleteVertexArrays(1,&m_defVao);
			
			GLint program;
			err += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			err += glCheck(glUseProgram(getGlId()));

			glGenVertexArrays(1, &m_defVao);
    		glBindVertexArray(m_defVao);

			err += glCheck(glUseProgram(program));
			
		}

		return err;
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
	Shader::Shader() : m_texCounter(0)
	{

	}


	/// destructor /////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////
	fm::Result Shader::freeSubShaders()
	{
		fm::Error err;
		
		C(m_subShaders.size())
			if (glIsShader(m_subShaders[i]))
			{
				if ((err = glCheck(glDetachShader(getGlId(),m_subShaders[i])))) return err;
				if ((err = glCheck(glDeleteShader(m_subShaders[i])))) return err;
			}
		
		return err;
	}

	/// functions /////////////////////////////////////////////////////////
	fm::Result Shader::loadFromFiles(const std::string *files,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
			return fm::Error("ShaderError","CalledWith0ID","Shader::loadFromFiles called with 0 id","loadFromFiles",__FILE__,__LINE__);
		
		unsigned int realCount = 0;
		
		C(count)
		{
			if (files[i].size()) realCount++;
		}
		
		const std::string *filesptr  = files;
		const unsigned int *typesptr = types;
		
		if (realCount != count)
		{
			std::string *realFiles  = new std::string[realCount];
			unsigned int *realTypes = new unsigned int[realCount];
			
			filesptr = realFiles;
			typesptr = realTypes;
			
			unsigned int curI = 0;
			C(count)
			{
				if (files[i].size())
				{
					realFiles[curI] = files[i],
					realTypes[curI] = types[i];
					
					++curI;
				}
			}
		}
		
		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(realCount);
		
		fm::Error err;
		C(realCount)
		{
			err += compileSubShaderFromFile(filesptr[i],typesptr[i],m_subShaders[i]);
		}
		
		if (realCount != count)
		{
			delete[] filesptr;
			delete[] typesptr;			
		}
		
		if (err) return err;
		
		return link();
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromMemory(const std::string *data,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
			return fm::Error("ShaderError","CalledWith0ID","Shader::loadFromMemory called with 0 id","loadFromMemory",__FILE__,__LINE__);
		
		unsigned int realCount = 0;
		
		C(count)
		{
			if (data[i].size()) realCount++;
		}
		
		const std::string *dataptr = data;
		const unsigned int *typesptr = types;
		
		if (realCount != count)
		{
			std::string *realData   = new std::string[realCount];
			unsigned int *realTypes = new unsigned int[realCount];
			
			dataptr = realData;
			typesptr = realTypes;
			
			unsigned int curI = 0;
			C(count)
			{
				if (data[i].size())
				{
					realData[curI]  = data[i],
					realTypes[curI] = types[i];
					
					++curI;
				}
			}
		}
		
		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(realCount);
		
		fm::Error err;
		C(realCount)
		{
			err += compileSubShader(dataptr[i],typesptr[i],m_subShaders[i]);
		}
		
		if (realCount != count)
		{
			delete[] dataptr;
			delete[] typesptr;			
		}
		
		if (err) return err;
		
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
	void Shader::bind(const Shader *program)
	{
		if (program)
			program->bind();
		else glUseProgram(0);
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
			return fm::Error("GLError","AttrNotFound","Attribute "+name+" couldn't be found","getAttribLocation",__FILE__,__LINE__);

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
		fm::Error err;                                                                    \
		if (getGlId())                                                                    \
		{                                                                                 \
			GLint program;                                                                \
			err += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));                   \
			if ((GLuint)program != getGlId())                                             \
				err += glCheck(glUseProgram(getGlId()));                                  \
                                                                                          \
			int location = getUniformLocation(name);                                      \
			if (location != -1)                                                           \
				err += glCheck(callFunc);                                                 \
                                                                                          \
			if ((GLuint)program != getGlId())                                             \
				err += glCheck(glUseProgram(program));                                    \
		}                                                                                 \
		return err;                                                                       \
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
	fm::Result Shader::setUniform(const std::string &name,const fm::mat3 &m,fm::MATRIX::StorageOrder storeOrder)
	{
		fm::Error err;
		
		if (getGlId())
		{
			GLint program;
			err += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			err += glCheck(glUseProgram(getGlId()));

			// get location of the uniform
			int location = getUniformLocation(name);
			if (location != -1)
			{
				// transpose the matrix if needed and upload
				if (storeOrder==fm::MATRIX::RowMajor)
				{
					err += glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, &(m.transpose())[0][0]));
				}
				else
					err += glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]));
			}

			err += glCheck(glUseProgram(program));
		}
		return err;
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const fm::mat4 &m,fm::MATRIX::StorageOrder storeOrder)
	{
		fm::Error err;
		
		if (getGlId())
		{
			GLint program;
			err += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			err += glCheck(glUseProgram(getGlId()));

			// get location of the uniform
			int location = getUniformLocation(name);
			if (location != -1)
			{
				// transpose the matrix if needed and upload
				if (storeOrder==fm::MATRIX::RowMajor)
				{
					err += glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, &(m.transpose())[0][0]));
				}
				else
					err += glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]));
			}

			err += glCheck(glUseProgram(program));
		}
		
		return err;
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const Texture &tex)
	{
		return setUniform(name,&tex);
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const Texture *tex)
	{
		fm::Error err;
		
		if (getGlId())
		{
			GLint program;
			err += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			err += glCheck(glUseProgram(getGlId()));

			// check if the uniform is in the dictionary
			std::map<std::string, TexUniformData >::iterator it = m_textures.find(name);
			if (it != m_textures.end())
			{
				// activate slot
				err += glCheck(glActiveTexture(GL_TEXTURE0+it->second.slot));
				err += glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
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
					err += glCheck(glUniform1i(location, m_texCounter));

					// activate slot
					err += glCheck(glActiveTexture(GL_TEXTURE0+m_texCounter));
					err += glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
					m_texCounter++;
				}
				else
                    return fm::Error("GLSLError","NoSuchUniform",name+" texture couldn't be found in shader","setUniform",__FILE__,__LINE__);
			}
			
			err += glCheck(glActiveTexture(GL_TEXTURE0));
			err += glCheck(glUseProgram(program));
		}
		return err;
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const CubeTexture &tex)
	{
		return setUniform(name,&tex);
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const CubeTexture *tex)
	{
		fm::Error err;
			
		if (getGlId())
		{
			GLint program;
			err += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			err += glCheck(glUseProgram(getGlId()));

			// check if the uniform is in the dictionary
			std::map<std::string, TexUniformData >::iterator it = m_textures.find(name);
			if (it != m_textures.end())
			{
				// activate slot
				err += glCheck(glActiveTexture(GL_TEXTURE0+it->second.slot));
				err += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,tex ? tex->getGlId() : 0));
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
					err += glCheck(glUniform1i(location, m_texCounter));

					// activate slot
					err += glCheck(glActiveTexture(GL_TEXTURE0+m_texCounter));
					err += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,tex ? tex->getGlId() : 0));
					m_texCounter++;
				}
				else
					return fm::Error("GLSLError","NoSuchUniform",name+" texture couldn't be found in shader","setUniform",__FILE__,__LINE__);

				err += glCheck(glActiveTexture(GL_TEXTURE0));
				err += glCheck(glUseProgram(program));

			}
		}
		
		return err;
	}


	////////////////////////////////////////////////////////////
    fm::Result Shader::setAttribPointer(const std::string &name,unsigned int components,unsigned long type,bool normalize,const void *pointer,unsigned int stride)
    {
		fm::Error err;
		
		if (getGlId())
		{
			int location = getAttribLocation(name);

			if (location!=-1)
			{
				enableAttribPointer(name,true);
				err += glCheck(glVertexAttribPointer(location,components,type,normalize,stride,pointer));
			}
		}
		return err;
    }

	/////////////////////////////////////////////////////////////
	fm::Result Shader::setAttribPointer(const std::string &name,const fg::Color *pointer,unsigned int stride)
	{
		return setAttribPointer(name,fg::Color::components,fg::Is_GLDataType<fg::Color::component_type>::enumVal,false,pointer,stride);
	}

	////////////////////////////////////////////////////////////
    bool Shader::isAvailable()
    {
		std::string glslVer = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		return (glGetError() == GL_NO_ERROR && glslVer.size()>0);
    }
}
