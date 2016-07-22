#include <FRONTIER/Graphics/FixedShaderManager.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
    FixedShaderManager::FixedShaderManager() : ShaderManager(),
											   m_activeTex(fm::nullPtr)
    {

    }

	////////////////////////////////////////////////////////////
    FixedShaderManager::~FixedShaderManager()
    {

    }

	////////////////////////////////////////////////////////////
    void FixedShaderManager::setCamera(fm::Camera *camera,
									   const std::string &projMat,
									   const std::string &viewMat,
									   const std::string &plyPos,
									   const std::string &plyView)
    {
        m_cam = camera;
        (void)projMat;
        (void)viewMat;
        (void)plyPos;
        (void)plyView;
    }

	////////////////////////////////////////////////////////////
    void FixedShaderManager::setMatrices(const std::string &modelMat,
										 const std::string &normalMat,
										 const std::string &colorMat,
										 const std::string &texUVMat)
    {
        m_matIds[4] = modelMat.length()  ? 1 : 0;
        m_matIds[5] = normalMat.length() ? 1 : 0;
        m_matIds[6] = colorMat.length()  ? 1 : 0;
        m_matIds[7] = texUVMat.length()  ? 1 : 0;
    }

	////////////////////////////////////////////////////////////
    void FixedShaderManager::useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex)
    {
    	if (texIndex == 0) // GL can only use 1 tex at a time
        {
        	m_activeTex = (const fg::Texture*)tex;
        }
    }

	////////////////////////////////////////////////////////////
    void FixedShaderManager::update()
    {

    }

	////////////////////////////////////////////////////////////
    void FixedShaderManager::prepareDraw(const fg::DrawData &data)
    {
    	if (Shader::isAvailable())
		{
			fg::Shader::bind(fm::nullPtr);
		}
		
		// activate the texture if any
		if (m_activeTex)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);

		fg::Texture::bind(m_activeTex);
		
		// set projection matrix
		if (m_cam)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(&m_cam->getProjMat().transpose()[0][0]);
		}
		
		// set model and view matrix
		fm::mat4 m = m_stacks[0].top();
		if (m_cam)
			m = m_cam->getViewMat() * m;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&m.transpose()[0][0]);
		
		// set texture matrix
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&m_stacks[2].top().transpose()[0][0]);
		
		// set attributes
		if (data.hasAttr(fg::Assoc::Position))
		{
			const fg::DrawData::Attribute &attr = data[fg::Assoc::Position];
			glEnableClientState(GL_VERTEX_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
			glVertexPointer(attr.components,attr.componentType,attr.stride,fm::nullPtr);
		}
		if (data.hasAttr(fg::Assoc::Color))
		{
			const fg::DrawData::Attribute &attr = data[fg::Assoc::Color];
			glEnableClientState(GL_COLOR_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
			glColorPointer(attr.components,attr.componentType,attr.stride,fm::nullPtr);
		}
		if (data.hasAttr(fg::Assoc::TextureUV))
		{
			const fg::DrawData::Attribute &attr = data[fg::Assoc::TextureUV];
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);
			glTexCoordPointer(attr.components,attr.componentType,attr.stride,fm::nullPtr);
		}
		if (data.hasAttr(fg::Assoc::Normal))
		{
			const fg::DrawData::Attribute &attr = data[fg::Assoc::Normal];
			glEnableClientState(GL_NORMAL_ARRAY);
			fg::Buffer::bind(attr.buf,fg::ArrayBuffer);

			if (attr.components == 3) // GL only accepts 3d normals
				glNormalPointer(attr.componentType,attr.stride,fm::nullPtr);
		}
    }

	////////////////////////////////////////////////////////////
    void FixedShaderManager::draw(const fg::DrawData &data,fm::Size indexBeg,fm::Size indexCount)
    {
        prepareDraw(data);

        C(indexCount)
        {
			const fg::DrawData::DrawCall &draw = data.getDraw(indexBeg+i);

			fg::Buffer::bind(draw.buf,fg::IndexBuffer);

			if (draw.componentType)
				glDrawElements(draw.primitive,draw.indexCount,draw.componentType,fm::nullPtr);
			else
				glDrawArrays(draw.primitive,draw.drawBeg,draw.drawLen);
        }

		if (data.hasAttr(fg::Assoc::Position))  glDisableClientState(GL_VERTEX_ARRAY);
		if (data.hasAttr(fg::Assoc::Color))     glDisableClientState(GL_COLOR_ARRAY);
		if (data.hasAttr(fg::Assoc::TextureUV)) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		if (data.hasAttr(fg::Assoc::Normal))    glDisableClientState(GL_NORMAL_ARRAY);
		
		if (m_activeTex)
			glDisable(GL_TEXTURE_2D);
    }
}
