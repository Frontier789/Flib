#ifndef FRONTIER_FGLOG_HPP_INCLUDED
#define FRONTIER_FGLOG_HPP_INCLUDED
#include <FRONTIER/System/Log.hpp>
#define FRONTIER_FGLOG
namespace fg
{
	//////////////////////////////////
	/// @brief Log of the Graphics module
	/// 
	/// Every time an error occures in
	/// a class that is part of the graphics module
	/// the bitshift (<<) operator will be called on 
	/// this instance
	/// 
	//////////////////////////////////
    extern fm::Log fg_log;
}
#endif // FRONTIER_FGLOG_HPP_INCLUDED
