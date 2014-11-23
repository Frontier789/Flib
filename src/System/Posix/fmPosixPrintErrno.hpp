#include <FRONTIER/System/log.hpp>

/////////////////////////////////////////////////////////////
namespace fm
{
	void PosixPrintErrnoFunc(Log &output,const char *functionName,const char *file, unsigned int line);
}

#define PosixPrintErrno(log,name) PosixPrintErrnoFunc(log,#name,__FILE__,__LINE__)