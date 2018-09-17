#include "Player.hpp"

void Player::setmove(vec2i p)
{
	if (canmove(p) && !m_ready) {
		m_move = p;
		m_ready = true;
	}
}

bool Player::canmove(vec2i p) const
{
	return getgame().at(p) == 0;
}

Player::Player(Game &game,int id) : Widget(game.window(), game.window().getSize()),
	m_id(id),
	m_size(game.size()),
	m_ready(true),
	m_game(game)
{
	onresize = [&](vec2i size) {
		setSize(size);	
	};
}

void Player::beginturn()
{
	m_ready = false;
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

void Player::enemymove(vec2i,int)
{
	
}

void Player::update()
{

}
