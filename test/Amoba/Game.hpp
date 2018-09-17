#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <Frontier.hpp>
#include <iostream>
#include <vector>
#include "BoardDisp.hpp"

class Player;

using namespace std;

class Game
{
	GuiWindow m_win;
	vector<Player*> m_players;
	BoardDisp *m_board;
	int m_winlen;
	int m_winner;
	bool m_playing;
	int m_turn;
	int m_size;
	vector<vector<int>> m_map;
	vector<vec3i> m_moves;

	bool checkwin() const;
	void handlePlayers();
public:
	Game(int size,int winlen = 5);
	void addPlayer(Player *player);

	~Game();
	
	int at(vec2i p) const;
	int size() const {return m_size;}
	const vector<vector<int>> &map() const {return m_map;}
	GuiWindow &window() {return m_win;}
	BoardDisp &board() {return *m_board;}
	const vector<vec3i> &moves() const {return m_moves;}
	int winner() const {return m_winner;}
	
	void run();
	vec2i cellFromPix(vec2 p) const;
};

#endif // GAME_INCLUDED