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
#ifndef FRONTIER_VERTEX_HPP_INCLUDED
#define FRONTIER_VERTEX_HPP_INCLUDED
#define FRONTIER_VERTEX
namespace fm
{
	template<class> class vector4;
	template<class> class vector3;
	template<class> class vector2;
	typedef vector2<float> vec2;
	typedef vector3<float> vec3;
	typedef vector4<float> vec4;

	namespace priv
	{
		/////////////////////////////////////////////////////////////
		///
		/// 	@brief This class is used to indicate that
		///			   the vertex doesn't have a given field
		///
		/////////////////////////////////////////////////////////////
		class nullClass
		{
		public:
			typedef int value_type;
			enum {
				components = 0u ///< Public value indicating the amount of value_type's in the class
			};

			/////////////////////////////////////////////////////////////
			/// @brief This class can be converted to any class that has a default constructor
			///
			/////////////////////////////////////////////////////////////
			template<class T>
			operator T() const;
		};
	}

	/////////////////////////////////////////////////////////////
	/// @brief vertex class used to hold the attributes of a point
	/// 
	/// @ingroup System
	/// 
	/// The attributes consist of 
	///		@li a position of posType 
	/// 	@li a color of clrType
	/// 	@li a texture position of texPosType
	/// 	@li a normal vector of normType
	/// 
	/// Thanks to the specializations of the vertex class 
	/// it can be used without a normal and without a texture position
	/// 
	/// It is always laid out in memory as follows:<br/>
	/// 	|position||color||texture position (if has)||normal vector (if has)|
	/// 
	/// For further information read <a href="http://en.wikipedia.org/wiki/Vertex_%28computer_graphics%29">this wikipedia article</a>
	/// 
	/// This class can be used to store vertex data what will be sent to OpenGL
	/// 
	/////////////////////////////////////////////////////////////
	template<class posType,class clrType,class texPosType=priv::nullClass,class normType=priv::nullClass>
	class vertex
	{
	public:
		typedef vertex<posType,clrType,texPosType,normType> &reference;
		typedef const vertex<posType,clrType,texPosType,normType> &const_reference;
		posType pos;       ///< position attribute
		clrType clr;       ///< color attribute
		texPosType texPos; ///< texture position attribute
		normType norm;     ///< normal vector attribute

		//////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param pos Value to initialize @a pos with
		/// @param clr Value to initialize @a clr with
		/// @param texPos Value to initialize @a texPos with
		/// @param norm Value to initialize @a norm with
		/// 
		//////////////////////////////////
		vertex(const posType    &pos    = posType(),
			   const clrType    &clr	= clrType(),
			   const texPosType &texPos = texPosType(),
			   const normType   &norm   = normType());

		//////////////////////////////////
		/// @brief Copy-convert consructor
		/// 
		/// Initialize the vertex from @a vert,
		/// please note that posType2 must be convertible to posType, clrType2 to clrType ...
		/// 
		/// @param vert The vertex to initialize with
		/// 
		//////////////////////////////////
		template<class posType2,class clrType2,class texPosType2,class normType2>
		vertex(const vertex<posType2,clrType2,texPosType2,normType2> &vert);

		//////////////////////////////////
		/// @brief Change the value of the position and the color
		/// 
		/// @param _pos New value of the position
		/// @param _clr New value of the color
		/// 
		/// @return reference to itself
		/// 
		//////////////////////////////////
		reference operator()(const posType &_pos,const clrType &_clr);

		//////////////////////////////////
		/// @brief Change the value of the position, the color and the texture coordinate
		/// 
		/// @param _pos New value of the position
		/// @param _clr New value of the color
		/// @param _texPos New value of the texture position
		/// 
		/// @return reference to itself
		/// 
		//////////////////////////////////
		reference operator()(const posType &_pos,const clrType &_clr,const texPosType &_texPos);

		//////////////////////////////////
		/// @brief Change the value of the position, the color, the texture coordinate and the normal
		/// 
		/// @param _pos New value of the position
		/// @param _clr New value of the color
		/// @param _texPos New value of the texture position
		/// @param _norm New value of the normal
		/// 
		/// @return reference to itself
		/// 
		//////////////////////////////////
		reference operator()(const posType &_pos,const clrType &_clr,const texPosType &_texPos,const normType &_norm);


		//////////////////////////////////
		/// @brief Constants holding the size of the attributes in bytes
		/// 
		//////////////////////////////////
		enum {
			posSize = sizeof(posType),       ///< Sizeof posType
			clrSize = sizeof(clrType),		 ///< Sizeof clrType
			texPosSize = sizeof(texPosType), ///< Sizeof texPosType
			normSize = sizeof(normType)      ///< Sizeof normType
		};
	};


	//////////////////////////////////
	/// @brief Partial specialization of fm::vertex
	/// 
	/// This specialization has a position, a color and a texture position
	/// 
	//////////////////////////////////
	template<class posType,class clrType,class texPosType>
	class vertex<posType,clrType,texPosType,priv::nullClass>
	{
	public:
		typedef vertex<posType,clrType,texPosType,priv::nullClass> &reference;
		typedef const vertex<posType,clrType,texPosType,priv::nullClass> &const_reference;
		posType pos;       ///< position attribute
		clrType clr;       ///< color attribute
		texPosType texPos; ///< texture position attribute

		//////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param pos Value to initialize @a pos with
		/// @param clr Value to initialize @a clr with
		/// @param texPos Value to initialize @a texPos with
		/// 
		//////////////////////////////////
		vertex(const posType    &pos    = posType(),
			   const clrType    &clr	= clrType(),
			   const texPosType &texPos = texPosType());

		//////////////////////////////////
		/// @brief Copy-convert consructor
		/// 
		/// Initialize the vertex from @a vert,
		/// please note that posType2 must be convertible to posType, clrType2 to clrType ...
		/// 
		/// @param vert The vertex to initialize with
		/// 
		//////////////////////////////////
		template<class posType2,class clrType2,class texPosType2>
		vertex(const vertex<posType2,clrType2,texPosType2> &vert);

		//////////////////////////////////
		/// @brief Change the value of the position and the color
		/// 
		/// @param _pos New value of the position
		/// @param _clr New value of the color
		/// 
		/// @return reference to itself
		/// 
		//////////////////////////////////
		reference operator()(const posType &_pos,const clrType &_clr);

		//////////////////////////////////
		/// @brief Change the value of the position, the color and the texture coordinate
		/// 
		/// @param _pos New value of the position
		/// @param _clr New value of the color
		/// @param _texPos New value of the texture position
		/// 
		/// @return reference to itself
		/// 
		//////////////////////////////////
		reference operator()(const posType &_pos,const clrType &_clr,const texPosType &_texPos);


		//////////////////////////////////
		/// @brief Constants holding the size of the attributes in bytes
		/// 
		//////////////////////////////////
		enum {
			posSize = sizeof(posType),       ///< Sizeof posType
			clrSize = sizeof(clrType),		 ///< Sizeof clrType
			texPosSize = sizeof(texPosType), ///< Sizeof texPosType
			normSize = 0      				 ///< 0 (no normal vector)
		};
	};


	//////////////////////////////////
	/// @brief Partial specialization of fm::vertex
	/// 
	/// This specialization has a position and a color
	/// 
	//////////////////////////////////
	template<class posType,class clrType>
	class vertex<posType,clrType,priv::nullClass,priv::nullClass>
	{
	public:
		typedef vertex<posType,clrType,priv::nullClass,priv::nullClass> &reference;
		typedef const vertex<posType,clrType,priv::nullClass,priv::nullClass> &const_reference;
		posType pos;       ///< position attribute
		clrType clr;       ///< color attribute

		//////////////////////////////////
		/// @brief Default constructor
		/// 
		/// @param pos Value to initialize @a pos with
		/// @param clr Value to initialize @a clr with
		/// 
		//////////////////////////////////
		vertex(const posType    &pos    = posType(),
			   const clrType    &clr	= clrType());

		//////////////////////////////////
		/// @brief Copy-convert consructor
		/// 
		/// Initialize the vertex from @a vert,
		/// please note that posType2 must be convertible to posType, clrType2 to clrType ...
		/// 
		/// @param vert The vertex to initialize with
		/// 
		//////////////////////////////////
		template<class posType2,class clrType2,class texPosType2,class normType2>
		vertex(const vertex<posType2,clrType2,texPosType2,normType2> &vert);


		//////////////////////////////////
		/// @brief Change the value of the position and the color
		/// 
		/// @param _pos New value of the position
		/// @param _clr New value of the color
		/// 
		/// @return reference to itself
		/// 
		//////////////////////////////////
		reference operator()(const posType &_pos,const clrType &_clr);


		//////////////////////////////////
		/// @brief Constants holding the size of the attributes in bytes
		/// 
		//////////////////////////////////
		enum {
			posSize = sizeof(posType),       ///< Sizeof posType
			clrSize = sizeof(clrType),		 ///< Sizeof clrType
			texPosSize = 0, 				 ///< 0 (no texture position)
			normSize = 0      				 ///< 0 (no normal vector)
		};
	};

	typedef vertex<vec3,vec4> 			vert3f;     ///< Typedef of fm::vertex that has vec3 position vec4 color 
	typedef vertex<vec3,vec4,vec2> 		vertex3f;   ///< Typedef of fm::vertex that has vec3 position vec4 color vec2 as texture position
	typedef vertex<vec3,vec4,vec2> 		vertex3ft;  ///< Typedef of fm::vertex that has vec3 position vec4 color vec2 as texture position
	typedef vertex<vec3,vec4,vec2,vec3> vertex3ftn; ///< Typedef of fm::vertex that has vec3 position vec4 color vec2 as texture position vec3 as normal vector
	typedef vertex<vec2,vec4> 			vert2f;     ///< Typedef of fm::vertex that has vec2 position vec4 color 
	typedef vertex<vec2,vec4,vec2> 		vertex2f;   ///< Typedef of fm::vertex that has vec2 position vec4 color vec2 as texture position
	typedef vertex<vec2,vec4,vec2> 		vertex2ft;  ///< Typedef of fm::vertex that has vec2 position vec4 color vec2 as texture position
	typedef vertex<vec2,vec4,vec2,vec3> vertex2ftn; ///< Typedef of fm::vertex that has vec2 position vec4 color vec2 as texture position vec3 as normal vector


}
#endif // FRONTIER_VERTEX_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Vertex.inl>
#endif
