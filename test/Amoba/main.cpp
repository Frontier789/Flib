#include <Frontier.hpp>
#include <iostream>
#include "BoardDisp.hpp"
#include "Player.hpp"
#include "Game.hpp"

using namespace std;

class Match
{
public:
	vector<vec2i> moves;
	
	void readFromLine(string line);
};

class HumanPlayer : public Player, public GuiElement
{
	const BoardDisp &m_board;
public:
	HumanPlayer(const BoardDisp &board,int id,GuiContext &cont) : 
		Player(board.getProps().N,id),
		GuiElement(cont),
		m_board(board)
	{
		setSize(cont.getSize());
	}

	bool onEvent(fw::Event &ev) override {
		if (ev.type == Event::ButtonPressed) {
			if (ev.mouse.button == Mouse::Left) {
				vec2i m = m_board.cellFromPix(vec2i(ev.mouse));
				setmove(m);
			}
		}
		return false;
	}
};

#include <fstream>
int main()
{
	Match m;
	ifstream in("gomoku.db");
	
	string line;
	getline(in,line);
	
	m.readFromLine(line);
	
	GuiWindow win(vec2(640,480));
	win.setClearColor(vec4(.3,1));
	
	auto board = new BoardDisp(win);
	auto p1 = new HumanPlayer(*board,1,win);
	auto p2 = new HumanPlayer(*board,2,win);
	auto game = new Game(p1,p2,board,win);

	win.getMainLayout().addChildElement(board);
	win.getMainLayout().addChildElement(p1);
	win.getMainLayout().addChildElement(p2);
	win.getMainLayout().addChildElement(game);
	
	win.runGuiLoop();
}

#include <sstream>
void Match::readFromLine(string line)
{
	stringstream ss(std::move(line));
	moves.clear();
	string s;
	
	while (ss >> s) {
		int x = s[0]-'a';
		int y = 15-atoi(s.c_str()+1);
		moves.push_back(vec2i(x,y));
	}
}
