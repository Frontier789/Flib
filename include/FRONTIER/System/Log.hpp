#ifndef FRONTIER_LOG_HPP_INCLUDED
#define FRONTIER_LOG_HPP_INCLUDED
#define FRONTIER_LOG
#include <string>
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
		bool m_canRecallLog;   ///< If true, the last log can be accessed through getLastLog
		bool m_newLog;         ///< Iternal variable
		std::string m_lastLog; ///< Contains the last sent log
		std::string m_name;    ///< The name of the log (can be changed)
		bool m_promptName;     ///< If true then at every time logging @a m_name will be prompted (initially false)
		std::ostream *m_os;    ///< Pointer to the current target output stream
	public:
		typedef Log &reference;
		typedef const Log &const_reference;

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
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if anything is logged
		/// 
		/// @return True if has log
		/// 
		/////////////////////////////////////////////////////////////
		bool hasLog() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get last logged information
		/// 
		/// This function resets the state thus
		/// Immediately after this hasLog() would return false
		/// 
		/// @return The log (empty string if has no log)
		/// 
		/////////////////////////////////////////////////////////////
		std::string getLastLog();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the name of the log
		/// 
		/// The initial name is an empty string
		/// 
		/// @return The name
		/// 
		/////////////////////////////////////////////////////////////
		const std::string &getName() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the name of the log
		/// 
		/// If promptName is set to true
		/// then before every log this name is prompted
		/// 
		/// @param name The new name
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference setName(const std::string &name);
		
		/////////////////////////////////////////////////////////////
		/// @brief Find out if the object does log its name
		/// 
		/// Initially set to false
		/// 
		/// @return True if it does
		/// 
		/////////////////////////////////////////////////////////////
		bool doesPromptName() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Change the name prompting behavior
		/// 
		/// @param prompt Set true to have the log promt its name when logging
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference promptName(bool prompt=true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Change logging behavior
		/// 
		/// If disabled then hasLog will always return false
		/// and getLastLog will return empty string
		/// Default is true
		/// 
		/// @param enable Enables or disables log-recall
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference enableLogRecall(bool enable=true);
	};
}


#endif // FRONTIER_LOG_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Log.inl>
#endif
