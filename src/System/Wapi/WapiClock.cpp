#include <windows.h>

class TicksPerSecondHolder
{
public:
	LARGE_INTEGER ticksPerSecond;
	TicksPerSecondHolder()
	{
		QueryPerformanceFrequency(&ticksPerSecond);
	}
};

const TicksPerSecondHolder tpsh;

fm::Time getCurrentTime()
{
	LARGE_INTEGER actTime;
	QueryPerformanceCounter(&actTime);
	return fm::seconds(((double)actTime.QuadPart) / ((double)tpsh.ticksPerSecond.QuadPart));
}