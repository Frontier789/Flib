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
#ifndef FRONTIER_KDTREE_HPP_INCLUDED
#define FRONTIER_KDTREE_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/Delegate.hpp>
#define FRONTIER_KDTREE
#include <vector>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to build and query a k dimensional space partitioning tree
	/// 
	/////////////////////////////////////////////////////////////
	template<class VectorType,
			 fm::Size Dim = VectorType::components,
			 class AxisType = typename VectorType::component_type>
	class KDTree
	{
	private:
		std::vector<AxisType>   m_axes; ///< Holds the cutting axes
		std::vector<VectorType> m_data; ///< Holds the points
		fm::Delegate<AxisType,const VectorType &,const VectorType &> m_distFunc; ///< The distance function
		
		void buildKDTRange(fm::Size first,fm::Size last,fm::Size depth); ///< Build a range of the tree
		VectorType getNearestRange(VectorType p,fm::Size first,fm::Size last,fm::Size depth) const; ///< Query a range of the tree
		AxisType getDist1Dim(const AxisType &a1,const AxisType &a2) const; ///< Get the distance between the scalar types
		
	public:
		typedef KDTree<VectorType,Dim,AxisType> &reference;
		typedef const KDTree<VectorType,Dim,AxisType> &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		KDTree();
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param copy The tree to copy
		/// 
		/////////////////////////////////////////////////////////////
		KDTree(const KDTree<VectorType,Dim,AxisType> &copy);
		
		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param move The tree to move
		/// 
		/////////////////////////////////////////////////////////////
		KDTree(KDTree<VectorType,Dim,AxisType> &&move);
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap two trees
		/// 
		/// @param tree The tree to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		KDTree &swap(KDTree &tree);
		
		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment operator
		/// 
		/// @param tree The tree to copy
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		KDTree &operator=(const KDTree<VectorType,Dim,AxisType> &tree);
		
		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		/// 
		/// @param tree The tree to move
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		KDTree &operator=(KDTree<VectorType,Dim,AxisType> &&tree);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the tree from a data pointer and distance function
		/// 
		/// @param ptr The data pointer
		/// @param vecCount The number of points to copy from ptr
		/// @param distFunc The distance function
		/// 
		/////////////////////////////////////////////////////////////
		KDTree(VectorType *ptr,fm::Size vecCount,fm::Delegate<AxisType,const VectorType &,const VectorType &> distFunc = defaultDistanceFunc);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the tree by moving data from a vector
		/// 
		/// @param data The data to move
		/// @param distFunc The distance function
		/// 
		/////////////////////////////////////////////////////////////
		KDTree(std::vector<VectorType> &&data,fm::Delegate<AxisType,const VectorType &,const VectorType &> distFunc = defaultDistanceFunc);
		
		/////////////////////////////////////////////////////////////
		/// @brief Find the nearest point in the tree for a given point
		/// 
		/// @param p The target point
		/// 
		/// @return The nearest point
		/// 
		/////////////////////////////////////////////////////////////
		VectorType getNearest(VectorType p) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Find the nearest point in the tree for a given point
		/// 
		/// @param p The target point
		/// 
		/// @return The nearest point
		/// 
		/////////////////////////////////////////////////////////////
		VectorType operator()(VectorType p) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Evaluate the default distance function
		/// 
		/// d = SUM((v1[i]-v2[i])^2)
		/// 
		/// @param v1 The first point
		/// @param v2 The second point
		/// 
		/// @return The distance between @a v1 and @a v2
		/// 
		/////////////////////////////////////////////////////////////
		static AxisType defaultDistanceFunc(const VectorType &v1,const VectorType &v2);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of points in the tree (same as number of axes)
		/// 
		/// @return The number of points in the tree
		/// 
		/////////////////////////////////////////////////////////////
		fm::Size getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a pointer to the axes
		/// 
		/// @return A pointer to the axes
		/// 
		/////////////////////////////////////////////////////////////
		const AxisType *getAxesPtr() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a pointer to the points
		/// 
		/// @return A pointer to the points
		/// 
		/////////////////////////////////////////////////////////////
		const VectorType *getPointsPtr() const;
	};

	typedef KDTree<vec2> KDTree2;
	typedef KDTree<vec3> KDTree3;
	typedef KDTree<vec4> KDTree4;
}

#endif // FRONTIER_KDTREE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/KDTree.inl>
#endif
