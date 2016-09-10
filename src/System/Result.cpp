#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Error.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	Result::Result(const std::string &id,
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
	Result::Result(const Error &error)
	{
		(*this) = error;
	}
	
	/////////////////////////////////////////////////////////////
	Result::operator bool() const
	{
		return id.size() == 0;
	}
	
	/////////////////////////////////////////////////////////////
	bool Result::operator!() const
	{
		return id.size() != 0;
	}
	
	/////////////////////////////////////////////////////////////
	Result &Result::operator=(const Error &error)
	{
		id     = error.id;
		detail = error.detail;
		desc   = error.desc;
		file   = error.file;
		func   = error.func;
		line   = error.line;
		
		return *this;
	}
		
	/////////////////////////////////////////////////////////////
	std::string Result::toString() const
	{
		if (id == "") return "";
		
		return "Error (" + id + "." + detail + ") hapenned in " + file + " on line " + fm::toString(line).str() + " whilst calling " + func + " details: " + desc;
	}
}

////////////////////////////////////////////////////////////
std::basic_ostream<char, std::char_traits<char> > &operator<<(std::basic_ostream<char, std::char_traits<char> > &out,const fm::Result &result)
{
	return out << result.toString();
}
