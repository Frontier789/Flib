#ifndef MATCH_DATA_INCLUDED
#define MATCH_DATA_INCLUDED

#include <Frontier.hpp>
#include <iostream>

using namespace std;

class MatchData
{
public:
	vector<vec2i> moves;
	
	void readFromLine(string line);
};

#endif // MATCH_DATA_INCLUDED