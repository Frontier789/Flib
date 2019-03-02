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
#ifndef FRONTIER_BUFFER_HPP_INCLUDED
#define FRONTIER_BUFFER_HPP_INCLUDED
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/HeavyToCopy.hpp>
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Ref.hpp>
#define FRONTIER_BUFFER

namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Type used to specify OpenGL buffer-usage
	///
	/////////////////////////////////////////////////////////////
	enum BufferType {
		ArrayBuffer = 0x8892, ///< Means that the buffer is generic (may hold position color etc)
		IndexBuffer = 0x8893, ///< Means that the buffer holds indices
		ShaderStorageBuffer = 0x90D2 ///< Indicates a buffer for plain shader input output 
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Values used to hint OpenGL on the usage of the data
	///
	/////////////////////////////////////////////////////////////
	enum BufferUsage {
		StaticDraw  = 0x88e4, ///< The application will send data to OpenGL once        (Please note that this is only a hint)
		StaticRead  = 0x88e5, ///< OpenGL will send data to the application once        (Please note that this is only a hint)
		StaticCopy  = 0x88e6, ///< Data will be transferred inside OpenGL once          (Please note that this is only a hint)
		DynamicDraw = 0x88e8, ///< The application will send data to OpenGL frequently  (Please note that this is only a hint)
		DynamicRead = 0x88e9, ///< OpenGL will send data to the application frequently  (Please note that this is only a hint)
		DynamicCopy = 0x88ea, ///< Data will be transferred inside OpenGL frequently    (Please note that this is only a hint)
		StreamDraw  = 0x88e0, ///< The application will send data to OpenGL per frame   (Please note that this is only a hint)
		StreamRead  = 0x88e1, ///< OpenGL will send data to the application per frame   (Please note that this is only a hint)
		StreamCopy  = 0x88e2  ///< Data will be transferred inside OpenGL per frame     (Please note that this is only a hint)
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Encode an access policy for buffer mapping
	///
	/////////////////////////////////////////////////////////////
	enum BufferAccess {
		BufferReadOnly  = 0x88b8,
		BufferWriteOnly = 0x88b9,
		BufferReadWrite = 0x88ba
	};

	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle <a href="http://www.opengl.org/wiki/Buffer_Object">OpenGL buffer objects</a>
	///
	/// @ingroup Graphics
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Buffer : public GlObject
	{
	private:
		BufferType m_type;    ///< Type of the buffer
		BufferUsage m_usage;  ///< Hinting for OpenGL on buffer usage
		fm::Size m_compType;  ///< The OpenGL id of the type of components
		fm::Size m_compCount; ///< The number of components
		fm::Size m_byteCount; ///< Number of bytes stored
		void init(); ///< Internal function used to create the OpenGL id
		
		fm::Result mapData(void *&ptr,BufferAccess access); ///< Internal helper map function
		fm::Result mapData(void *&ptr,fm::Size beg,fm::Size len,BufferAccess access); ///< Internal helper map function
		fm::Result cpyData(void *ptr,fm::Size beg,fm::Size len); ///< Internal helper copy function

	public:
		typedef Buffer &reference;
		typedef const Buffer &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the component type enum
		///
		/// @return The OpenGL id of the type of components
		///
		/////////////////////////////////////////////////////////////
		fm::Size getCompType() const; 
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of components in an item
		///
		/// @return The number of components
		///
		/////////////////////////////////////////////////////////////
		fm::Size getCompCount() const; 
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of items in the buffer
		///
		/// @return The number of items
		///
		/////////////////////////////////////////////////////////////
		fm::Size getItemCount() const; 
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of bytes an item occupies
		///
		/// @return The number of bytes per item
		///
		/////////////////////////////////////////////////////////////
		fm::Size getItemSize() const; 

		/////////////////////////////////////////////////////////////
		/// @brief Destroy the buffer
		///
		/// Releases all OGL data
		///
		/////////////////////////////////////////////////////////////
		fm::Result destroy();

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Buffer();

		/////////////////////////////////////////////////////////////
		/// @brief Copy constructor
		///
		/// Create a new buffer as a copy of an existing one
		///
		/// @param buf The buffer to copy
		///
		/////////////////////////////////////////////////////////////
		Buffer(const Buffer &buf) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Move constructor
		///
		/// @param buf The buffer to move
		///
		/////////////////////////////////////////////////////////////
		Buffer(Buffer &&buf);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// If the OpenGL id is valid it will be
		/// deleted automatically
		///
		/////////////////////////////////////////////////////////////
		~Buffer();

		/////////////////////////////////////////////////////////////
		/// @brief Construct buffer with given type
		///
		/// The object is uninitialized (invalid) after this call
		///
		/// @param type The type of the new buffer
		/// @param usage The usage hint of the new buffer
		///
		/////////////////////////////////////////////////////////////
		explicit Buffer(BufferType type,BufferUsage = StaticDraw);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the type of the buffer
		///
		/// @param type The new type of the buffer
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		Buffer &setType(BufferType type);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the usage of the buffer
		///
		/// @param usage The new usage of the buffer
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		Buffer &setUsage(BufferUsage usage);

		/////////////////////////////////////////////////////////////
		/// @brief Set whole data of the buffer
		///
		/// Resizes the buffer if necessary
		///
		/// @param ptr Pointer to the data
		/// @param compType The GL enum of the type of data
		/// @param compCount The number of components per data
		/// @param byteCount The number of BYTES (not items) to upload
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result setData(const void *ptr,fm::Size compType,fm::Size compCount,fm::Size byteCount);

		/////////////////////////////////////////////////////////////
		/// @brief Set whole data of the buffer
		///
		/// Resizes the buffer if necessary
		///
		/// @param data The data to be sent to OpenGL
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size S>
		fm::Result setData(const T (&data)[S]);

		/////////////////////////////////////////////////////////////
		/// @brief Set whole data of the buffer
		///
		/// Resizes the buffer if necessary
		///
		/// @param data The data to be sent to OpenGL
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size S>
		fm::Result operator=(const T (&data)[S]);

		/////////////////////////////////////////////////////////////
		/// @brief Set whole data of the buffer
		///
		/// Resizes the buffer if necessary
		///
		/// @param data Pointer to the data to be sent to OpenGL
		/// @param dataCount The number of Ts in data
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result setData(const T *data,fm::Size dataCount);

		/////////////////////////////////////////////////////////////
		/// @brief Get data from the buffer
		///
		/// Downloads data from the GPU
		/// The caller must allocate the memory
		///
		/// @param data The data returned
		/// @param offset The offset of the read in items
		/// @param length The length of the read in items
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result getData(const T *data,fm::Size offset,fm::Size length);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set part of the data in the buffer
		/// 
		/// Does not create or resize buffer automatically
		/// 
		/// @param ptr Pointer to The data to be updated
		/// @param byteCount The number of BYTES (not items) to upload
		/// @param byteOffset Offset for the data to be uploaded in bytes
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result updateData(const void *ptr,fm::Size byteCount,fm::Size byteOffset = 0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set part of the data in the buffer
		/// 
		/// Does not create or resize buffer automatically
		///
		/// @param data The data to be sent to OpenGL
		/// @param byteOffset Offset for the data to be uploaded in bytes
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T,fm::Size S>
		fm::Result updateData(const T (&data)[S],fm::Size byteOffset = 0);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set data of the buffer
		/// 
		/// Does not create or resize buffer automatically
		///
		/// @param data Pointer to the data to be sent to OpenGL
		/// @param dataCount The number of Ts in data
		/// @param byteOffset Offset for the data to be uploaded in bytes
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		template<class T>
		fm::Result updateData(const T *data,fm::Size dataCount,fm::Size byteOffset = 0);

		/////////////////////////////////////////////////////////////
		/// @brief Map the whole buffer for read/write
		///
		/// Returns nullptr on error
		///
		/// @param access The access policy for the mapping
		///
		/// @return A pointer to the mapped memory (nullptr on error)
		///
		/////////////////////////////////////////////////////////////
		template<class T = void>
		T *map(BufferAccess access = BufferReadWrite);

		/////////////////////////////////////////////////////////////
		/// @brief Map the whole buffer for read/write
		///
		/// If read is false the data is not copied from GPU
		/// If write is false the data is not copied to GPU
		///
		/// Returns nullptr on error
		///
		/// @param ptr The pointer returned (not mdified on error)
		/// @param access The access policy for the mapping
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		template<class T = void>
		fm::Result map(T *&ptr,BufferAccess access = BufferReadWrite);

		/////////////////////////////////////////////////////////////
		/// @brief Map a range of the buffer for read/write
		///
		/// Returns nullptr on error
		///
		/// @param beg The number of items to skip at the beginning
		/// @param end The number of items to map
		/// @param access The access policy for the mapping
		///
		/// @return A pointer to the mapped memory (nullptr on error)
		///
		/////////////////////////////////////////////////////////////
		template<class T = void>
		T *map(fm::Size beg,fm::Size len,BufferAccess access = BufferReadWrite);

		/////////////////////////////////////////////////////////////
		/// @brief Map a range of the buffer for read/write
		///
		/// If read is false the data is not copied from GPU
		/// If write is false the data is not copied to GPU
		///
		/// Returns nullptr on error
		///
		/// @param ptr The pointer returned (not mdified on error)
		/// @param beg The number of items to skip at the beginning
		/// @param end The number of items to map
		/// @param access The access policy for the mapping
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		template<class T = void>
		fm::Result map(T *&ptr,fm::Size beg,fm::Size len,BufferAccess access = BufferReadWrite);

		/////////////////////////////////////////////////////////////
		/// @brief Unmap the mapped buffer
		///
		/// @return The result of the operation
		///
		/////////////////////////////////////////////////////////////
		fm::Result unMap();

		/////////////////////////////////////////////////////////////
		/// @brief Get the type of the buffer
		///
		/// @return The type
		///
		/////////////////////////////////////////////////////////////
		BufferType getType() const;

		/////////////////////////////////////////////////////////////
		/// @brief Get the usage of the buffer
		///
		/// @return The usage
		///
		/////////////////////////////////////////////////////////////
		BufferUsage getUsage() const;

		/////////////////////////////////////////////////////////////
		/// @brief Bind the buffer to the binding defined by its type
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result bind() const;

		/////////////////////////////////////////////////////////////
		/// @brief Bind the buffer to a given binding point
		///
		/// @param type The binding point
		/// 
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result bind(BufferType type) const;

		/////////////////////////////////////////////////////////////
		/// @brief Binary operator =
		///
		/// Set the buffer up as a copy of another
		///
		/// @param buf The buffer to copy
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		Buffer &operator=(const Buffer &buf) FRONTIER_HEAVYCOPY_QUALIFIER;

		/////////////////////////////////////////////////////////////
		/// @brief Binary operator =
		///
		/// @param buf The buffer to move
		///
		/// @return reference to itself
		///
		/////////////////////////////////////////////////////////////
		Buffer &operator=(Buffer &&buf);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a buffer to a given binding point
		///
		/// @param buffer The buffer to bind
		/// @param type The type to bind the buffer to
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(fm::Ref<const Buffer> buffer,BufferType type);

		/////////////////////////////////////////////////////////////
		/// @brief Bind a buffer to the binding defined by its type
		///
		/// @param buffer The buffer to bind
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result bind(const Buffer &buffer);

		/////////////////////////////////////////////////////////////
		/// @brief Release binding on a type of buffer
		///
		/// @param type The type on the binding to be released
		///
		/// @return the error-state of the function
		///
		/////////////////////////////////////////////////////////////
		static fm::Result unBind(BufferType type);

		/////////////////////////////////////////////////////////////
		/// @brief Check if buffers can be used
		/// 
		/// @return True iff gpu side buffers are available
		/// 
		/////////////////////////////////////////////////////////////
		static bool isAvailable();
		
		/////////////////////////////////////////////////////////////
		/// @brief Swap the content of the two objects
		///
		/// swaps in constant time
		///
		/// @param buf The object to swap with
		///
		/// @return Reference to itself
		///
		/////////////////////////////////////////////////////////////
		reference swap(Buffer &buf);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create a hard copy of the buffer object
		/// 
		/// @return A brand new buffer with the same properties as the original
		///
		/////////////////////////////////////////////////////////////
		Buffer makeCopy() const;
	};

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator ==
	///
	/// This function does not check if the two buffers' content is the
	/// same just compares their ids
	///
	/// @param left Left operand (Buffer)
	/// @param right Right operand (Buffer)
	///
	/// @return True if the two Buffers' ids are equal
	///
	/////////////////////////////////////////////////////////////
	bool FRONTIER_API operator==(const Buffer &left,const Buffer &right);

	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator !=
	///
	/// This function does not check if the two buffers' content is the
	/// same just compares their ids
	///
	/// @param left Left operand (Buffer)
	/// @param right Right operand (Buffer)
	///
	/// @return True if the two Buffers' ids are not equal
	///
	/////////////////////////////////////////////////////////////
	bool FRONTIER_API operator!=(const Buffer &left,const Buffer &right);
}

#endif //FRONTIER_BUFFER_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Buffer.inl>
#endif
