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
#ifndef FRONTIER_SHADERMANAGER_HPP_INCLUDED
#define FRONTIER_SHADERMANAGER_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Shader.hpp>
#include <FRONTIER/System/Camera.hpp>
#define FRONTIER_SHADERMANAGER

namespace fm
{
    template<fm::Size,fm::Size,class> class matrix;
    typedef matrix<4,4,float> mat4;
}

namespace fg
{
    class Mesh;

    class ShaderManager : public fg::Shader
    {
        void clearData();
    public:
        fm::Camera *cam;
        std::vector<std::string> matNames;
        std::vector<int> matIds;

        std::map<int,std::string> assocPoints; /// e.g.  Mesh::Pos -> "in_pos"

        ShaderManager();

        void setCamera(fm::Camera *cam,const std::string &projMat = "",const std::string &viewMat = "",const std::string &plyPos = "",const std::string &plyView = "");
        void associate(const std::string &attrName,int point,bool overWrite = true);
        void setMatrices(const std::string &modelMat,const std::string &normalMat = std::string());
        void clearAssociations();
        void update();
        void draw(const fg::Mesh &m,const fm::mat4 &modelM = fm::mat4());
    };
}

#endif // FRONTIER_SHADERMANAGER_HPP_INCLUDED
