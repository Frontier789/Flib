#include <FRONTIER/Graphics/FixedShaderManager.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>

#include <iostream>
using namespace std;

namespace fg
{
	////////////////////////////////////////////////////////////
    FixedShaderManager::FixedShaderManager() : ShaderManager(),
											   m_activeTex(nullptr)
    {

    }

	////////////////////////////////////////////////////////////
    FixedShaderManager::~FixedShaderManager()
    {

    }

	////////////////////////////////////////////////////////////
	ShaderManager &FixedShaderManager::setUniformNames(const std::string &projMat,
													   const std::string &modelMat,
													   const std::string &viewMat,
													   const std::string &normalMat,
													   const std::string &colorMat,
													   const std::string &texUVMat,
													   const std::string &plyPos,
													   const std::string &plyView,
													   const std::string &time)
    {
        
        (void)projMat;
        (void)modelMat;
        (void)viewMat;
        (void)normalMat;
        (void)colorMat;
        (void)texUVMat;
        (void)plyPos;
        (void)plyView;
        (void)time;
		
		return *this;
    }

	////////////////////////////////////////////////////////////
    ShaderManager &FixedShaderManager::useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex)
    {
		if (texIndex == 0) // old GL can only use 1 tex at a time
        {
        	m_activeTex = (const fg::Texture*)tex;
        }
        
		return *this;
    }

	////////////////////////////////////////////////////////////
	fm::Result FixedShaderManager::update()
    {
		return fm::Result();
    }

	////////////////////////////////////////////////////////////
    fm::Result FixedShaderManager::prepareDraw(const fg::DrawData &data)
    {
    	if (Shader::isAvailable())
		{
			fg::Shader::bind(nullptr);
		}
		
		setBlendMode(m_blendMode);
		
		// activate the texture if any
		if (m_activeTex)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);

		fg::Texture::bind(m_activeTex);
		
		// set projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&m_cam.getProjMat().transpose()[0][0]);
		
		// set model and view matrix
		fm::mat4 m = m_cam.getViewMat() * m_stacks[0].top();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&m.transpose()[0][0]);
		
		// set texture matrix
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&m_stacks[2].top().transpose()[0][0]);
		
		// set attributes
		if (data.hasAttr(fg::Assoc::Position))
		{
			const fg::Attribute &attr = data[fg::Assoc::Position];
			glEnableClientState(GL_VERTEX_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
			glVertexPointer(attr.components,attr.componentType,attr.stride,attr.buf ? attr.buf->getDataPtr() : nullptr);
		}
		if (data.hasAttr(fg::Assoc::Color))
		{
			const fg::Attribute &attr = data[fg::Assoc::Color];
			glEnableClientState(GL_COLOR_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
			glColorPointer(attr.components,attr.componentType,attr.stride,attr.buf ? attr.buf->getDataPtr() : nullptr);
		}
		if (data.hasAttr(fg::Assoc::TextureUV))
		{
			const fg::Attribute &attr = data[fg::Assoc::TextureUV];
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
			glTexCoordPointer(attr.components,attr.componentType,attr.stride,attr.buf ? attr.buf->getDataPtr() : nullptr);
		}
		if (data.hasAttr(fg::Assoc::Normal))
		{
			const fg::Attribute &attr = data[fg::Assoc::Normal];
			glEnableClientState(GL_NORMAL_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);

			if (attr.components == 3) // GL only accepts 3d normals
				glNormalPointer(attr.componentType,attr.stride,attr.buf ? attr.buf->getDataPtr() : nullptr);
		}
		
		return glCheck((void)0);
    }
    
	////////////////////////////////////////////////////////////
    fm::Result FixedShaderManager::draw(const fg::DrawData &data,fm::Size indexBeg,fm::Size indexCount)
    {
        prepareDraw(data);
		
        C(indexCount)
        {
			if (indexBeg+i >= data.getDrawCount()) break;
			
			const fg::DrawCall &draw = data.getDraw(indexBeg+i);

			fg::Buffer::bind(draw.buf,fg::IndexBuffer);
			
			if (draw.componentType)
				glDrawElements(draw.primitive,draw.indexCount,draw.componentType,draw.buf ? draw.buf->getDataPtr() : nullptr);
			else
			{
				fm::Size len = draw.drawLen;
				if (len == 0 && data.hasAttr(fg::Assoc::Position))
					len = data[fg::Assoc::Position].count;
					
				if (len)
					glDrawArrays(draw.primitive,draw.drawBeg,len);
			}
        }
		
		if (indexCount == 0 && data.hasAttr(fg::Assoc::Position))
		{
			glDrawArrays(fg::Triangles,0,data[fg::Assoc::Position].count);
		}

		if (data.hasAttr(fg::Assoc::Position))  glDisableClientState(GL_VERTEX_ARRAY);
		if (data.hasAttr(fg::Assoc::Color))     glDisableClientState(GL_COLOR_ARRAY);
		if (data.hasAttr(fg::Assoc::TextureUV)) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		if (data.hasAttr(fg::Assoc::Normal))    glDisableClientState(GL_NORMAL_ARRAY);
		
		if (m_activeTex)
			glDisable(GL_TEXTURE_2D);
		
		return glCheck((void)0);
    }
}
