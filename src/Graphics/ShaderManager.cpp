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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
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
    ShaderManager::ShaderManager() : m_stacks(3,fm::MatrixStack<4,4,float>(fm::mat4())),
                                     m_cam(fm::nullPtr),
                                     m_matNames(8,std::string()),
                                     m_matIds(8,0)
    {

    }

	////////////////////////////////////////////////////////////
    ShaderManager::~ShaderManager()
    {

    }

	////////////////////////////////////////////////////////////
    void ShaderManager::clearData()
    {
        m_cam = fm::nullPtr;

        m_assocPoints.clear();
        m_texNames = std::vector<std::string>();
        m_texUseNames = std::vector<std::string>();

        C(m_matIds.size())
            m_matIds[i] = 0;

        Shader::clearData();
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::associate(const std::string &attrName,int point,bool overWrite)
    {
        if (overWrite)
            m_assocPoints[point] = attrName;
        else if (m_assocPoints.find(point) == m_assocPoints.end())
                 m_assocPoints[point] = attrName;
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::setMatrices(const std::string &modelMat,
                                    const std::string &normalMat,
                                    const std::string &colorMat,
                                    const std::string &texUVMat)
    {
        m_matNames[4] = modelMat;
        m_matNames[5] = normalMat;
        m_matNames[6] = colorMat;
        m_matNames[7] = texUVMat;

        m_matIds[4] = modelMat.length()  ? 0 : 2;
        m_matIds[5] = normalMat.length() ? 0 : 2;
        m_matIds[6] = colorMat.length()  ? 0 : 2;
        m_matIds[7] = texUVMat.length()  ? 0 : 2;
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::regTexture(const std::string &texName,const std::string &texInUse)
    {
        m_texNames.push_back(texName);
        m_texUseNames.push_back(texInUse);
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex)
    {
        if (texIndex < m_texNames.size())
        {
            setUniform(m_texNames[texIndex],(const fg::Texture*)tex);

            if (m_texUseNames[texIndex].size() != 0)
                setUniform(m_texUseNames[texIndex],(tex ? 1 : 0));
        }
    }
    
	////////////////////////////////////////////////////////////
	void ShaderManager::setCamera(fm::Ref<fm::Camera> cam)
	{
		m_cam = cam;
	}
	
	////////////////////////////////////////////////////////////
	void ShaderManager::setCamera(fm::Ref<fm::Camera> cam,const std::string &projMat,const std::string &viewMat,const std::string &plyPos,const std::string &plyView)
	{
		setCamera(cam);

        m_matNames[0] = projMat;
        m_matNames[1] = viewMat;
        m_matNames[2] = plyPos;
        m_matNames[3] = plyView;

        C(m_matIds.size())
            m_matIds[i] = (m_matNames[i].length() ? 0 : 2);
	}
	////////////////////////////////////////////////////////////
	fm::Camera *ShaderManager::getCamera()
	{
		return m_cam;
	}

	////////////////////////////////////////////////////////////
    void ShaderManager::clearAssociations()
    {
        m_texNames = std::vector<std::string>();
        m_texUseNames = std::vector<std::string>();
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::clearTextures()
    {
        m_assocPoints.clear();
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::update()
    {
        C(m_matIds.size())
            if (m_matIds[i] == 0)
                m_matIds[i] = (hasUniform(m_matNames[i]) ? 1 : 2);

        if (m_cam)
        {
            if (m_matIds[0] == 1)
                setUniform(m_matNames[0],m_cam->getProjMat());

            if (m_matIds[1] == 1)
                setUniform(m_matNames[1],m_cam->getViewMat());

            if (m_matIds[2] == 1)
                setUniform(m_matNames[2],m_cam->getPos());

            if (m_matIds[3] == 1)
                setUniform(m_matNames[3],m_cam->getViewDir());
        }
    }

	////////////////////////////////////////////////////////////
    fm::Result ShaderManager::prepareDraw(const fg::DrawData &data)
    {
    	update();
    	
        bind();
        
		fm::Error err;

        if (m_matIds[4] == 1)
			err += glCheck(setUniform(m_matNames[4],m_stacks[0].top()));

        if (m_matIds[5] == 1)
            err += glCheck(setUniform(m_matNames[5],fm::MATRIX::inverse(m_stacks[0].top()).transpose()));

        if (m_matIds[6] == 1)
            err += glCheck(setUniform(m_matNames[6],m_stacks[1].top()));

        if (m_matIds[7] == 1)
            err += glCheck(setUniform(m_matNames[7],m_stacks[2].top()));

        for (std::map<int,std::string>::const_iterator it = m_assocPoints.begin();it != m_assocPoints.end();++it)
        {
			if (data.hasAttr((fg::Assoc::Point)it->first) && hasAttribute(it->second))
			{
				const fg::DrawData::Attribute &attr = data[(fg::Assoc::Point)it->first];

				fg::Buffer::bind(attr.buf,fg::ArrayBuffer),
				err += setAttribPointer(it->second,attr.components,attr.componentType,false,fm::nullPtr,attr.stride);
			}
        }
        
		return err;
    }

	////////////////////////////////////////////////////////////
    fm::Result ShaderManager::draw(const fg::DrawData &data)
    {
		return draw(data,0,data.getDrawCount());
    }

	////////////////////////////////////////////////////////////
    fm::Result ShaderManager::draw(const fg::DrawData &data,fm::Size indexSet)
    {
		return draw(data,indexSet,1);
    }

	////////////////////////////////////////////////////////////
    fm::Result ShaderManager::draw(const fg::DrawData &data,fm::Size indexBeg,fm::Size indexCount)
    {
		fm::Error err;
		
		if ((err = prepareDraw(data))) return err;

        C(indexCount)
        {
			const fg::DrawData::DrawCall &draw = data.getDraw(indexBeg+i);

			fg::Buffer::bind(draw.buf,fg::IndexBuffer);
			
			if (draw.componentType)
				err = glCheck(glDrawElements(draw.primitive,draw.indexCount,draw.componentType,fm::nullPtr));
			else
				err = glCheck(glDrawArrays(draw.primitive,draw.drawBeg,draw.drawLen));
        }
        
		err += glCheck((void)0);

        for (std::map<int,std::string>::const_iterator it = m_assocPoints.begin();it != m_assocPoints.end();++it)
        {
			if (data.hasAttr((fg::Assoc::Point)it->first) && hasAttribute(it->second))
			{
				enableAttribPointer(it->second,false);
			}
        }
        
		err += glCheck((void)0);
        
		return err;
    }

	////////////////////////////////////////////////////////////
    fm::MatrixStack<4,4,float> &ShaderManager::getModelStack()
    {
        return m_stacks[0];
    }

	////////////////////////////////////////////////////////////
    fm::MatrixStack<4,4,float> &ShaderManager::getColorStack()
    {
        return m_stacks[1];
    }

	////////////////////////////////////////////////////////////
    fm::MatrixStack<4,4,float> &ShaderManager::getTexUVStack()
    {
        return m_stacks[2];
    }
}
