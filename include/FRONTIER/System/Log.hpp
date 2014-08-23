#ifndef FRONTIER_LOG_HPP_INCLUDED
#define FRONTIER_LOG_HPP_INCLUDED
#define FRONTIER_LOG
namespace std
{
    template<typename,typename>
    class basic_ostream;
    template<typename,typename>
    class basic_istream;
    template<typename,typename>
    class basic_ios;
    template<typename>
    class char_traits;
    class ios_base;
    typedef basic_ostream<char, std::char_traits<char> > ostream;
    template <class charT, class traits>
	basic_ostream<charT,traits> &endl(basic_ostream<charT,traits> &);
}
namespace fm
{
	/////////////////////////////////////////////////////////////
	///
	/// 	@brief A wrapper class around std::ostream it is mainly used to 
	///			   log errors and warnings redirectibly from flib
	///
	/////////////////////////////////////////////////////////////
	class Log
	{
		std::ostream *m_os; ///< Pointer to the current target output stream
	public:
		typedef std::ostream *value_type;
		typedef Log &reference;
		typedef const Log &const_reference;
		enum {
			dimensions = 1u ///< Public value indicating the amount of value_type's in the class
		};

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Log();
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the Log from a std::ostream
		/// 
		/// Please note that it is the caller's response to keep @a os stream valid when calling
		/// the bit-shift (<<) operator on the log
		/// 
		/// @param os The stream what's address is taken
		/// 
		/////////////////////////////////////////////////////////////
		Log(std::ostream &os);
		
		/////////////////////////////////////////////////////////////
		/// @brief Construct the Log from a pointer to a std::ostream
		/// 
		/// Please note that it is the caller's response to keep @a os stream valid when calling
		/// the bit-shift (<<) operator on the log
		/// 
		/// @param os The stream what's address is taken, if NULL then no logging will be done
		/// 
		/////////////////////////////////////////////////////////////
		Log(std::ostream *os);
		
		/////////////////////////////////////////////////////////////
		/// @brief Reassign the Log to a std::ostream
		/// 
		/// Please note that it is the caller's response to keep @a os stream valid when calling
		/// the bit-shift (<<) operator on the log
		/// 
		/// @param os The stream what's address is taken
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator=(std::ostream &os);
		
		/////////////////////////////////////////////////////////////
		/// @brief Reassign the Log to a pointer to a std::ostream
		/// 
		/// Please note that it is the caller's response to keep @a os stream valid when calling
		/// the bit-shift (<<) operator on the log
		/// 
		/// @param os The stream what's address is taken, if NULL then no logging will be done
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator=(std::ostream *os);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to keep the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param value The value to output to the stream
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		template<class T>
		reference operator<<(const T &value);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to keep the passed stream 
		/// valid when calling the this operator<br\>
		/// This operator is used when std::endl or other similar std function is passed
		/// 
		/// @param func The function to apply to the stream
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(std::ostream &(*func)(std::ostream &));
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to keep the passed stream 
		/// valid when calling the this operator<br\>
		/// This operator is used when std::endl or other similar std function is passed
		/// 
		/// @param func The function to apply to the stream
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(std::basic_ios<char, std::char_traits<char> > &(*func)(std::basic_ios<char, std::char_traits<char> > &));
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to keep the passed stream 
		/// valid when calling the this operator<br\>
		/// This operator is used when std::endl or other similar std function is passed
		/// 
		/// @param func The function to apply to the stream
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(std::ios_base &(*func)(std::ios_base &));
	};
}


#endif // FRONTIER_LOG_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Log.inl>
#endif
