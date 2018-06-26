#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <Frontier.hpp>
#include <iostream>
#include "BoardDisp.hpp"
#include "Player.hpp"

using namespace std;

class Game : public GuiElement
{
	Player *m_p1;
	Player *m_p2;
	BoardDisp *m_board;
	bool m_playing;
	int m_turn;
	int m_N;
	vector<vector<int>> m_map;

	bool checkwin() const;
public:
	Game(Player *p1,Player *p2,BoardDisp *board,GuiContext &cont);

	~Game();

	void onUpdate(const fm::Time &) override;
};

#endif // GAME_INCLUDED