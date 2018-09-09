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
	for (int x=0;x<m_N;++x)
	for (int y=0;y<m_N;++y) {
		int a = m_map[x][y];
		if (a) {
			for (vec2i dir : {vec2i(1,0),vec2i(1,1),vec2i(0,1),vec2i(1,-1)}) {
				bool win = true;
				for (int i=1;i<5;++i) {
					vec2i p = vec2i(x,y) + dir * i;
					if (p.x < 0 || p.y < 0 || p.x > m_N-1 || p.y > m_N-1) {
						win = false;
					} else {
						win = win && a == m_map[p.x][p.y];
					}
				}
				if (win) return true;
			}
		}
	}
	return false;
}
