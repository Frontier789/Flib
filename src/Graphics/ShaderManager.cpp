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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/Graphics/Mesh.hpp>
#include <FRONTIER/System/matrix.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
    ShaderManager::ShaderManager() : matNames(6,std::string()),
                                     matIds(6,0)
    {

    }

	////////////////////////////////////////////////////////////
    void ShaderManager::clearData()
    {
        cam = fm::nullPtr;

        assocPoints.clear();

        C(matIds.size())
            matIds[i] = 0;

        Shader::clearData();
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::setCamera(fm::Camera *camera,const std::string &projMat,const std::string &viewMat,const std::string &plyPos,const std::string &plyView)
    {
        cam = camera;

        matNames[0] = projMat;
        matNames[1] = viewMat;
        matNames[2] = plyPos;
        matNames[3] = plyView;

        C(matIds.size())
            matIds[i] = (matNames[i].length() ? 0 : 2);
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::associate(const std::string &attrName,int point,bool overWrite)
    {
        if (overWrite)
            assocPoints[point] = attrName;
        else if (assocPoints.find(point) == assocPoints.end())
                assocPoints[point] = attrName;
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::setMatrices(const std::string &modelMat,const std::string &normalMat)
    {
        matNames[4] = modelMat;
        matNames[5] = normalMat;

        matIds[4] = modelMat.length() ? 0 : 2;
        matIds[5] = normalMat.length() ? 0 : 2;
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::clearAssociations()
    {
        assocPoints.clear();
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::update()
    {
        C(matIds.size())
            if (matIds[i] == 0)
                matIds[i] = (hasUniform(matNames[i]) ? 1 : 2);

        if (cam)
        {
            if (matIds[0] == 1)
                setUniform(matNames[0],cam->getProjMat());

            if (matIds[1] == 1)
                setUniform(matNames[1],cam->getViewMat());

            if (matIds[2] == 1)
                setUniform(matNames[2],cam->getPos());

            if (matIds[3] == 1)
                setUniform(matNames[3],cam->getViewDir());
        }
    }

	////////////////////////////////////////////////////////////
    void ShaderManager::draw(const Mesh &m,const fm::mat4 &modelM)
    {
        bind();

        if (matIds[4] == 1)
            setUniform(matNames[4],modelM);

        if (matIds[5] == 1)
            setUniform(matNames[5],fm::MATRIX::inverse(modelM).transpose());

        for (std::map<int,std::string>::const_iterator it = assocPoints.begin();it != assocPoints.end();++it)
            C(m.attrs.size())
                if (m.attrs[i] && m.attrs[i]->type == it->first && hasAttribute(it->second))
                    fg::Buffer::bind(m.attrs[i]->buf,fg::ArrayBuffer),
                    setAttribPointer(it->second,m.attrs[i]->components,m.attrs[i]->componentType,false,m.attrs[i]->ptr,m.attrs[i]->stride);

        C(m.indices.size())
            if(m.indices[i])
                fg::Buffer::bind(m.indices[i]->buf,fg::IndexBuffer),
                glDrawElements(m.indices[i]->primitive,m.indices[i]->indexCount,m.indices[i]->componentType,m.indices[i]->ptr);
    }
}
