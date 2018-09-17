#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <Frontier.hpp>
#include <iostream>
#include "Game.hpp"

using namespace std;

class Player : public Widget
{
	int m_id;
	int m_size;
	bool m_ready;
	vec2i m_move;
	Game &m_game;

protected:
	void setmove(vec2i p);
	bool canmove(vec2i p) const;
	Game &getgame() {return m_game;}
	const Game &getgame() const {return m_game;}

public:
	Player(Game &game,int id);

	int getid() const;
	bool ready() const;
	vec2i getmove() const;
	void beginturn();
	bool myturn() const {return !m_ready;}
	
	virtual void enemymove(vec2i p,int eid);
	virtual void update();
};

#endif // PLAYER_INCLUDED