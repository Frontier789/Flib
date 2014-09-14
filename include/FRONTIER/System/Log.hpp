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
	
	template<typename>
    class allocator;
	template<typename,typename,typename> 
	class basic_string;
	
	typedef basic_string<char,std::char_traits<char>,std::allocator<char> > string;
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
		bool m_canRecallLog;        ///< If true, the last log can be accessed through getLastLog
		bool m_newLog;              ///< Iternal variable
		std::string *m_lastLog;     ///< Contains the last sent log
		std::string *m_name;        ///< The name of the log (can be changed)
		std::string *m_tmp;	        ///< Iternally used temrory object
		bool m_promptName;          ///< If true then at every time logging @a m_name will be prompted (initially false)
		std::ostream *m_os;         ///< Pointer to the current target output stream
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
		/// @brief Default destructor
		/// 
		/////////////////////////////////////////////////////////////
		~Log();
		
		/////////////////////////////////////////////////////////////
		/// @brief Associate the Log with a std::ostream
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
		/// @brief Associate the Log with a std::ostream
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
		/// @brief Get the associated stream
		/// 
		/// May return NULL
		/// 
		/// @return Pointer to the stream
		/// 
		/////////////////////////////////////////////////////////////
		std::ostream *getStream();
		
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param text The output string
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const char *text);
		
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param text The output string
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const std::string &text);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const long &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const unsigned long &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const int &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const unsigned int &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param character The output character
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const char &character);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const unsigned char &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const float &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param number The output number
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const double &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief Templated overload of the << binary operator
		/// 
		/// Please note that it is the caller's response to have the passed stream 
		/// valid when calling the this operator
		/// 
		/// @param address The output address
		/// 
		/// @return Reference to itself
		/// 
		/////////////////////////////////////////////////////////////
		reference operator<<(const void *address);
		
		
		
		
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
		/// @brief Find out if the log has associated stream
		/// 
		/// Even if the associated stream is invalid (destructed)
		/// this function returns true
		/// 
		/// @return True if has associated stream
		/// 
		/////////////////////////////////////////////////////////////
		bool hasStream() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get last logged information
		/// 
		/// This function resets the state thus
		/// Immediately after this hasLog() would return false
		/// 
		/// @return The log (empty string if has no log)
		/// 
		/////////////////////////////////////////////////////////////
		const std::string &getLastLog();
		
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
