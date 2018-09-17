#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(Game &game,int id) : Player(game, id) {
	onbuttonpress = [&](vec2 p,Mouse::Button b) {
		if (myturn() && b == Mouse::Left) {
			vec2i k = getgame().cellFromPix(p);
			cout << getid() << ": " << k << endl;
			setmove(k);
		}
	};
}
