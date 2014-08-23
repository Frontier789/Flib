#ifndef FRONTIER_SLEEP_HPP_INCLUDED
#define FRONTIER_SLEEP_HPP_INCLUDED
#define FRONTIER_SLEEP
namespace fm
{
	class Time;
	
	/////////////////////////////////////////////////////////////
	/// @brief Function used to pause the execution for @a duration time
	/// 
	/// @param duration Time amount to pause for
	/// 
	/////////////////////////////////////////////////////////////
	void Sleep(const Time &duration);
}
#endif // FRONTIER_SLEEP_HPP_INCLUDED
