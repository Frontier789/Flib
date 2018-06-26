#include "Game.hpp"

Game::Game(Player *p1,Player *p2,BoardDisp *board,GuiContext &cont) : 
	GuiElement(cont),
	m_p1(p1),
	m_p2(p2),
	m_board(board),
	m_playing(true),
	m_turn(0),
	m_N(board->getProps().N),
	m_map(vector<vector<int>>(m_N,vector<int>(m_N,0)))
{

}

Game::~Game()
{
	
}

void Game::onUpdate(const fm::Time &)
{
	if (!m_playing) return;

	bool ply1 = m_turn%2 == 0;
	Player *ply = (ply1 ? m_p1 : m_p2);
	Player *enemy = (ply1 ? m_p2 : m_p1);

	ply->update();

	if (ply->ready()) {
		vec2i m = ply->getmove();

		m_map[m.x][m.y] = ply->getid();

		if (checkwin()) {
			m_playing = false;
			cout << "player id " << ply->getid() << " won" << endl;
		}

		enemy->enemymove(m,ply->getid());
		m_board->addStep(m);
		++m_turn;
	}
}

bool Game::checkwin() const
{
	for (vec4i d : {vec4i(-4,0,1,0),vec4i(0,-4,0,1),vec4i(-4,-4,1,1),vec4i(-4,-4,-1,1)}) {
		for (int x=0;x<m_N + d.x;++x)
		for (int y=0;y<m_N + d.y;++y) {
			int a = m_map[x][y];
			if (a) {
				bool win = true;
				for (int i=1;i<5;++i) {
					int j = (d.z == -1 ? 5-i : i);
					win = win && m_map[x+j][y+i*d.w]==a;
				}
				if (win) return true;
			}
		}
	}
	return false;
}
