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

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_MESH
#include <vector>
#include <deque>

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief A class used to hold all data about a set of vertices
	///
	/////////////////////////////////////////////////////////////
    class FRONTIER_API Mesh
    {
    public:
		typedef Mesh &reference;
		typedef const Mesh &const_reference;
		
		std::vector<fm::vec3> pts; ///< The poisition
		std::vector<fm::vec2> uvs; ///< The texture poisition
		std::vector<fm::vec4> clr; ///< The color

		std::vector<fm::vec3> bitans; ///< The bitangent vector
		std::vector<fm::vec3> norms;  ///< The normal vector
		std::vector<fm::vec3> tans;   ///< The tangent vector
		
		/////////////////////////////////////////////////////////////
		/// @brief Simple class describing a face of the mesh
		///
		/////////////////////////////////////////////////////////////
		class Face
		{
		public:
			std::vector<fm::Uint32> indices; ///< The indices in the face
			fg::Primitive primitive; ///< The used primitive in the face
			
			/////////////////////////////////////////////////////////////
			/// @brief Construct the face 
			/// 
			/// @param primitive The primitive
			/// @param indexCount The number of indices
			/// 
			/////////////////////////////////////////////////////////////
			Face(fg::Primitive primitive = fg::Triangles,fm::Size indexCount = 0);
		};
		
		std::deque<Face> faces; ///< The faces that make up the mesh
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Mesh() = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param copy The mesh to copy
		/// 
		/////////////////////////////////////////////////////////////
		Mesh(const Mesh &copy) FRONTIER_HEAVYCOPY_QUALIFIER;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param move The mesh to move
		/// 
		/////////////////////////////////////////////////////////////
		Mesh(Mesh &&move);
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment
		/// 
		/// @param copy The mesh to copy
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator=(const Mesh &copy) FRONTIER_HEAVYCOPY_QUALIFIER;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move assignment
		/// 
		/// @param move The mesh to move
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator=(Mesh &&move);
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap two meshes
		/// 
		/// @param target The mesh to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference swap(Mesh &target);

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the normal vectors for every vertex
		/// 
		/// Only calculates if no normal vectors are present
		/// 
		/// @param joinIdenticalVertices Indicates whether the vertices with matching positions are to be considered as one
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference calcNormals(bool joinIdenticalVertices = true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Calculate the tangent vectors for every vertex
		/// 
		/// Needs normals to be able to calculate
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference calcTangents();

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the vertices of a sphere
		/// 
		/// @param radius The radius of the sphere
		/// @param W The number of vertices on the horizontal belt
		/// @param H The number of vertices on the vertical belt
		/// @param rfunc A distort function to be applied to the sphere
		/// 
		/// @return The mesh calculated
		/// 
		/////////////////////////////////////////////////////////////
		static Mesh getSphere(float radius = 1,fm::Size W = 20,fm::Size H = 20,const fm::Delegate<float,float &,float &> &rfunc = nullptr);

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the vertices of a torus
		/// 
		/// @param majorR The radius of the main circle
		/// @param mainorR The radius of the secondary circle
		/// @param W The number of vertices on the horizontal belt
		/// @param H The number of vertices on the vertical belt
		/// @param rfunc A distort function to be applied to the torus
		/// 
		/// @return The mesh calculated
		/// 
		/////////////////////////////////////////////////////////////
        static Mesh getTorus(float majorR = 1,float minorR = .5,fm::Size W = 30,fm::Size H = 15,const fm::Delegate<float,float &,float &> &rfunc = nullptr);

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the vertices of a cube
		/// 
		/// @param size The size of the edge of the cube
		/// @param W The number of vertices on the horizontal side
		/// @param H The number of vertices on the vertical side
		/// @param rfunc A distort function to be applied to the cube
		/// 
		/// @return The mesh calculated
		/// 
		/////////////////////////////////////////////////////////////
		static Mesh getCube(float size = 1,fm::Size N = 2,const fm::Delegate<float,float &,float &> &rfunc = nullptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Calculate the vertices of a cylinder
		/// 
		/// @param radius The radius of the cylinder
		/// @param height The height of the cylinder
		/// @param W The number of vertices on the horizontal side
		/// @param H The number of vertices on the vertical side
		/// @param rfunc A distort function to be applied to the cylinder
		/// 
		/// @return The mesh calculated
		/// 
		/////////////////////////////////////////////////////////////
        static Mesh getCylinder(float radius = 1,float height = 1, fm::Size W = 20,fm::Size H = 2, const fm::Delegate<float,float &,float &> &rfunc = nullptr);
    };
}

#endif // FRONTIER_MESH_HPP_INCLUDED
