////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_DRAWDATA_HPP_INCLUDED
#define FRONTIER_DRAWDATA_HPP_INCLUDED

#include <FRONTIER/System/util/dont_include_inl_begin>
#include <FRONTIER/Graphics/AttributeRef.hpp>
#include <FRONTIER/System/util/dont_include_inl_end>

#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/DrawCall.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_DRAWDATA
#include <vector>
#include <map>

namespace fg
{
	class IndexHolder;
	class Attribute;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to store all the data needed for an opengl draw call
	///
	/// @ingroup Graphics
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API DrawData
	{
	public:
		mutable std::map<AssocPoint,Attribute*> m_attrs; ///< Maps the association points to attributes
		std::vector<DrawCall> m_drawCalls; ///< Stores drawing calls

    public:
		typedef DrawData &reference;
		typedef const DrawData &const_reference;
		
		AttributeRef positions;    ///< References the positions attribute
		AttributeRef colors;       ///< References the colors attribute
		AttributeRef normals;      ///< References the normals attribute
		AttributeRef tangents;     ///< References the tangents attribute
		AttributeRef bitangents;   ///< References the bitangents attribute
		AttributeRef texPositions; ///< References the texPositions attribute
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		DrawData();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~DrawData();
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param drawData The drawData to copy
		/// 
		/////////////////////////////////////////////////////////////
		DrawData(const DrawData &drawData) FRONTIER_HEAVYCOPY_QUALIFIER;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param drawData The drawData to move
		/// 
		/////////////////////////////////////////////////////////////
		DrawData(DrawData &&drawData);

		/////////////////////////////////////////////////////////////
		/// @brief Construct the draw data from a mesh
		/// 
		/// @param mesh The mesh to use
		/// 
		/////////////////////////////////////////////////////////////
		DrawData(const Mesh &mesh);

		/////////////////////////////////////////////////////////////
		/// @brief Fetch an attribute based on its association point
		/// 
		/// @param type The association point
		/// 
		/// @return References to the attribute
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &operator[](AssocPoint type);

		/////////////////////////////////////////////////////////////
		/// @brief Fetch an attribute based on its association point
		/// 
		/// @param type The association point
		/// 
		/// @return References to the attribute
		/// 
		/////////////////////////////////////////////////////////////
		const Attribute &operator[](AssocPoint type) const;

		/////////////////////////////////////////////////////////////
		/// @brief Fetch an attribute based on its association point
		/// 
		/// @param type The association point
		/// 
		/// @return References to the attribute
		/// 
		/////////////////////////////////////////////////////////////
		Attribute &getAttribute(AssocPoint type);

		/////////////////////////////////////////////////////////////
		/// @brief Fetch an attribute based on its association point
		/// 
		/// @param type The association point
		/// 
		/// @return References to the attribute
		/// 
		/////////////////////////////////////////////////////////////
		const Attribute &getAttribute(AssocPoint type) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment operator
		/// 
		/// @param drawData The drawData to copy
		/// 
		/// @return References to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawData &operator=(const DrawData &drawData) FRONTIER_HEAVYCOPY_QUALIFIER;
		
		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		/// 
		/// @param drawData The drawData to move
		/// 
		/// @return References to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawData &operator=(DrawData &&drawData);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the draw data from a mesh
		/// 
		/// @param mesh The mesh to use
		/// 
		/// @return References to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawData &operator=(const Mesh &mesh);
		
		/////////////////////////////////////////////////////////////
		/// @brief Remove an Attribute based on its association
		/// 
		/// @param type The association point
		/// 
		/// @return References to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawData &remAttr(AssocPoint type);
		
		/////////////////////////////////////////////////////////////
		/// @brief Test if there is an attribute with given association point
		/// 
		/// @param type The association point
		/// 
		/// @return True iff there is an attribute with given association point
		/// 
		/////////////////////////////////////////////////////////////
		bool hasAttr(AssocPoint type) const;

		/////////////////////////////////////////////////////////////
		/// @brief Add a drawing call that uses all vertices to the data
		/// 
		/// @param primitive The primitive to draw with
		/// 
		/// @return reference to the added drawcall
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &addDraw(fg::Primitive primitive = fg::Triangles);
		
		/////////////////////////////////////////////////////////////
		/// @brief Add a drawing call from indices to the data
		/// 
		/// @param inds The indices to be used
		/// @param primitive The primitive to draw with
		/// 
		/// @return reference to the added drawcall
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &addDraw(const fg::IndexHolder &inds,fg::Primitive primitive = fg::Triangles);
		
		/////////////////////////////////////////////////////////////
		/// @brief Add a range dased drawing call to the data
		/// 
		/// @param beg The first vertex to use
		/// @param len The number vertices to use
		/// @param primitive The primitive to draw with
		/// 
		/// @return reference to the added drawcall
		/// 
		/////////////////////////////////////////////////////////////
        DrawCall &addDraw(fm::Size beg,fm::Size len,fg::Primitive primitive = fg::Triangles);
		
		/////////////////////////////////////////////////////////////
		/// @brief Add an index dased drawing call to the data
		/// 
		/// @param data The indices to use
		/// @param primitive The primitive to draw with
		/// 
		/// @return reference to the added drawcall
		/// 
		/////////////////////////////////////////////////////////////
        template<class T,fm::Size N>
        inline DrawCall &addDraw(const T (&data)[N],fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Add an index dased drawing call to the data
		/// 
		/// @param pointer Pointer to the indices
		/// @param N The number of indices
		/// @param primitive The primitive to draw with
		/// 
		/// @return reference to the added drawcall
		/// 
		/////////////////////////////////////////////////////////////
        template<class T>
        inline DrawCall &addDraw(const T *pointer,fm::Size N,fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Get reference to a drawcall
		/// 
		/// @param index The index of the drawcall
		/// 
		/// @return reference to the drawcall
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &getDraw(fm::Size index);

		/////////////////////////////////////////////////////////////
		/// @brief Get reference to a drawcall
		/// 
		/// @param index The index of the drawcall
		/// 
		/// @return reference to the drawcall
		/// 
		/////////////////////////////////////////////////////////////
		const DrawCall &getDraw(fm::Size index) const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of drawcalls
		/// 
		/// @return The number of drawcalls
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getDrawCount() const;

		/////////////////////////////////////////////////////////////
		/// @brief Clear drawing data
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawData &clearDraws();

		/////////////////////////////////////////////////////////////
		/// @brief Clear all data (attributes and drawcalls alike)
		/// 
		/////////////////////////////////////////////////////////////
		DrawData &reset();
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of the drawdata
		/// 
		/// @param drawData The drawdata to swap with
		/// 
		/// @return References to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference swap(DrawData &drawData);
	};
}

#endif // FRONTIER_DRAWDATA_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/DrawData.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
