////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
#ifndef FRONTIER_PRIMITIVE_HPP_INCLUDED
#define FRONTIER_PRIMITIVE_HPP_INCLUDED
#define FRONTIER_PRIMITIVE

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Identicetes how vertices will be rasterized
	///
	///
	/// Points:
	/// 	for every vertex render a point
	/// 	0
	/// 	 1
	/// 	  2
	/// 	   3
	/// Lines:
	/// 	group vertices by two and render lines
	/// 	01
	/// 	  23
	/// 		45
	/// 		  67
	/// LineStrip:
	/// 	render a single broken line from the given vertices
	/// 	01
	/// 	 12
	/// 	  23
	/// 	   34
	/// LineLoop:
	/// 	render a single broken closed line from the given vertices
	///    n0
	/// 	01
	/// 	 12
	/// 	  23
	/// 	   34
	/// Triangles:
	/// 	group vertices by three and render triangles
	/// 	012
	/// 	   345
	/// 		  678
	/// TriangleStrip:
	/// 	render series of triangles
	/// 	012
	/// 	 123
	/// 	  234
	/// 	   345
	/// TriangleFan:
	/// 	render triangles with a fixed vertex
	/// 	012
	/// 	0 23
	/// 	0  34
	/// 	0   45
	///
	/////////////////////////////////////////////////////////////
	enum Primitive {
		Lines         = 1u, ///< render N/2 lines
		Points        = 0u, ///< render N points
		LineLoop      = 2u, ///< render a single closed broken line
		Triangles     = 4u, ///< render N/3 triangles
		LineStrip     = 3u, ///< render a single broken line
		TriangleFan   = 6u, ///< render (N-1) triangles (N>=3)
		TriangleStrip = 5u  ///< render (N-1) triangles (N>=3)
	};
}
#endif //FRONTIER_PRIMITIVE_HPP_INCLUDED
