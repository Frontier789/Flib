#include <FRONTIER/Window/Wapi/WapiCheck.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <FRONTIER/Config.hpp>
#include <string>

namespace fw
{
	namespace priv
	{
		/// glCheckError /////////////////////////////////////////////////////////
		void WapiCheckError(const char *file, unsigned int line,long error)
		{
			if (!error)
				return;
			std::string errorStr;
			char *errorText; //::FormatMessage allocates memory automatically for us
			DWORD err=GetLastError();
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS,
						   NULL,
						   err,
						   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						   (LPTSTR)&errorText,0,NULL);
			if (errorText)
			{
				// if we got any description then we convert it to string
				errorStr=std::string(errorText);
				::LocalFree(errorText); //we free up the allocated memory
			}
			
			while (errorStr.length() && (errorStr[errorStr.length()-1]=='\t' || 
										 errorStr[errorStr.length()-1]==' '  || 
										 errorStr[errorStr.length()-1]=='\r' || 
										 errorStr[errorStr.length()-1]=='\n') ) errorStr.resize(errorStr.length()-1);
			
			fw::fw_log << "A iternal windows api call in " << file << " (line: "<<line<<") failed.\n"
					   << "lestError=\"" << errorStr << std::endl;
			
		}
	}
}
