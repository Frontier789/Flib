#include "fmPosixPrintErrno.hpp"
#include <errno.h>
#include <string>

namespace fm
{
	void PosixPrintErrnoFunc(Log &output,const char *functionName,const char *file, unsigned int line)
	{
		// only call errno once because it might
		// expand to something like ERRNO()
		int errorNum = errno;
		char *errorStr = strerror(errorNum);
		std::string str = errorStr ? std::string(errorStr) : std::string("unknown error");
		
		output << functionName << " failed in " << file << " : " << line << " (errno=\"" << str << "\")=" << errorNum << std::endl;
	}
}
