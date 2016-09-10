#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Error.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	Error::Error(const std::string &id,
		  		 const std::string &detail,
		  		 const std::string &desc,
		  		 const std::string &func,
		  		 const std::string &file,
		  		 fm::Size line) : id(id),
								  detail(detail),
								  desc(desc),
								  func(func),
								  file(file),
								  line(line)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Error::Error(const Result &result)
	{
		(*this) = result;
	}
	
	/////////////////////////////////////////////////////////////
	Error::operator bool() const
	{
		return id.size() != 0;
	}
	
	/////////////////////////////////////////////////////////////
	bool Error::operator!() const
	{
		return id.size() == 0;
	}
	
	/////////////////////////////////////////////////////////////
	Error &Error::operator=(const Result &result)
	{
		id     = result.id;
		detail = result.detail;
		desc   = result.desc;
		file   = result.file;
		func   = result.func;
		line   = result.line;
		
		return *this;
	}
		
	/////////////////////////////////////////////////////////////
	std::string Error::toString() const
	{
		if (id == "") return "";
		
		return "Error (" + id + "." + detail + ") hapenned in " + file + " on line " + fm::toString(line).str() + " whilst calling " + func + " details: " + desc;
	}
	
	/////////////////////////////////////////////////////////////
	Error &Error::operator+=(const Error &error)
	{
		if (id == "") (*this) = error;
		return *this;
	}
}

////////////////////////////////////////////////////////////
std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out,const fm::Error &err)
{
	return out << err.toString();
}
