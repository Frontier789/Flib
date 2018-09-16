#include "Game.hpp"

Game::Game(vector<Player*> players,int size,int winlen) : 
	m_players(players),
	m_board(new BoardDisp(size)),
	m_winlen(winlen),
	m_playing(true),
	m_turn(0),
	m_size(size),
	m_map(vector<vector<int>>(m_size,vector<int>(m_size,0)))
{

}

Game::~Game()
{
	delete m_board;
}

void Game::handlePlayers()
{
	if (!m_playing) return;

	int plyId = m_turn%m_size;
	auto ply = m_players[plyId];
	
	ply->update();

	if (ply->ready()) {
		vec2i m = ply->getmove();

		m_map[m.x][m.y] = ply->getid();

		if (checkwin()) {
			m_playing = false;
			cout << "player id " << ply->getid() << " won" << endl;
		}

		for (int i=0;i<m_size;++i) {
			if (i != plyId) {
				auto enemy = m_players[i];
				enemy->enemymove(m,ply->getid());
			}
		}
		
		m_board->addStep(m, ply->getid());
		++m_turn;
	}
}

bool Game::checkwin() const
{
	for (int x=0;x<m_size;++x)
	for (int y=0;y<m_size;++y) {
		int a = m_map[x][y];
		if (a) {
			for (vec2i d : {vec2i(1,0), vec2i(0,1), vec2i(1,-1), vec2i(1,1)}) {
				bool win = true;
				for (int i=1;i<m_winlen;++i) {
					vec2i p = vec2i(x,y) + d*i;
					if (p.x >= 0 && p.y >= 0 && p.x < m_size && p.y < m_size) {
						int j = m_map[p.x][p.y];
						win = win && j==a;
					} else win = false;
				}
				if (win) return true;
			}
		}
	}
	
	return false;
}
