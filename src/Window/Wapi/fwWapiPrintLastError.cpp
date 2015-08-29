////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/Window/Wapi/fwWapiPrintLastError.hpp>
#include <windows.h>
#include <string>

namespace fw
{
	void WapiPrintLastErrorFunc(fm::Log &output,const char *functionName,const char *file, unsigned int line)
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
			// convert description to string
			ret=std::string(errorText);
			::LocalFree(errorText); // free up the allocated memory
		}
		
		while (ret.length() && (ret[ret.length()-1]=='\t' || 
								ret[ret.length()-1]==' '  || 
								ret[ret.length()-1]=='\r' || 
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);
		
		
		output << functionName << " failed in " << file << " : " << line << " (LastError=\"" << ret << "\")=" << err << std::endl;
	}	
}
