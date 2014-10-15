#include <ctime>

fm::Time getCurrentTime()
{
	return fm::seconds(((double)std::clock()) / ((double)CLOCKS_PER_SEC));
}