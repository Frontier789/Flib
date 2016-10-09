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
#ifndef FRONTIER_MESH_HPP_INCLUDED
#define FRONTIER_MESH_HPP_INCLUDED

#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/Vector3.hpp>

#define FRONTIER_MESH
#include <vector>
#include <deque>

namespace fg
{
    class FRONTIER_API Mesh : public fm::NonCopyable
    {
		Mesh(const Mesh &m);
    public:
		typedef Mesh &reference;
		typedef const Mesh &const_reference;
		
		std::vector<fm::vec3> pts;
		std::vector<fm::vec2> uvs;

		std::vector<fm::vec3> norms;
		std::vector<fm::vec3> tans;
		std::vector<fm::vec3> bitans;
		
		class Face
		{
		public:
			std::vector<fm::Uint32> indices;
			fg::Primitive primitive;
			
			Face(fg::Primitive primitive = fg::Triangles,fm::Size indexCount = 0);
		};
		
		std::deque<Face> faces;
		
		Mesh();
		
		Mesh copy();
		reference swap(Mesh &target);

		void calcNormals(bool joinIdenticalVertices = true);
		void calcTangents();
		void disjoinFaces();

		static Mesh getSphere  (float radius = 1,                  fm::Size W = 20,fm::Size H = 20,const fm::Delegate<float,float &,float &> &rfunc = fm::nullPtr);
        static Mesh getTorus   (float majorR = 1,float minorR = .5,fm::Size W = 30,fm::Size H = 15,const fm::Delegate<float,float &,float &> &rfunc = fm::nullPtr);
        static Mesh getCube    (float size   = 1,                  fm::Size N = 2,                 const fm::Delegate<float,float &,float &> &rfunc = fm::nullPtr);
        static Mesh getCylinder(float radius = 1,float height = 1, fm::Size W = 20,fm::Size H = 2, const fm::Delegate<float,float &,float &> &rfunc = fm::nullPtr);
        static Mesh getCone    (float radius = 1,float height = 1, fm::Size N = 20,                const fm::Delegate<float,float &,float &> &rfunc = fm::nullPtr);
    };
}

#endif // FRONTIER_MESH_HPP_INCLUDED
