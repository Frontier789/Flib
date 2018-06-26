#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <Frontier.hpp>
#include <iostream>

using namespace std;

class Player
{
	int m_id;
	int m_N;
	bool m_ready;
	vec2i m_move;
	vector<vector<int>> m_map; 

protected:
	void setmove(vec2i p);
	bool canmove(vec2i p) const;
	int mapsize() const;

public:
	Player(int N,int id);

	virtual void update();
	int getid() const;
	bool ready() const;
	vec2i getmove() const;
	void enemymove(vec2i p,int eid);
	int getcell(vec2i p) const;
};

#endif // PLAYER_INCLUDED