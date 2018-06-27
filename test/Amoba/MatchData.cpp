#include "MatchData.hpp"
#include <sstream>

void MatchData::readFromLine(string line)
{
	stringstream ss(std::move(line));
	moves.clear();
	string s;
	
	while (ss >> s) {
		int x = s[0]-'a';
		int y = 15-atoi(s.c_str()+1);
		moves.push_back(vec2i(x,y));
	}
}
