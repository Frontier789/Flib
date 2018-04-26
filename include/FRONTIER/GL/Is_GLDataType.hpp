////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_Is_GLDataType_INCLUDED
#define FRONTIER_Is_GLDataType_INCLUDED
#include <FRONTIER/GL/GL_TYPES.hpp>
#include <type_traits>

namespace fg
{
	namespace priv {
		template<class T>
		class HasCompSFINAE
		{
			typedef int YES;
			typedef char NO;
			
			template<size_t,class>
			class Helper;
			
			template<class U>
			static YES test(Helper<U::components,typename U::component_type> *);
			
			template<class>
			static NO test(...);
			
		public:
			enum {
				value = std::is_same<decltype(test<T>(nullptr)),YES>::value
			};
		};
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a OpenGL Data Type
	///
	/////////////////////////////////////////////////////////////
	template<class T,bool = priv::HasCompSFINAE<T>::value>
	class Is_GLDataType
	{
	public:
		enum {
			value     /** @cond DOXYGEN_HIDE */ = false /** @endcond */ ,///< True if T is a OpenGL Data Type
			enumVal   /** @cond DOXYGEN_HIDE */ = 0x0 /* GL_NONE */ /** @endcond */ ,///< The corresponding OpenGL value
			compCount /** @cond DOXYGEN_HIDE */ = 0 /* 0 */ /** @endcond */ ///< The number of components in one item  
		};
	};
	
	/// @cond DOXYGEN_HIDE
	template<>
	class Is_GLDataType<GLbyte,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1400, // GL_BYTE
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLubyte,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1401, // GL_UNSIGNED_BYTE
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLshort,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1402, // GL_SHORT
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLushort,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1403, // GL_UNSIGNED_SHORT
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLint,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1404, // GL_INT
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLuint,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1405, // GL_UNSIGNED_INT
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLfloat,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x1406, // GL_FLOAT
			compCount = 1
		};
	};

	template<>
	class Is_GLDataType<GLdouble,false>
	{
	public:
		enum {
			value     = true,
			enumVal   = 0x140a, // GL_DOUBLE
			compCount = 1
		};
	};
	
	template<class T>
	class Is_GLDataType<T,true>
	{
	public:
		enum {
			value     = Is_GLDataType<typename T::component_type>::value,
			enumVal   = Is_GLDataType<typename T::component_type>::enumVal,
			compCount = Is_GLDataType<typename T::component_type>::compCount * T::components
		};
	};
	
	template<class T,fm::Size S>
	class Is_GLDataType<T[S],false>
	{
	public:
		enum {
			value     = Is_GLDataType<T>::value,
			enumVal   = Is_GLDataType<T>::enumVal,
			compCount = Is_GLDataType<T>::compCount * S
		};
	};
	/// @endcond
}

#endif // FRONTIER_Is_GLDataType_INCLUDED
