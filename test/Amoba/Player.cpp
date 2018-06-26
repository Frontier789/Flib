#include "Player.hpp"

	bool m_ready;
	vec2i m_move;
	vector<vector<int>> m_map; 

void Player::setmove(vec2i p)
{
	if (canmove(p) && !m_ready) {
		m_move = p;
		m_ready = true;
		m_map[p.x][p.y] = getid();
	}
}

bool Player::canmove(vec2i p) const
{
	return p.x >= 0 && p.y >= 0 && 
		   p.x < mapsize() && p.y < mapsize() && 
		   m_map[p.x][p.y]==0;
}

int Player::mapsize() const
{
	return m_N;
}

Player::Player(int N,int id) : 
	m_id(id),
	m_N(N),
	m_ready(false),
	m_map(vector<vector<int>>(N,vector<int>(N,0)))
{

}

bool Player::ready() const
{
	return m_ready;
}

vec2i Player::getmove() const
{
	return m_move;
}

int Player::getid() const
{
	return m_id;
}

void Player::enemymove(vec2i p,int eid)
{
	m_map[p.x][p.y] = eid;
	m_ready = false;
}

void Player::update()
{

}

int Player::getcell(vec2i p) const
{
	return m_map[p.x][p.y];
}
