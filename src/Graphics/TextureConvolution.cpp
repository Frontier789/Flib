#include <FRONTIER/Graphics/TextureConvolution.hpp>
#include <FRONTIER/Graphics/AttributeRef.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/util/C.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	TextureConvolution::TextureConvolution() : m_target(nullptr),
											   m_linear(true)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	TextureConvolution::TextureConvolution(TextureConvolution &&move)
	{
		move.swap(*this);
	}
	
	/////////////////////////////////////////////////////////////
	TextureConvolution &TextureConvolution::operator=(TextureConvolution &&move)
	{
		return this->swap(move);
	}
	
	/////////////////////////////////////////////////////////////
	TextureConvolution &TextureConvolution::swap(TextureConvolution &conv)
	{
		m_data        .swap(conv.m_data);
		m_fboSecond   .swap(conv.m_fboSecond);
		m_shader      .swap(conv.m_shader);
		m_fboFirst    .swap(conv.m_fboFirst);
		m_drawData    .swap(conv.m_drawData);
		m_texture     .swap(conv.m_texture);
		std::swap(m_shaderResult,conv.m_shaderResult);
		std::swap(m_matrixSize,  conv.m_matrixSize);
		std::swap(m_target,      conv.m_target);
		std::swap(m_linear,      conv.m_linear);
		return *this;
	}
	
	void TextureConvolution::setupShader()
	{
		if (m_linear)
		{
			fm::Size N = m_matrixSize.w;
			
			std::string sourceVert = 
				"#version 130\n"
				"\n"
				"#define FRONTIER_POS\n"
				"\n"
				"uniform vec2 u_texSize;\n"
				"uniform bool u_vertical;\n"
				"\n"
				"in vec2 FRONTIER_POS in_pos;\n"
				"\n"
				"out vec2 va_pos[" + fm::toString(N).str() + "];\n"
				"\n"
				"void main()\n"
				"{\n"
				"	gl_Position = vec4(in_pos,0.0,1.0);\n"
				"	\n"
				"	vec2 rpos = in_pos * .5 + vec2(.5);\n"
				"	\n"
				"	if (u_vertical)\n"
				"	{\n";
			
			for (int x = 0;x<int(N);++x)
			{
				float offs = x - float(N) / 2 + .5;
				sourceVert += 
				"		va_pos[" + fm::toString(x).str() + "] = rpos + vec2( " + fm::toString(offs) + ",0.0) / u_texSize;\n";
			}
				
			sourceVert +=
				"	}\n"
				"	else\n"
				"	{\n";
				
			for (int x = 0;x<int(N);++x)
			{
				float offs = x - float(N) / 2 + .5;
				sourceVert += 
				"		va_pos[" + fm::toString(x).str() + "] = rpos + vec2(0.0, " + fm::toString(offs) + ") / u_texSize;\n";
			}
			
			sourceVert +=
				"	}\n"
				"}\n";
			
			std::string sourceFrag = 
				"#version 130\n"
				"\n"
				"uniform sampler2D u_tex;\n"
				"\n"
				"in vec2 va_pos[" + fm::toString(N).str() + "];\n"
				"out vec4 out_color;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	out_color = \n";
				
			for (int x = 0;x<int(N);++x)
			{
				sourceFrag += 
				"	texture2D(u_tex,va_pos[" + fm::toString(x).str() + "]) * " + fm::toString(m_data[x]);
				
				if (x+1 != int(N))
					sourceFrag += 
				" + \n";
				else
					sourceFrag += 
				";\n";
			}
			
			sourceFrag += 
				"}\n";
			
			m_shaderResult = m_shader.loadFromMemory(sourceVert,sourceFrag);
		}
		else
		{
			fm::Size W = m_matrixSize.w;
			fm::Size H = m_matrixSize.h;
			
			std::string sourceVert = 
				"#version 130\n"
				"\n"
				"#define FRONTIER_POS\n"
				"\n"
				"uniform vec2 u_texSize;\n"
				"uniform bool u_vertical;\n"
				"\n"
				"in vec2 FRONTIER_POS in_pos;\n"
				"\n"
				"out vec2 va_pos[" + fm::toString(W*H).str() + "];\n"
				"out vec2 va_npos;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	gl_Position = vec4(in_pos,0.0,1.0);\n"
				"	\n"
				"	vec2 rpos = in_pos * .5 + vec2(.5);\n"
				"	va_npos = rpos;\n"
				"	\n";
			
			for (int x = 0;x < int(W);++x)
			{
				float offsx = x - float(W) / 2 + .5;
				sourceVert += 
				"	";
				for (int y = 0;y < int(H);++y)
				{
					float offsy = y - float(H) / 2 + .5;
					sourceVert += 
				"va_pos[" + fm::toString(x*H + y).str() + "] = rpos + vec2( " + fm::toString(offsx) + "," + fm::toString(offsy) + ") / u_texSize; ";
				}
				sourceVert += 
				"\n";
			}
				
			sourceVert +=
				"}\n";
			
			std::string sourceFrag = 
				"#version 130\n"
				"\n"
				"uniform sampler2D u_tex;\n"
				"uniform bool u_apply;\n"
				"\n"
				"in vec2 va_pos[" + fm::toString(W*H).str() + "];\n"
				"in vec2 va_npos;\n"
				"out vec4 out_color;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	if (u_apply)\n"
				"	{\n"
				"		out_color = ";
				
			for (int x = 0;x < int(W);++x)
			{
				if (x)
					sourceFrag +=
				"		               ";
				
				for (int y = 0;y < int(H);++y)
				{
					sourceFrag += 
				"texture2D(u_tex,va_pos[" + fm::toString(x*H + y).str() + "]) * " + fm::toString(m_data[x*H + y]);
					if (y+1 != int(H) || x+1 != int(W))
						sourceFrag += 
				" + ";
					else
						sourceFrag += 
				";";
				}
				sourceFrag += "\n";
			}
			
			sourceFrag += 
				"	}\n"
				"	else out_color = texture2D(u_tex,va_npos);\n"
				"}\n";
				
			m_shaderResult = m_shader.loadFromMemory(sourceVert,sourceFrag);
		}
		
		m_shader.setBlendMode(fg::Overwrite);
	}

	void TextureConvolution::setupFBOs()
	{
		if (m_target && m_target->getSize() != m_texture.getSize())
		{
			fm::vec2 s = m_target->getSize();
			
			m_texture.create(s);
			
			const fm::vec2 pts[] = {fm::vec2(-1,-1),fm::vec2(1,-1),fm::vec2(1,1),fm::vec2(-1,-1),fm::vec2(1,1),fm::vec2(-1,1)};
			
			m_fboFirst.create(m_texture);
			m_drawData.positions = pts;
			m_drawData.addDraw(0,6,fg::Triangles);
			
			m_shader.setUniform("u_texSize",s);
		
			m_fboSecond.create(*m_target);
		}
	}

	/////////////////////////////////////////////////////////////
	void TextureConvolution::normalizeData()
	{
		float sum = 0;
		for (float f : m_data)
			sum += f;
		
		if (sum) for (float &f : m_data) f /= sum;
	}

	/////////////////////////////////////////////////////////////
	TextureConvolution::reference TextureConvolution::setTarget(fm::Ref<Texture> target)
	{
		if (m_target != target)
		{
			m_target = target;
			setupFBOs();
		}
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Texture *TextureConvolution::getTarget() const
	{
		return m_target;
	}

	/////////////////////////////////////////////////////////////
	TextureConvolution::reference TextureConvolution::applyTo(Texture &newTarget,fm::Size applyCount)
	{
		setTarget(newTarget);
		return applyToTarget(applyCount);
	}

	/////////////////////////////////////////////////////////////
	TextureConvolution::reference TextureConvolution::applyToTarget(fm::Size applyCount)
	{
		if (!m_target)
			return *this;
		
		fm::rect2s viewport = fg::FrameBuffer::getViewport();
		
		if (m_linear)
		{
			C(applyCount)
			{
				m_fboFirst.bind();
				m_shader.setUniform("u_tex",*m_target);
				m_shader.setUniform("u_vertical",true);
				m_shader.draw(m_drawData);
				
				m_fboSecond.bind();
				m_shader.setUniform("u_tex",m_texture);
				m_shader.setUniform("u_vertical",false);
				m_shader.draw(m_drawData);
			}
		}
		else
		{
			m_shader.setUniform("u_apply",true);
			C(applyCount)
			{
				if (i%2 == 0)
				{
					m_fboFirst.bind();
					m_shader.setUniform("u_tex",*m_target);
					m_shader.draw(m_drawData);
				}
				else
				{
					m_fboSecond.bind();
					m_shader.setUniform("u_tex",m_texture);
					m_shader.draw(m_drawData);
				}
			}
			
			if (applyCount%2 == 1)
			{
				m_shader.setUniform("u_apply",false);
				
				m_fboSecond.bind();
				m_shader.setUniform("u_tex",m_texture);
				m_shader.draw(m_drawData);
			}
		}
		
		FrameBuffer::bind(nullptr);
		FrameBuffer::setViewport(viewport);
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	bool TextureConvolution::isAvailable()
	{
		return FrameBuffer::isAvailable();
	}

	/////////////////////////////////////////////////////////////
	fm::Result TextureConvolution::getShaderResult() const
	{
		return m_shaderResult;
	}
	
	/////////////////////////////////////////////////////////////
	TextureConvolution TextureConvolution::getGaussianBlurrer(fm::Size kernelSize,
															  double sigma)
	{
		double sigma2 = sigma*sigma;
		
		double *data = new double[kernelSize];
		
		Cx(kernelSize)
		{
			double dx = kernelSize/2.0 - x - 0.5;
			double d = dx*dx;
			
			data[x] = std::exp(-d / 2.0 / sigma2) / std::sqrt(2*F_PI*sigma2);
		}
		
		TextureConvolution ret;
		ret.setData(data,fm::vec2s(kernelSize,1),nullptr,true,true);
	
		delete[] data;
		
		return ret;
	}
}

