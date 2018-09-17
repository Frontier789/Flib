#ifndef HUMANPLAYER_INCLUDED
#define HUMANPLAYER_INCLUDED

#include "Player.hpp"

class HumanPlayer : public Player
{
public:
	HumanPlayer(Game &game,int id);
};

#endif // HUMANPLAYER_INCLUDED