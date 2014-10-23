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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_DRAWABLE_HPP_INCLUDED
#define FRONTIER_DRAWABLE_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_DRAWABLE
namespace fm
{
	template<class,class,class,class>
	class vertex;
}
namespace fg
{
	class Shader;
	class Buffer;
	class Texture;

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
	
	/////////////////////////////////////////////////////////////
	/// @brief Store data about a vertex attribute
	/// 
	/// Stores 
	/// 	the number of components in the vertex attribute
	/// 	the number of bytes in a component
	/// 	the number of bytes in a vertex (also called stride)
	/// 	the pointer to the data
	/// 	the pointer to the GL buffer (if using it)
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Attribute
	{
	public:
		const void *m_ptr;    ///< pointer to data stored in ram
		fg::Buffer *m_buffer; ///< pointer to GL buffer (data on gpu)
		unsigned char  m_components;        ///< number of components in a vertex
		unsigned char  m_bytesPerComponent; ///< number of bytes in a component 
		unsigned short m_bytesPerVertex;    ///< number of bytes in a vertex (also called stride)
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct using a type
		/// 
		/// Note: T must have components member enum (or static integer variable)
		/// T must also have a member typedef component_type
		/// 
		/// @param ptr The pointer to the data
		/// @param stride The number of bytes in a vertex (0 means sizeof(T))
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		Attribute(const T *ptr,unsigned short stride = 0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// Constructs Empty attribute
		/// 
		/////////////////////////////////////////////////////////////
		Attribute();
		
		/////////////////////////////////////////////////////////////
		/// @brief Indicates a unused attribute
		/// 
		/////////////////////////////////////////////////////////////
		static Attribute Unused;
		
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if attribute is used
		/// 
		/// @return True iff the attribute has data in it
		/// 
		/////////////////////////////////////////////////////////////
		bool isUsed() const;
	};		
	
	/////////////////////////////////////////////////////////////
	/// @brief Capable of holding the properties of index pointers
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API IndexPointer
	{
	public:
		unsigned int m_bytesPerIndex; ///< Holds the number of bytes in a index (may be NULL)
		const fg::Buffer *m_buffer;   ///< The GL buffer holding the indices (may be NULL)
		const void *m_ptr; ///< The pointer to the indices in ram
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		IndexPointer();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from uchar type pointer
		/// 
		/////////////////////////////////////////////////////////////
		IndexPointer(const unsigned char  *ptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from ushort type pointer
		/// 
		/////////////////////////////////////////////////////////////
		IndexPointer(const unsigned short *ptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from uint type pointer
		/// 
		/////////////////////////////////////////////////////////////
		IndexPointer(const unsigned int   *ptr);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct from a general pointer, the index size and a buffer (can be null)
		/// 
		/// @param ptr The pointer
		/// @param bytesPerIndex Amount of bytes in a index
		/// @param buffer The GL buffer holding th e indices
		/// 
		/////////////////////////////////////////////////////////////
		IndexPointer(const void *ptr,unsigned char bytesPerIndex,const fg::Buffer *buffer = 0);
	};
	
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		/// @brief The default draw function
		/// 
		/// Warning: it uses old fixed functions
		/// and uses GL buffers, so if buffers arent available or shaders are available
		/// one should implement this function instead and replace it with setDrawFunc
		/// 
		/// @param pos The attributes of the position
		/// @param clr The attributes of the color
		/// @param texPos The attributes of the texture position
		/// @param norm The attributes of the normals
		/// @param indices The properties of he indices
		/// @param texture The texture to be used
		/// @param shader The shader to be used
		/// @param vertexCount The number of vertices to be drawn
		/// @param primitive The way vertices should be rasterized
		/// 
		/////////////////////////////////////////////////////////////
		void FRONTIER_API defaultDraw(const Attribute &pos,
									  const Attribute &clr,
									  const Attribute &texPos,
									  const Attribute &norm,
									  fm::Size vertexCount,
									  fg::Primitive primitive,
									  const Texture *texture,
									  const Shader *shader,
									  const IndexPointer &indices);

	}
	
	typedef void drawFuncType(const Attribute &,
							  const Attribute &,
							  const Attribute &,
							  const Attribute &,
							  fm::Size,
							  fg::Primitive,
							  const Texture *,
							  const Shader *,
							  const IndexPointer &); ///< The type of the drawer function
	
	/////////////////////////////////////////////////////////////
	/// @brief Replace the drawing function
	/// 
	/// The drawing function is thread-local
	/// Default is fg::priv::defaultDraw
	/// 
	/// @param func The new function (NULL means fg::priv::defaultDraw)
	/// 
	/////////////////////////////////////////////////////////////
	void FRONTIER_API setDrawFunc(drawFuncType *func);
	
	/////////////////////////////////////////////////////////////
	/// @brief Render using position and color
	/// 
	/// @param pos The attributes of the position
	/// @param clr The attributes of the color
	/// @param vertexCount The number of vertices to be drawn
	/// @param primitive The way vertices should be rasterized
	/// @param texture The texture to be used
	/// @param shader The shader to be used
	/// @param indices The properties of he indices
	/// 
	/////////////////////////////////////////////////////////////
	void FRONTIER_API draw(const Attribute &pos,
						   const Attribute &clr,
						   fm::Size vertexCount,
						   fg::Primitive primitive,
						   const Texture *texture = 0,
						   const Shader *shader = 0,
						   const IndexPointer &indices = IndexPointer());
	
	
	/////////////////////////////////////////////////////////////
	/// @brief Render using position, color and texture position
	/// 
	/// @param pos The attributes of the position
	/// @param clr The attributes of the color
	/// @param texPos The attributes of the texture position
	/// @param vertexCount The number of vertices to be drawn
	/// @param primitive The way vertices should be rasterized
	/// @param texture The texture to be used
	/// @param shader The shader to be used
	/// @param indices The properties of he indices
	/// 
	/////////////////////////////////////////////////////////////
	void FRONTIER_API draw(const Attribute &pos,
						   const Attribute &clr,
						   const Attribute &texPos,
						   fm::Size vertexCount,
						   fg::Primitive primitive,
						   const Texture *texture = 0,
						   const Shader *shader = 0,
						   const IndexPointer &indices = IndexPointer());
	
	
	/////////////////////////////////////////////////////////////
	/// @brief Render using position, color, texture position and normal vectors
	/// 
	/// @param pos The attributes of the position
	/// @param clr The attributes of the color
	/// @param texPos The attributes of the texture position
	/// @param norm The attributes of the normals
	/// @param vertexCount The number of vertices to be drawn
	/// @param primitive The way vertices should be rasterized
	/// @param texture The texture to be used
	/// @param shader The shader to be used
	/// @param indices The properties of he indices
	/// 
	/////////////////////////////////////////////////////////////
	void FRONTIER_API draw(const Attribute &pos,
						   const Attribute &clr,
						   const Attribute &texPos,
						   const Attribute &norm,
						   fm::Size vertexCount,
						   fg::Primitive primitive,
						   const Texture *texture = 0,
						   const Shader *shader = 0,
						   const IndexPointer &indices = IndexPointer());
	
	/////////////////////////////////////////////////////////////
	/// @brief Render vertices
	/// 
	/// @param vertices A pointer to the vertices
	/// @param vertexCount The number of vertices to be drawn
	/// @param primitive The way vertices should be rasterized
	/// @param texture The texture to be used
	/// @param shader The shader to be used
	/// @param indices The properties of he indices
	/// 
	/////////////////////////////////////////////////////////////
	template <class pt,class ct,class tpt,class nt>
	void draw(const fm::vertex<pt,ct,tpt,nt> *vertices,
			  fm::Size vertexCount,
			  fg::Primitive primitive,
			  const Texture *texture = 0,
			  const Shader *shader = 0,
			  const IndexPointer &indices = IndexPointer());
	
	/////////////////////////////////////////////////////////////
	/// @brief Render vertices
	/// 
	/// @param vertices A pointer to the vertices
	/// @param vertexCount The number of vertices to be drawn
	/// @param primitive The way vertices should be rasterized
	/// @param indices The properties of he indices
	/// 
	/////////////////////////////////////////////////////////////
	template <class pt,class ct,class tpt,class nt>
	void draw(const fm::vertex<pt,ct,tpt,nt> *vertices,
			  fm::Size vertexCount,
			  fg::Primitive primitive,
			  const IndexPointer &indices);
	
	/////////////////////////////////////////////////////////////
	/// @brief Render vertices
	/// 
	/// @param vertices A pointer to the vertices
	/// @param primitive The way vertices should be rasterized
	/// @param texture The texture to be used
	/// @param shader The shader to be used
	/// @param indices The properties of he indices
	/// 
	/////////////////////////////////////////////////////////////
	template <class pt,class ct,class tpt,class nt,fm::Size vertexCount>
	void draw(const fm::vertex<pt,ct,tpt,nt> (&vertices)[vertexCount],
			  fg::Primitive primitive,
			  const Texture *texture = 0,
			  const Shader *shader = 0);
	
			
	/////////////////////////////////////////////////////////////
	/// @brief Construct a Attribute using a type
	/// 
	/// Note: T must have components member enum (or static integer variable)
	/// T must also have a member typedef component_type
	/// 
	/// @param ptr The pointer to the data
	/// @param stride The number of bytes in a vertex (0 means sizeof(T))
	/// 
	/// @return The created Attribute
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	Attribute Attr(const T *ptr,unsigned short stride=0);
	
	/////////////////////////////////////////////////////////////
	/// @brief Construct a Attribute  using a buffer, offset and stride
	/// 
	/// Note: T must have components member enum (or static integer variable)
	/// T must also have a member typedef component_type
	/// 
	/// @param buf The buffer
	/// @param offset The offset in the buffer (in bytes)
	/// @param stride The bytes in a vertex
	/// 
	/// @return The created Attribute
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	Attribute Attr(fg::Buffer &buf,fm::Ptrdiff offset=0,unsigned char stride=0);

}
#endif //FRONTIER_DRAWABLE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Drawable.inl>
#endif
