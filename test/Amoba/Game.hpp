#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <Frontier.hpp>
#include <iostream>
#include <vector>
#include "BoardDisp.hpp"
#include "Player.hpp"

using namespace std;

class Game
{
	vector<Player*> m_players;
	BoardDisp *m_board;
	bool m_playing;
	int m_winlen;
	int m_size;
	int m_turn;
	vector<vector<int>> m_map;
	GuiWindow m_win;

	bool checkwin() const;
	void handlePlayers();
public:
	Game(vector<Player*> players,int size,int winlen = 5);

	~Game();
	
};

#endif // GAME_INCLUDED