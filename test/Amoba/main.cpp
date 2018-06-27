#include <Frontier.hpp>
#include <iostream>
#include "BoardDisp.hpp"
#include "Player.hpp"
#include "Game.hpp"

using namespace std;

#include <fstream>
int main()
{
	GuiWindow win(vec2(640,480));
	win.setClearColor(vec4(.3,1));
	
	auto board = new BoardDisp(win);
	auto p1 = new HumanPlayer(*board,1,win);
	auto p2 = new NNPlayer(board->getProps().N,2,"ply.nn");
	auto game = new Game(p1,p2,board,win);

	win.getMainLayout().addChildElement(board);
	win.getMainLayout().addChildElement(p1);
	win.getMainLayout().addChildElement(game);
	
	win.runGuiLoop();

	delete p2;
}
