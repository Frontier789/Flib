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
#include <FRONTIER/Graphics/ShaderSource.hpp>
#include <FRONTIER/Graphics/CubeTexture.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/System/Delegate.hpp>
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
		
	/////////////////////////////////////////////////////////////
	Shader::TexUniformData::TexUniformData(int location,int slot,int act_id) : location(location),
																			   slot(slot),
																			   act_id(act_id)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Shader::InputData::InputData(fm::Size type,fm::Size size,int location) : flags(0),
																			 type(type),
																			 size(size),
																			 location(location)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	bool Shader::InputData::isArray() const
	{
		return size != 0;
	}
	
	/////////////////////////////////////////////////////////////
	bool Shader::InputData::isAssociated() const
	{
		return flags & FRONTIER_SHADER_UNIFORM_ASSOCIATED_BIT;
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
	fm::Result Shader::createDefVao()
	{
		fm::Result res;
		
		if (!m_ownVao) return res;
		
    	// generate default vao if available
		if (::priv::so_loader.getProcAddr("glGenVertexArrays"))
		{
			if (glIsVertexArray(m_defVao))
				glDeleteVertexArrays(1,&m_defVao);
			
			GLint program;
			res += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			res += glCheck(glUseProgram(getGlId()));

			res += glCheck(glGenVertexArrays(1, &m_defVao));
    		res += glCheck(glBindVertexArray(m_defVao));
			
			res += glCheck(glUseProgram(program));
		}
		
		return res;
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
		
		res += createDefVao();

		return res;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result Shader::postProcess(const std::string *data,const unsigned int *types,unsigned int count)
	{
		(void)data;
		(void)types;
		(void)count;
		return fm::Result();
	}

	////////////////////////////////////////////////////////////
	void Shader::clearData()
	{
		m_textures.clear();
		m_images.clear();
		m_uniforms.clear();
	    m_attributes.clear();
	}

	/// constructor /////////////////////////////////////////////////////////
	Shader::Shader() : m_blendMode(fg::Alpha),
					   m_ownVao(true)
	{

	}


	////////////////////////////////////////////////////////////
	Shader::~Shader()
	{
		if (getGlId() && glIsProgram(getGlId()))
		{
			GLint program;
			glGetIntegerv(GL_CURRENT_PROGRAM,&program);
			
			if (fm::Uint32(program) == getGlId())
				glUseProgram(0);
			
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
		m_images.swap(shader.m_images);
		m_attributes.swap(shader.m_attributes);
		
		std::swap(m_defVao,shader.m_defVao);
		std::swap(m_ownVao,shader.m_ownVao);
		
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
			return fm::Result("GLError",fm::Result::OPFailed,"InitFailed","loadFromFiles",__FILE__,__LINE__);
		
		std::vector<std::string> data(count,"");
		
		C(count)
		{
			std::ifstream in(files[i].c_str(), std::ios::in | std::ios::binary);
			if (in)
			{
				// read whole file
				std::string &contents = data[i];
				in.seekg(0, std::ios::end);
				contents.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&contents[0], contents.size());
				in.close();
			}
			else
				return fm::Result("IOError",fm::Result::OPFailed,"FileNotFound","loadFromFiles",__FILE__,__LINE__,files[i]);
		}
		
		
		return loadFromMemory(&data[0],types,count);
	}


	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromMemory(const std::string *data,const unsigned int *types,unsigned int count)
	{
		// init GlId
		init();
		if (!getGlId())
			return fm::Result("GLError",fm::Result::OPFailed,"InitFailed","loadFromMemory",__FILE__,__LINE__);
		
		// refill sub shaders
		freeSubShaders();
		m_subShaders.resize(count);
		
		fm::Result res;
		C(count)
		{
			res += compileSubShader(data[i],types[i],m_subShaders[i]);
		}
		
		if (!res) return res;
		res = link();
		if (!res) return res;
		
		fillAttribAndUniformData();
		
		m_hasInstancing = ::priv::so_loader.getProcAddr("glVertexAttribDivisor") != nullptr;
		
		return postProcess(data,types,count);
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::loadFromMemory(const fg::ShaderSource *data,unsigned int count)
	{
		std::vector<std::string>  strings(data,data + count);
		std::vector<unsigned int> types(count);
		C(count)
			types[i] = data[i].type;
		
		return loadFromMemory(&strings[0],&types[0],count);
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

		int location = hasUniform(name) - 1;

		return location;
	}
	
	////////////////////////////////////////////////////////////
	void Shader::fillAttribAndUniformData()
	{
		auto uniOrAttribFiller = [&](fm::Size activeFlag,
									 fm::Size maxLenFlag,
									 decltype(glGetActiveUniform) glDataGetterFunc,
									 decltype(glGetUniformLocation) glLocGetterFunc,
									 decltype(m_uniforms) &inputDataHolder){
			GLint dataCount;
			glGetProgramiv(getGlId(),activeFlag,&dataCount);
			
			GLint maxlen;
			glGetProgramiv(getGlId(),maxLenFlag,&maxlen);
			
			std::vector<GLchar> buf(maxlen);
			
			for (int i=0;i<dataCount;++i)
			{
				GLsizei written;
				GLint unisize;
				GLenum unitype;
				glDataGetterFunc(getGlId(),i,maxlen,&written,&unisize,&unitype,&buf[0]);
				
				std::string name;
				bool isarray = false;
				
				for (fm::Size i=0;i<buf.size();++i)
				{
					if (buf[i] == '[')
					{
						name = std::string(buf.begin(),buf.begin()+i);
						isarray = true;
						break;
					}
				}
				
				if (!isarray)
				{
					name = &buf[0];
					unisize = 0;
				}
				
				InputData &inputData = inputDataHolder[name];
				
				inputData.type = unitype;
				inputData.size = unisize;
				inputData.location = glLocGetterFunc(getGlId(),&buf[0]);
			}
		};
		
		uniOrAttribFiller(GL_ACTIVE_UNIFORMS,GL_ACTIVE_UNIFORM_MAX_LENGTH,glGetActiveUniform,glGetUniformLocation,m_uniforms);
		uniOrAttribFiller(GL_ACTIVE_ATTRIBUTES,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,glGetActiveAttrib,glGetAttribLocation,m_attributes);
	}

	////////////////////////////////////////////////////////////
	int Shader::hasUniform(const std::string &name)
	{
		if (!getGlId())
			return 0;
		
		fm::Size i = name.find('[');
		
		if (i == std::string::npos)
		{
			auto it = m_uniforms.find(name);
			if (it == m_uniforms.end())
				return 0;
			
			return it->second.location + 1;			
		}
		
		return glGetUniformLocation(getGlId(),name.c_str()) + 1;	
	}

	/////////////////////////////////////////////////////////////
	fm::Result Shader::enableAttribPointer(const std::string &name,bool enable)
	{
		int location = getAttribLocation(name);

		if (location == -1)
			return fm::Result("GLError",fm::Result::OPFailed,"AttrNotFound","enableAttribPointer",__FILE__,__LINE__,name);
		
		if (enable)
		{
			m_attributes[name].flags &= FRONTIER_SHADER_ATTRIB_POINTER_ENABLED_BIT;
			return glCheck(glEnableVertexAttribArray(location));			
		}
		else
		{
			m_attributes[name].flags &= ~FRONTIER_SHADER_ATTRIB_POINTER_ENABLED_BIT;
			return glCheck(glDisableVertexAttribArray(location));
		}
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
		auto it = m_attributes.find(name);
		if (it != m_attributes.end())
			return it->second.location+1;

		return 0;
	}
	
	namespace priv
	{
		class ProgKeeper
		{
			GLint m_newId;
			GLint m_id;
		public:
			fm::Result &res;
			
			ProgKeeper(GLint id,fm::Result &res) : m_newId(id), res(res)
			{
				glGetIntegerv(GL_CURRENT_PROGRAM,&m_id);
				
				if (m_id != m_newId)
					res += glCheck(glUseProgram(m_newId));
			}
			
			~ProgKeeper()
			{
				if (m_id != m_newId)
				{
					res += glCheck(glUseProgram(m_id));
				}
			}
		};
		
		template<class A,class B> class ConvertHelper                   { public: static A convert(const B &b) {return b;}};
		template<class A,class B> class ConvertHelper<A,fm::vector2<B>> { public: static A convert(const fm::vector2<B> &b) {return b.x;}};
		template<class A,class B> class ConvertHelper<A,fm::vector3<B>> { public: static A convert(const fm::vector3<B> &b) {return b.x;}};
		template<class A,class B> class ConvertHelper<A,fm::vector4<B>> { public: static A convert(const fm::vector4<B> &b) {return b.x;}};
		
		template<class InputT>
		inline fm::Result glUniformForwarder(int loc,fm::Size type,InputT input)
		{
			if (type == GL_FLOAT)      { float     v = ConvertHelper<float,InputT>::convert(input); return glCheck(glUniform1f(loc,v));};
			if (type == GL_FLOAT_VEC2) { fm::vec2  v = input; return glCheck(glUniform2f(loc,v.x,v.y));};
			if (type == GL_FLOAT_VEC3) { fm::vec3  v = input; return glCheck(glUniform3f(loc,v.x,v.y,v.z));};
			if (type == GL_FLOAT_VEC4) { fm::vec4  v = input; return glCheck(glUniform4f(loc,v.x,v.y,v.z,v.w));};
			
			if (type == GL_INT)        { int       v = ConvertHelper<int,InputT>::convert(input); return glCheck(glUniform1i(loc,v));};
			if (type == GL_INT_VEC2)   { fm::vec2i v = input; return glCheck(glUniform2i(loc,v.x,v.y));};
			if (type == GL_INT_VEC3)   { fm::vec3i v = input; return glCheck(glUniform3i(loc,v.x,v.y,v.z));};
			if (type == GL_INT_VEC4)   { fm::vec4i v = input; return glCheck(glUniform4i(loc,v.x,v.y,v.z,v.w));};
			
			if (type == GL_BOOL)       { bool      v = ConvertHelper<bool,InputT>::convert(input); return glCheck(glUniform1i(loc,v));};
			if (type == GL_BOOL_VEC2)  { fm::vec2b v = input; return glCheck(glUniform2i(loc,v.x,v.y));};
			if (type == GL_BOOL_VEC3)  { fm::vec3b v = input; return glCheck(glUniform3i(loc,v.x,v.y,v.z));};
			if (type == GL_BOOL_VEC4)  { fm::vec4b v = input; return glCheck(glUniform4i(loc,v.x,v.y,v.z,v.w));};
			
			return fm::Result("GLSLError",fm::Result::OPChanged,
							  "UniformNotConvertible","setUniform",__FILE__,__LINE__);
		}
	}
	
	#define FRONTIER_ADD_UNIFORM_CONVERTER_true  else res += priv::glUniformForwarder(dat.location,dat.type,v);
	#define FRONTIER_ADD_UNIFORM_CONVERTER_false else res += fm::Result("GLSLError",fm::Result::OPChanged,"UniformNotConvertible","setUniform",__FILE__,__LINE__);
	
	#define FRONTIER_CREATE_SET_UNIFORM(param,valEnum,convertible,callFunc)                     \
	fm::Result Shader::setUniform(const std::string &name,param)                                \
	{                                                                                           \
		fm::Result res;                                                                         \
		if (isLoaded())                                                                          \
		{                                                                                       \
			priv::ProgKeeper keeper(getGlId(),res);                                             \
			                                                                                    \
			auto it = m_uniforms.find(name);                                                    \
			UniformData dat;                                                                    \
			                                                                                    \
			if (it == m_uniforms.end())                                                         \
			{                                                                                   \
				dat.location = glGetUniformLocation(getGlId(),name.c_str());                    \
				dat.type = 0;                                                                   \
				                                                                                \
				if (dat.location == -1)                                                         \
					res += fm::Result("GLSLError",fm::Result::OPFailed,                         \
									  "UniformNotFound","setUniform",__FILE__,__LINE__,name);	\
			}                                                                                   \
			else                                                                                \
				dat = it->second;                                                               \
			                                                                                    \
			if (dat.location != -1)                                                             \
			{                                                                                   \
				if (dat.type == 0 || dat.type == valEnum)                                       \
					res += glCheck(callFunc);                                                   \
				FRONTIER_ADD_UNIFORM_CONVERTER_##convertible                                    \
			}                                                                                   \
		}                                                                                       \
		return res;                                                                             \
	}
	
	FRONTIER_CREATE_SET_UNIFORM(int v,GL_INT,true,glUniform1i(dat.location,v))
	
	FRONTIER_CREATE_SET_UNIFORM(const fm::vec2i &v,GL_INT_VEC2,true,glUniform2i(dat.location,v.x,v.y))
	FRONTIER_CREATE_SET_UNIFORM(const fm::vec3i &v,GL_INT_VEC3,true,glUniform3i(dat.location,v.x,v.y,v.z))
	FRONTIER_CREATE_SET_UNIFORM(const fm::vec4i &v,GL_INT_VEC4,true,glUniform4i(dat.location,v.x,v.y,v.z,v.w))
	
	FRONTIER_CREATE_SET_UNIFORM(float v,GL_FLOAT,true,glUniform1f(dat.location,v))
	
	FRONTIER_CREATE_SET_UNIFORM(const fm::vec2f &v,GL_FLOAT_VEC2,true,glUniform2f(dat.location,v.x,v.y))
	FRONTIER_CREATE_SET_UNIFORM(const fm::vec3f &v,GL_FLOAT_VEC3,true,glUniform3f(dat.location,v.x,v.y,v.z))
	FRONTIER_CREATE_SET_UNIFORM(const fm::vec4f &v,GL_FLOAT_VEC4,true,glUniform4f(dat.location,v.x,v.y,v.z,v.w))
	
	fm::Result Shader::setUniform(const std::string &name,const fg::Color &c)
	{
		return setUniform(name,c.rgba()/255.f);
	}
	
	FRONTIER_CREATE_SET_UNIFORM(const fm::mat2 &m,GL_FLOAT_MAT2,false,glUniformMatrix2fv(dat.location, 1, GL_FALSE, &(m.transpose())[0][0]))
	FRONTIER_CREATE_SET_UNIFORM(const fm::mat3 &m,GL_FLOAT_MAT3,false,glUniformMatrix3fv(dat.location, 1, GL_FALSE, &(m.transpose())[0][0]))
	FRONTIER_CREATE_SET_UNIFORM(const fm::mat4 &m,GL_FLOAT_MAT4,false,glUniformMatrix4fv(dat.location, 1, GL_FALSE, &(m.transpose())[0][0]))

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const Texture &tex,bool sampler)
	{
		return setUniform(name,&tex,sampler);
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const Texture *tex,bool sampler)
	{
		fm::Result res;
		
		if (isLoaded())
		{
			GLint program;
			res += glCheck(glGetIntegerv(GL_CURRENT_PROGRAM,&program));
			res += glCheck(glUseProgram(getGlId()));

			auto &texDict = (sampler ? m_textures : m_images);

			// check if the uniform is in the dictionary
			auto it = texDict.find(name);
			if (it != texDict.end())
			{
				// activate slot
				res += glCheck(glActiveTexture(GL_TEXTURE0 + it->second.slot));
				res += glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));
				if (!sampler && tex && tex->getGlId())
					res += glCheck(glBindImageTexture(0, tex->getGlId(), 0, GL_FALSE, 0, GL_READ_WRITE, tex->getInternalFormat()));
				
				it->second.act_id = tex ? tex->getGlId() : 0;
			}
			else
			{
				// get uniform location
				int location = glGetUniformLocation(getGlId(),name.c_str());

				if (location != -1)
				{
					fm::Size texCount = texDict.size();

					// add to dictionary
					texDict.insert(std::make_pair(name, TexUniformData(location,texCount,tex ? tex->getGlId() : 0)));

					// assign id to texture slot
					res += glCheck(glUniform1i(location, texCount));

					// activate slot
					res += glCheck(glActiveTexture(GL_TEXTURE0 + texCount));
					res += glCheck(glBindTexture(GL_TEXTURE_2D,tex ? tex->getGlId() : 0));

					if (!sampler && tex && tex->getGlId())
						res += glCheck(glBindImageTexture(0, tex->getGlId(), 0, GL_FALSE, 0, GL_READ_WRITE, tex->getInternalFormat()));
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
	fm::Result Shader::setUniform(const std::string &name,const CubeTexture &tex)
	{
		return setUniform(name,&tex);
	}

	////////////////////////////////////////////////////////////
	fm::Result Shader::setUniform(const std::string &name,const CubeTexture *tex)
	{
		fm::Result res;
			
		if (isLoaded())
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
					fm::Size texCount = m_textures.size();

					// add to dictionary
					m_textures.insert(std::make_pair(name, TexUniformData(location,texCount,tex ? tex->getGlId() : 0)));

					// assign id to texture slot
					res += glCheck(glUniform1i(location, texCount));

					// activate slot
					res += glCheck(glActiveTexture(GL_TEXTURE0 + texCount));
					res += glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP,tex ? tex->getGlId() : 0));
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
		
		if (isLoaded())
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
	
	/////////////////////////////////////////////////////////////
	bool Shader::instancingAvailable() const
	{
		return m_hasInstancing;
	}
	
	/////////////////////////////////////////////////////////////
	void Shader::forEachUniform(fm::Delegate<void,std::string,UniformData> func) const
	{
		for (auto &pr : m_uniforms)
			func(pr.first,pr.second);
	}

	/////////////////////////////////////////////////////////////
	void Shader::forEachAttrib(fm::Delegate<void,std::string,AttribData> func) const
	{
		for (auto &pr : m_attributes)
			func(pr.first,pr.second);
	}

	/////////////////////////////////////////////////////////////
	bool Shader::isLoaded() const
	{
		return getGlId();
	}
	
	/////////////////////////////////////////////////////////////
	void Shader::setOwnVao(bool ownVao)
	{
		m_ownVao = ownVao;
	}
}
