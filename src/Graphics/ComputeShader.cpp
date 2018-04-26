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
#include <FRONTIER/Graphics/ComputeShader.hpp>
#include <FRONTIER/Graphics/ShaderSource.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	ComputeShader::ComputeShader()
	{
		
	}

	/////////////////////////////////////////////////////////////
	ComputeShader::ComputeShader(ComputeShader &&shader)
	{
		shader.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	ComputeShader &ComputeShader::operator=(ComputeShader &&shader)
	{
		return this->swap(shader);
	}

	/////////////////////////////////////////////////////////////
	ComputeShader &ComputeShader::swap(ComputeShader &shader)
	{
		m_shader.swap(shader.m_shader);
		std::swap(getGlId(),shader.getGlId());
		return *this;
	}

	/////////////////////////////////////////////////////////////
	ComputeShader::~ComputeShader()
	{

	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::loadFromFile(const std::string &file)
	{
		unsigned int type = GL_COMPUTE_SHADER;

		fm::Result res = m_shader.loadFromFiles(&file,&type,1);
		
		getGlId() = m_shader.getGlId();
		
		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::loadFromMemory(const std::string &data)
	{
		unsigned int type = GL_COMPUTE_SHADER;

		fm::Result res = m_shader.loadFromMemory(&data,&type,1);
		
		getGlId() = m_shader.getGlId();
		
		return res;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::loadFromMemory(const fg::ShaderSource &data)
	{
		fm::Result res;
		
		if (data.type == GL_COMPUTE_SHADER)
			res = m_shader.loadFromMemory(&data,1);
		else
		{
			fg::ShaderSource cpy = data;
			cpy.type = GL_COMPUTE_SHADER;

			res = m_shader.loadFromMemory(&cpy,1);
		}
		
		getGlId() = m_shader.getGlId();
		
		return res;
	}

	/////////////////////////////////////////////////////////////
	void ComputeShader::bind() const
	{
		m_shader.bind();
	}

	/////////////////////////////////////////////////////////////
	int ComputeShader::getUniformLocation(const std::string &name)
	{
		return m_shader.getUniformLocation(name);
	}
	
	/////////////////////////////////////////////////////////////
	void ComputeShader::fillUniformData()
	{
		m_shader.fillAttribAndUniformData();
	}

	/////////////////////////////////////////////////////////////
	int ComputeShader::hasUniform(const std::string &name)
	{
		return m_shader.hasUniform(name);
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::setUniform(const std::string &name,const Texture &tex,bool sampler)
	{
		return m_shader.setUniform(name,tex,sampler);
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::setUniform(const std::string &name,const Texture *tex,bool sampler)
	{
		return m_shader.setUniform(name,tex,sampler);
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::setUniform(const std::string &name,const FloatTexture &tex,bool sampler)
	{
		return m_shader.setUniform(name,tex,sampler);
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::setUniform(const std::string &name,const FloatTexture *tex,bool sampler)
	{
		return m_shader.setUniform(name,tex,sampler);
	}

	/////////////////////////////////////////////////////////////
	void ComputeShader::bind(fm::Ref<const ComputeShader> program)
	{
		Shader::bind(program ? &program->m_shader : nullptr);
	}

	/////////////////////////////////////////////////////////////
	bool ComputeShader::isAvailable()
	{
		GLint max_x;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &max_x);
		
		return (glGetError() == GL_NO_ERROR && max_x > 0);
	}

	/////////////////////////////////////////////////////////////
	void ComputeShader::forEachUniform(fm::Delegate<void,std::string,Shader::UniformData> func) const
	{
		m_shader.forEachUniform(func);
	}

	/////////////////////////////////////////////////////////////
	bool ComputeShader::isLoaded() const
	{
		return m_shader.isLoaded();
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::dispatch(fm::vec3s workGroupDims) const
	{
		if (workGroupDims.x == 0) workGroupDims.x = 1;
		if (workGroupDims.y == 0) workGroupDims.y = 1;
		if (workGroupDims.z == 0) workGroupDims.z = 1;
		
		bind();

		return glCheck(glDispatchCompute(workGroupDims.x, workGroupDims.y, workGroupDims.z));
	}

	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::run(fm::vec3s workGroupDims) const
	{
		return dispatch(workGroupDims);
	}
	
	/////////////////////////////////////////////////////////////
	const VertexArray &ComputeShader::getVao() const
	{
		return m_shader.getVao();
	}

	/////////////////////////////////////////////////////////////
	VertexArray &ComputeShader::getVao()
	{
		return m_shader.getVao();
	}
	
	/////////////////////////////////////////////////////////////
	const Shader &ComputeShader::getShader() const
	{
		return m_shader;
	}

	/////////////////////////////////////////////////////////////
	Shader &ComputeShader::getShader()
	{
		return m_shader;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result ComputeShader::setStorageBuf(fm::Size index,fm::Ref<const fg::Buffer> buf,fm::Size offset,fm::Size size)
	{
		return m_shader.setStorageBuf(index,buf,offset,size);
	}
}
