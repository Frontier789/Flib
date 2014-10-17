#include <FRONTIER/System/log.hpp>

/////////////////////////////////////////////////////////////
namespace fw
{
	void WapiPrintLastErrorFunc(fm::Log &output,const char *functionName,const char *file, unsigned int line);
}

#define WapiPrintLastError(log,name) WapiPrintLastErrorFunc(log,#name,__FILE__,__LINE__)