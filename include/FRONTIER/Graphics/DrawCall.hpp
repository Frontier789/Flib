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
#ifndef FRONTIER_DRAW_CALL_HPP_INCLUDED
#define FRONTIER_DRAW_CALL_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/System/util/API.h>

#define FRONTIER_DRAW_CALL

namespace fg
{
	class IndexHolder;
	class Buffer;
	class Mesh;
	
	/////////////////////////////////////////////////////////////
	/// @brief Class used to store a way of making a drawing call to the GL
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API DrawCall
	{
	public:
		fg::Primitive primitive; ///< The primitve to be used
		fm::Size componentType;  ///< The OpenGL id of the type of the indices
		fm::Size indexCount;     ///< The number of indices available
		fm::Size drawBeg; ///< The first index to be used
		fm::Size drawLen; ///< The number of indices to be used
		fg::Buffer *buf;  ///< The buffer that holds the indices
		bool ownBuffer;   ///< Stores whether the buffer was created by this class

		/////////////////////////////////////////////////////////////
		/// @brief Construct a drawcall from its data
		/// 
		/// @param primitive The primitve to be used
		/// @param indexCount The number of indices available
		/// @param drawBeg The first index to be used
		/// @param drawLen The number of indices to be used
		/// @param compType The OpenGL id of the type of the indices
		/// @param buffer The buffer that holds the indices
		/// @param ownBuffer Stores whether the buffer was created by this class
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall(fg::Primitive primitive = fg::Triangles,
				 fm::Size indexCount = 0,
				 fm::Size drawBeg = 0,
				 fm::Size drawLen = 0,
				 fm::Size compType = 0,
				 fg::Buffer *buffer = nullptr,
				 bool ownBuffer = true);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~DrawCall();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		/// 
		/// @param drawCall The call to be copied
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall(const DrawCall &drawCall) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		/// 
		/// @param drawCall The call to be moved
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall(DrawCall &&drawCall);

		/////////////////////////////////////////////////////////////
		/// @brief Copy assignment operator
		/// 
		/// @param drawCall The call to be copied
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &operator=(const DrawCall &drawCall) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move assignment operator
		/// 
		/// @param drawCall The call to be moved
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &operator=(DrawCall &&drawCall);

		/////////////////////////////////////////////////////////////
		/// @brief Swap two drawcalls
		/// 
		/// @param drawCall The drawcall to swap with
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &swap(DrawCall &drawCall);

		/////////////////////////////////////////////////////////////
		/// @brief Assign index-data to the drawcall
		/// 
		/// @param data The data to be assigned
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size N>
		inline DrawCall &operator=(const T (&data)[N]);

		/////////////////////////////////////////////////////////////
		/// @brief Assign index-data to the drawcall
		/// 
		/// @param indices The data to be assigned
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &operator=(const IndexHolder &indices);

		/////////////////////////////////////////////////////////////
		/// @brief Assign index-data and primitive to the drawcall
		/// 
		/// @param data The data to be assigned
		/// @param primitive The primitive to be used
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size N>
		inline DrawCall &set(const T (&data)[N],fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Assign index-data and primitive to the drawcall
		/// 
		/// @param pointer Pointer to the data to be assigned
		/// @param N The number of indices to assign
		/// @param primitive The primitive to be used
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		inline DrawCall &set(const T *pointer,fm::Size N,fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Assign index-data and primitive to the drawcall
		/// 
		/// @param ptr Pointer to the data to be assigned
		/// @param indCount The number of indices to assign
		/// @param compType The OpenGL id of the type of the indices
		/// @param bytesToCopy The number of bytes to be copied
		/// @param primitive The primitive to be used
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &set(const void *ptr,fm::Size indCount,fm::Size compType,fm::Size bytesToCopy,fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Assign index-data and primitive to the drawcall
		/// 
		/// @param indices The data to be assigned
		/// @param primitive The primitive to be used
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &set(const IndexHolder &indices,fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Assign range-data and primitive to the drawcall
		/// 
		/// @param beg The first index to used
		/// @param len The number of indices to use
		/// @param primitive The primitive to be used
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &set(fm::Size beg,fm::Size len,fg::Primitive primitive);

		/////////////////////////////////////////////////////////////
		/// @brief Assign range-data to the drawcall
		/// 
		/// @param beg The first index to used
		/// @param len The number of indices to use
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		DrawCall &setRange(fm::Size beg,fm::Size len);
	};
}

#endif // FRONTIER_DRAW_CALL_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/DrawCall.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
