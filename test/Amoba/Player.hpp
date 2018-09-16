#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <Frontier.hpp>
#include <iostream>
#include "BoardDisp.hpp"

using namespace std;

class Player : public Widget
{
	int m_id;
	int m_size;
	bool m_ready;
	vec2i m_move;
	const BoardDisp &m_board;
	vector<vector<int>> m_map; 

protected:
	void setmove(vec2i p);
	bool canmove(vec2i p) const;
	int mapsize() const;

public:
	Player(int N,int id);

	virtual void update();
	int getid() const;
	bool ready() const;
	vec2i getmove() const;
	virtual void enemymove(vec2i p,int eid);
	int getcell(vec2i p) const;
};

class HumanPlayer : public Player, public GuiElement
{
	
public:
	HumanPlayer(const BoardDisp &board,int id,GuiContext &cont);

	bool onEvent(fw::Event &ev) override;
};

class NNPlayer : public Player
{
	NeuralNet<225,2,300,225> m_nn;
	string m_file;
public:
	NNPlayer(int N,int id,string nn);
	virtual ~NNPlayer();

	void enemymove(vec2i p,int eid) override;
};

#endif // PLAYER_INCLUDED