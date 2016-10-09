#ifndef FRONTIER_FN_GET_LAST_ERROR_HPP_INCLUDED
#define FRONTIER_FN_GET_LAST_ERROR_HPP_INCLUDED

#include <FRONTIER/System/Error.hpp>

namespace fn
{
	fm::Error getLastError(const char *netFunc,const char *file,int line);
}

#define FRONTIER_FN_GETLASTERROR(netFunc) ::fn::getLastError(netFunc,__FILE__,__LINE__)

#endif // FRONTIER_FN_GET_LAST_ERROR_HPP_INCLUDED
