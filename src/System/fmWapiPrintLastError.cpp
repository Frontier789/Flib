#include "Wapi/fmWapiPrintLastError.hpp"
#include <windows.h>
#include <string>

namespace fm
{
	void WapiPrintLastErrorFunc(Log &output,const char *functionName,const char *file, unsigned int line)
	{
		std::string ret;
		char *errorText; // FormatMessage allocates memory automatically for us
		DWORD err=GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   (LPTSTR)&errorText,0,NULL);
		if (errorText)
		{
			// if we got any description then we convert it to string
			ret=std::string(errorText);
			::LocalFree(errorText); //we free up the allocated memory
		}
		
		while (ret.length() && (ret[ret.length()-1]=='\t' || 
								ret[ret.length()-1]==' '  || 
								ret[ret.length()-1]=='\r' || 
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);
		
		
		output << functionName << " failed in " << file << " : " << line << " (LastError=\"" << ret << "\")=" << err << std::endl;
	}	
}
