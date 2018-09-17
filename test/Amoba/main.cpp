#include <Frontier.hpp>
#include "HumanPlayer.hpp"
#include "BoardDisp.hpp"
#include "Player.hpp"
#include "Game.hpp"

using namespace std;

int main()
{
	Game g(3,3);
	
	g.addPlayer(new HumanPlayer(g,1));
	g.addPlayer(new HumanPlayer(g,2));
	g.run();
	
	if (!g.winner()) cout << "its a tie" << endl;
	else
		cout << "plyer id " << g.winner() << " won" << endl;
	
	fm::Sleep(2);
}
