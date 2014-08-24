#ifndef FRONTIER_BUFFER_HPP_INCLUDED
#define FRONTIER_BUFFER_HPP_INCLUDED
#include <FRONTIER/Graphics/GlObject.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#define FRONTIER_BUFFER
#include <cstddef>
namespace fg
{
	/////////////////////////////////////////////////////////////
	/// @brief Type used to hint OpenGL on buffer-usage
	/////////////////////////////////////////////////////////////
	enum BufferType{};

	/////////////////////////////////////////////////////////////
	/// @brief Values used to hint OpenGL on the usage of the data
	/////////////////////////////////////////////////////////////
	enum Usage{};
	
	extern BufferType ArrayBuffer; ///< Means that the buffer is generic (may hold position color etc)
	extern BufferType IndexBuffer; ///< Means that the buffer holds indices
	
	/////////////////////////////////////////////////////////////
	/// 
	/// 	@brief Class used to handle <a href="http://www.opengl.org/wiki/Buffer_Object">OpenGL buffer objects</a>
	/// 
	/////////////////////////////////////////////////////////////
	class Buffer : public GlObject, public fm::NonCopyable
	{
	public:
		static Usage StaticDraw;  ///< The application will send data to OpenGL once        (Please note that this is only a hint)
		static Usage StaticRead;  ///< OpenGL will send data to the application once        (Please note that this is only a hint)
		static Usage StaticCopy;  ///< Data will be transferred inside OpenGL once          (Please note that this is only a hint)
		static Usage DynamicDraw; ///< The application will send data to OpenGL frequently  (Please note that this is only a hint)
		static Usage DynamicRead; ///< OpenGL will send data to the application frequently  (Please note that this is only a hint)
		static Usage DynamicCopy; ///< Data will be transferred inside OpenGL frequently    (Please note that this is only a hint)
		static Usage StreamDraw;  ///< The application will send data to OpenGL per frame   (Please note that this is only a hint)
		static Usage StreamRead;  ///< OpenGL will send data to the application per frame   (Please note that this is only a hint)
		static Usage StreamCopy;  ///< Data will be transferred inside OpenGL per frame     (Please note that this is only a hint)
	private:
		BufferType m_type;		   ///< Type of the buffer
		Usage m_usage;			   ///< Hinting for OpenGL on buffer usage
		void init(); ///< Iternal function used to create the OpenGL id
	public:
		typedef Buffer &reference;
		typedef const Buffer &const_reference;
	
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/// The object is uninitialized (invalid) after this call
		/// 
		/////////////////////////////////////////////////////////////
		Buffer();
	
		/////////////////////////////////////////////////////////////
		/// @brief Construct buffer with given type
		/// 
		/// The object is uninitialized (invalid) after this call
		/// 
		/// @param type The type of the new buffer
		/// @param usage The usage hint of the new buffer
		/// 
		/////////////////////////////////////////////////////////////
		explicit Buffer(BufferType type,Usage usage=StaticDraw);
	
		/////////////////////////////////////////////////////////////
		/// @brief Construct buffer with given type
		/// 
		/// The object is uninitialized (invalid) after this call
		/// 
		/// @param usage The usage hint of the new buffer
		/// @param type The type of the new buffer
		/// 
		/////////////////////////////////////////////////////////////
		explicit Buffer(Usage usage,BufferType type=ArrayBuffer);
	
		/////////////////////////////////////////////////////////////
		/// @brief Construct buffer with given type
		/// 
		/// The object is initialized (valid) after this call
		/// 
		/// @param data Pointer to the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes to copy
		/// @param type The type of the new buffer
		/// @param usage The usage hint of the new buffer
		/// 
		/////////////////////////////////////////////////////////////
		Buffer(const void *data,std::size_t bytesToCopy,BufferType type=ArrayBuffer,Usage usage=StaticDraw);
	
		/////////////////////////////////////////////////////////////
		/// @brief Construct buffer with given type
		/// 
		/// The object is initialized (valid) after this call
		/// 
		/// @param data Pointer to the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes to copy
		/// @param usage The usage hint of the new buffer
		/// @param type The type of the new buffer
		/// 
		/////////////////////////////////////////////////////////////
		Buffer(const void *data,std::size_t bytesToCopy,Usage usage,BufferType type=ArrayBuffer);
	
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		/// 
		/// If the OpenGL id is valid it will be
		/// deleted automatically
		/// 
		/////////////////////////////////////////////////////////////
		~Buffer();
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data of the buffer
		/// 
		/// If OpenGL runs out of memory an error will
		/// be prompted to fg_log and the object will
		/// become invalid
		/// 
		/// @param data The data to be sent to OpenGL
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,std::size_t S>
		reference setData(const T (&data)[S]);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data and the usage hint of the buffer
		/// 
		/// If OpenGL runs out of memory an error will
		/// be prompted to fg_log and the object will
		/// become invalid
		/// 
		/// @param data The data to be sent to OpenGL
		/// @param usage Usage hint for OpenGL
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,std::size_t S>
		reference setData(const T (&data)[S],Usage usage);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data of the buffer
		/// 
		/// If OpenGL runs out of memory an error will
		/// be prompted to fg_log and the object will
		/// become invalid
		/// 
		/// @param data The data to be sent to OpenGL
		/// @param type The type to change the current type of the buffer
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,std::size_t S>
		reference setData(const T (&data)[S],BufferType type);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data, AccessFrequency and Usage
		/// 
		/// @param data The data to be sent to OpenGL
		/// @param type The type to change the current type of the buffer
		/// @param usage Usage hint for OpenGL
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T,std::size_t S>
		reference setData(const T (&data)[S],BufferType type,Usage usage);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data of the buffer
		/// 
		/// @param data Pointer the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes in the data
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setData(const void *data,std::size_t bytesToCopy);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data and type
		/// 
		/// @param data Pointer the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes in the data
		/// @param type The new type of the buffer
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setData(const void *data,std::size_t bytesToCopy,BufferType type);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data and usage hint
		/// 
		/// @param data Pointer the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes in the data
		/// @param usage Usage hint for OpenGL
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setData(const void *data,std::size_t bytesToCopy,Usage usage);
	
		/////////////////////////////////////////////////////////////
		/// @brief Set data, type and Usage
		/// 
		/// @param data Pointer the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes in the data
		/// @param type The new type of the buffer
		/// @param usage Usage hint for OpenGL
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setData(const void *data,std::size_t bytesToCopy,BufferType type,Usage usage);
		
		/////////////////////////////////////////////////////////////
		/// @brief Change data in the buffer
		/// 
		/// @param data Pointer the data to be sent to OpenGL
		/// @param bytesToCopy Number of bytes in the data
		/// @param byteOffset The offset in the buffer, where the data will be uploaded
		/// 
		/// @return reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference updateData(const void *data,std::size_t bytesToCopy,std::size_t byteOffset=0);
	
		/////////////////////////////////////////////////////////////
		/// @brief Bind the buffer for usage
		/// 
		/// The buffer should be valid when binding or else OpenGL will generate an error
		/// that will be prompted to fg_log
		/// 
		/// @param targetType The type to bind the buffer to
		/// 
		/////////////////////////////////////////////////////////////
		void bind(BufferType targetType) const;
	
		/////////////////////////////////////////////////////////////
		/// @brief Bind the buffer for usage
		/// 
		/// The buffer should be valid when binding or else OpenGL will generate an error
		/// that will be prompted to fg_log
		/// 
		/// The type the buffer will be bound is defined when
		/// When setData or the constructor is called
		/// 
		/////////////////////////////////////////////////////////////
		void bind() const;
	
		/////////////////////////////////////////////////////////////
		/// @brief Bind a buffer for usage
		/// 
		/// The buffer should be valid when binding or else OpenGL will generate an error
		/// that will be prompted to fg_log
		/// 
		/// @param buffer The buffer to bind
		/// @param targetType The type to bind the buffer to
		/// 
		/////////////////////////////////////////////////////////////
		static void bind(const Buffer &buffer,BufferType targetType);
	
		/////////////////////////////////////////////////////////////
		/// @brief Bind a buffer for usage
		/// 
		/// The buffer should be valid when binding or else OpenGL will generate an error
		/// that will be prompted to fg_log
		/// 
		/// @param buffer The buffer to bind
		/// @param targetType The type to bind the buffer to
		/// 
		/////////////////////////////////////////////////////////////
		static void bind(const Buffer *buffer,BufferType targetType);
	
		/////////////////////////////////////////////////////////////
		/// @brief Bind a buffer for usage
		/// 
		/// The buffer should be valid when binding or else OpenGL will generate an error
		/// that will be prompted to fg_log
		/// The buffer will be bound to the target specifed
		/// by its constructor or the last call on setData
		/// 
		/// @param buffer The buffer to bind
		/// 
		/////////////////////////////////////////////////////////////
		static void bind(const Buffer &buffer);
	
		/////////////////////////////////////////////////////////////
		/// @brief Release binding on a type of buffer
		/// 
		/// @param targetType The type on the binding to be released
		/// 
		/////////////////////////////////////////////////////////////
		static void unBind(BufferType targetType);
	
		/////////////////////////////////////////////////////////////
		/// @brief Check if buffers can be used
		/// 
		/// Due to OpenGL version buffers might not be available
		/// therefore one should check if they are available
		/// 
		/////////////////////////////////////////////////////////////
		static bool isAvailable();
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator ==
	/// 
	/// This function does not check if the two buffers' content is the
	/// same just compares their id
	/// 
	/// @param left Left operand (Buffer)
	/// @param right Right operand (Buffer)
	/// 
	/// @return True if the two Buffers' ids are equal
	/// 
	/////////////////////////////////////////////////////////////
	bool operator==(const Buffer &left,const Buffer &right);
	
	/////////////////////////////////////////////////////////////
	/// @brief Overload of binary operator !=
	/// 
	/// This function does not check if the two buffers' content is the
	/// same just compares their id
	/// 
	/// @param left Left operand (Buffer)
	/// @param right Right operand (Buffer)
	/// 
	/// @return True if the two Buffers' ids are not equal
	/// 
	/////////////////////////////////////////////////////////////
	bool operator!=(const Buffer &left,const Buffer &right);
}

#endif //FRONTIER_BUFFER_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/Buffer.inl>
#endif

////////////////////////////////////////////////////////////
/// @class fg::Buffer
/// @ingroup Graphics
///
/// fg::Buffer is an interface used to handle OpenGL buffers
/// It can be used send draw data to OpenGL
///
/// Usage example:
/// Define a red circle with r=10, send the data to OpenGL and use it to draw
/// @code
/// 
/// fm::vec2 circlePoints[20];
/// fm::vec3 circleColors[20];
/// 
/// for (int i=0;i<20;i++)
/// 	circlePoints[i] = fm::pol2(10,fm::deg(i/19.f*360.f)),
/// 	circleColors[i] = fm::vec4::Red;
/// 
/// fg::Buffer circlePbuf(fg::ArrayBuffer),circleCbuf(fg::ArrayBuffer);
/// 
/// circlePbuf.setData(circlePoints);
/// circleCbuf.setData(circleColors);
/// 
/// circlePbuf.bind();
/// glVertexPointer(2,GL_FLOAT,0,0);
/// circleCbuf.bind();
/// glColorPointer(3,GL_FLOAT,0,0);
/// 
/// glDrawArrays(fg::LineLoop,0,20);
/// 
/// @endcode
///
/// @see fg::GlObject
///
////////////////////////////////////////////////////////////