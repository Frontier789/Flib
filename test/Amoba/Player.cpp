#include "Player.hpp"

void Player::setmove(vec2i p)
{
	if (canmove(p) && !m_ready) {
		m_move = p;
		m_ready = true;
		m_map[p.x][p.y] = getid();
	}
}

bool Player::canmove(vec2i p) const
{
	return p.x >= 0 && p.y >= 0 && 
		   p.x < mapsize() && p.y < mapsize() && 
		   m_map[p.x][p.y]==0;
}

int Player::mapsize() const
{
	return m_N;
}

Player::Player(int N,int id) : 
	m_id(id),
	m_N(N),
	m_ready(false),
	m_map(vector<vector<int>>(N,vector<int>(N,0)))
{

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

void Player::enemymove(vec2i p,int eid)
{
	m_map[p.x][p.y] = eid;
	m_ready = false;
}

void Player::update()
{

}

int Player::getcell(vec2i p) const
{
	return m_map[p.x][p.y];
}



// ------------------------------------- //


HumanPlayer::HumanPlayer(const BoardDisp &board,int id,GuiContext &cont) : 
	Player(board.getProps().N,id),
	GuiElement(cont),
	m_board(board)
{
	setSize(cont.getSize());
}

bool HumanPlayer::onEvent(fw::Event &ev) {
	if (ev.type == Event::ButtonPressed) {
		if (ev.mouse.button == Mouse::Left) {
			vec2i m = m_board.cellFromPix(vec2i(ev.mouse));
			setmove(m);
		}
	}
	return false;
}


#include "MatchData.hpp"
#include <fstream>

NNPlayer::NNPlayer(int N,int id,string nn) : 
	Player(N,id),
	m_file(nn)
{
	m_nn.loadFromFile(nn);

	
	MatchData m;
	ifstream in("gomoku.db");
	
	for (int n=0;n<10;++n) {
		string line;
		getline(in,line);
		
		m.readFromLine(line);

		vector<matrix<1,225>> boards(m.moves.size(),0);
		for (Size i=1;i<m.moves.size();++i) {
			boards[i] = boards[i-1];
			vec2i p = m.moves[i-1];
			boards[i][0][p.x*N+p.y] = (i%2 ? 1 : 2);
		}
		
		std::mt19937_64 m_mt(1245);
		std::uniform_real_distribution<double> m_dst(0,1);
		

		for (int k=0;k<20;++k) {
			vector<int> q(m.moves.size());
			for (Size i=0;i<q.size();++i) q[i] = i;

			for (Size i=0;i<q.size();++i) {
				float a = m_dst(m_mt);
				float b = m_dst(m_mt);
				swap(q[q.size()*a],q[q.size()*b]);
			}

			for (Size i=0;i<q.size();++i) {
				matrix<1,225> exout(0);
				vec2i p = m.moves[q[i]];
				exout[0][p.x*N+p.y] = 1;

				m_nn.train(boards[q[i]],exout,.1/(1+sqrt(k)));
			}
		}
	}
}

NNPlayer::~NNPlayer()
{
	m_nn.saveToFile(m_file);
}

void NNPlayer::enemymove(vec2i p,int eid)
{
	Player::enemymove(p,eid);

	int N = mapsize();

	matrix<1,225> arr;
	for (int x=0;x<N;++x)
		for (int y=0;y<N;++y)
			arr[0][x*N + y] = getcell(vec2i(x,y));
	
	matrix<1,225> out = m_nn(arr);
	
	float mx = 0;
	vec2i mv;
	bool found = false;

	for (int x=0;x<N;++x) {
		for (int y=0;y<N;++y) {
			cout << out[0][x*N+y] << " ";
			if (getcell(vec2i(x,y)) == 0) {
				if (out[0][x*N + y] > mx || !found) {
					mx = out[0][x*N + y];
					mv = vec2i(x,y);
					found = true;
				}
			}
		}
		cout << endl;
	}
	cout << endl;
	
	setmove(mv);
}

