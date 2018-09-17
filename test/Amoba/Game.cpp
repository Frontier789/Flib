#include "Game.hpp"
#include "Player.hpp"

Game::Game(int size,int winlen) : 
	m_win(vec2(640,480)),
	m_board(new BoardDisp(m_win,size)),
	m_winlen(winlen),
	m_winner(0),
	m_playing(true),
	m_turn(0),
	m_size(size),
	m_map(vector<vector<int>>(m_size,vector<int>(m_size,0)))
{
	m_win.setClearColor(vec4(.3,1));
	
	auto &layout = m_win.getMainLayout();
	layout.addChildElement(m_board);
}

void Game::addPlayer(Player *player) {
	auto &layout = m_win.getMainLayout();
	layout.addChildElement(player);
	m_players.push_back(player);
}

Game::~Game()
{
	
}

void Game::handlePlayers()
{
	if (!m_playing) return;

	int plyId = m_turn%m_players.size();
	auto ply = m_players[plyId];
	
	ply->update();

	if (ply->ready()) {
		vec2i m = ply->getmove();
		m_moves.push_back(vec3i(m, ply->getid()));
		
		if (m_moves.size() == m_size*m_size)
			m_playing = false;

		m_map[m.x][m.y] = ply->getid();

		if (checkwin()) {
			m_playing = false;
			m_winner = ply->getid();
		}

		for (int i=0;i<m_players.size();++i) {
			if (i != plyId) {
				auto enemy = m_players[i];
				enemy->enemymove(m,ply->getid());
			}
		}
		
		m_board->addStep(m, ply->getid());
		++m_turn;
		
		m_players[m_turn%m_players.size()]->beginturn();
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
				for (int i=1;i<m_winlen;++i)
					win = win && at(vec2i(x,y) + d*i)==a;
				
				if (win) return true;
			}
		}
	}
	
	return false;
}
	
int Game::at(vec2i p) const
{
	if (p.x >= 0 && p.y >= 0 && p.x < m_size && p.y < m_size)
		return m_map[p.x][p.y];
	return 0;
}

vec2i Game::cellFromPix(vec2 p) const
{
	auto props = m_board->getProps();
	
	vec2 t = (p - props.base) / float(props.gsz);

	return vec2i(floor(t.x),floor(t.y));
}

void Game::run() {
	m_players[0]->beginturn();
	m_win.runGuiLoop([&](){
		handlePlayers();
		if (!m_playing)
			m_win.stopGuiLoop();
	});
}
