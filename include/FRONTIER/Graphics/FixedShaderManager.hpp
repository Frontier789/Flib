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
#ifndef FRONTIER_FIXEDSHADERMANAGER_HPP_INCLUDED
#define FRONTIER_FIXEDSHADERMANAGER_HPP_INCLUDED
#include <FRONTIER/Graphics/ShaderManager.hpp>
#define FRONTIER_FIXEDSHADERMANAGER

namespace fg
{
    class FixedShaderManager : public fg::ShaderManager
    {
        ////////////////////////////////////////////////////////////
        fm::Result prepareDraw(const fg::DrawData &data);

        const fg::Texture *m_activeTex;

    public:
        ////////////////////////////////////////////////////////////
        FixedShaderManager();

        ~FixedShaderManager();

        ////////////////////////////////////////////////////////////
        void setCamera(fm::Ref<fm::Camera> cam,const std::string &projMat = "",const std::string &viewMat = "",const std::string &plyPos = "",const std::string &plyView = "");
        void setMatrices(const std::string &modelMat,const std::string &normalMat = "",const std::string &colorMat = "",const std::string &texUVMat = "");
        void useTexture(fm::Ref<const fg::Texture> tex,fm::Size texIndex = 0);

        ////////////////////////////////////////////////////////////
        void update();
        fm::Result draw(const fg::DrawData &data,fm::Size indexBeg,fm::Size indexCount);
    };
}

#endif // FRONTIER_FIXEDSHADERMANAGER_HPP_INCLUDED

