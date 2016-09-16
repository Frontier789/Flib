#include "fnGetLastError.hpp"
#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/macros/OS.h>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>


#ifdef FRONTIER_OS_WINDOWS
#include <windows.h>
#include <string>

namespace fn
{
	fm::Error getLastError(const char *netFunc,const char *file,int line)
	{
		std::string ret;
		char *errorText; // FormatMessage allocates memory automatically for us
		DWORD err = GetLastError();
		
		DWORD errors[] = {           WSAENETDOWN,               WSAEAFNOSUPPORT,         WSAEMFILE,        WSAENOBUFS,      WSAEPROTONOSUPPORT,                                 WSAEINVAL,                        WSAESOCKTNOSUPPORT,           WSAEADDRINUSE,             WSAEALREADY,     WSAECONNREFUSED,                   WSAENETUNREACH,               WSAEHOSTUNREACH,                        WSAECONNRESET,            WSAEOPNOTSUPP,           WSAENOTCONN};                       																
		std::string descs[] = {"Network is down","Address family not supported","Out of resources","Out of resources","Protocol not supported","Invalid value specified in internal code","Invalid value specified in internal code","Address already in use","Address already in use","Connection refused","Destination network unreachable","Destination host unreachable","Connection was reset by remote host","Operation not supported","Socket not connected"};
		std::string types[] = {        "NetDown",                "NotSupported",        "OutOfRes",        "OutOfRes",          "NotSupported",                           "InternalError",                           "InternalError",          "AlreadyInUse",          "AlreadyInUse", "ConnectionRefused",                "DestUnreachable",             "DestUnreachable",                           "ConReset",           "NotSupported",        "NotConnected"};
		
		C(sizeof(errors)/sizeof(*errors))
			if (err == errors[i])
			{
				return fm::Error("NetError",types[i],descs[i] + " error code (" + fm::toString(err).str() + ")",netFunc,file,line);
			}
		
		if (err == WSAEISCONN) return fm::Result();
		
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   (LPTSTR)&errorText,0,NULL);
		if (errorText)
		{
			// convert description to string
			ret = std::string(errorText);
			::LocalFree(errorText); // free up the allocated memory
		}

		while (ret.length() && (ret[ret.length()-1]=='\t' ||
								ret[ret.length()-1]==' '  ||
								ret[ret.length()-1]=='\r' ||
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);

		return fm::Error("NetError",fm::toString(err),ret,netFunc,file,line);
	}
}

#endif // winOS

#ifdef FRONTIER_OS_LINUX
#include <string.h>
#include <errno.h>

namespace fn
{
	fm::Error getLastError(const char *netFunc,const char *file,int line)
	{
		int err = errno;
		
		int errors[] = {                          EACCES,                  EAFNOSUPPORT,         EPROTONOSUPPORT,                                    EINVAL,            EMFILE,            ENFILE,           EADDRNOTAVAIL,                EALREADY,        ECONNREFUSED,                      ENETUNREACH,                  EHOSTUNREACH,                           ECONNRESET,               EOPNOTSUPP,                         ECONNABORTED,              ENOTCONN};
		std::string descs[] = {"Operation not permitted","Address family not supported","Protocol not supported","Invalid value specified in internal code","Out of resources","Out of resources","Address already in use","Address already in use","Connection refused","Destination network unreachable","Destination host unreachable","Connection was reset by remote host","Operation not supported","Connection was reset by remote host","Socket not connected"};
		std::string types[] = {              "Forbidden",                "NotSupported",          "NotSupported",                           "InternalError",        "OutOfRes",        "OutOfRes",          "AlreadyInUse",          "AlreadyInUse", "ConnectionRefused",                "DestUnreachable",             "DestUnreachable",                           "ConReset",           "NotSupported",                           "ConReset",        "NotConnected"};
		
		char *errDesc = strerror(err);
		
		C(sizeof(errors)/sizeof(*errors))
			if (err == errors[i])
			{
				return fm::Error("NetError",types[i],descs[i] + ", error code (" + fm::toString(err).str() + ", strerror = " + errDesc + " )",netFunc,file,line);
			}
			
		return fm::Error("NetError",fm::toString(err),"error code (" + fm::toString(err).str() + ", strerror = " + errDesc + " )",netFunc,file,line);
	}
}

#endif // Linux
