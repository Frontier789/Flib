#ifndef FRONTIER_FWLOG_HPP_INCLUDED
#define FRONTIER_FWLOG_HPP_INCLUDED
#include <FRONTIER/System/Log.hpp>
#define FRONTIER_FWLOG
namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief Log of the Window module
	/// 
	/// @ingroup Window
	/// 
	/// Every time an error occures in
	/// a class that is part of the Window module
	/// the bitshift (<<) operator will be called on 
	/// this instance
	/// 
	/////////////////////////////////////////////////////////////
    extern fm::Log fw_log;
}
#endif // FRONTIER_FWLOG_HPP_INCLUDED
