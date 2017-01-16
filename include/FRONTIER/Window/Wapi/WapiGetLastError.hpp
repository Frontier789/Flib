#include <FRONTIER/System/Result.hpp>

/////////////////////////////////////////////////////////////
namespace fw
{
	fm::Result WapiGetLastErrorFunc(const char *functionName,const char *file, unsigned int line);
}

#define WapiGetLastError(name) WapiGetLastErrorFunc(name,__FILE__,__LINE__)