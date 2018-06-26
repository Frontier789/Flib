#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include <random>
std::mt19937_64 mt(42.69);
std::uniform_real_distribution<double> dst(0,1);
double random()
{
	return dst(mt);
}

class Match
{
public:
	vector<vec2i> moves;
	
	void readFromLine(string line);
};

class BoardDisp : public GuiElement, public ClickListener
{
	DrawData m_grid,m_O,m_X;
	vector<vec3i> m_game;
	struct Props {
		int N;
		vec2  mid;
		float siz;
		vec2 base;
		float gsz;
	} props;
	void makeProps();
public:
	BoardDisp(GuiContext &cont);
	
	void prepareDDs();
	
	void addStep(vec2i p);
	void remStep(bool all = false);
	
	void onDraw(fg::ShaderManager &shader) override;
	
	void onClick(fw::Mouse::Button,fm::vec2 p) override {
		vec2i t = (p - props.base) / props.gsz;
		if (rect2i(vec2i(),vec2i(props.N-1)).contains(t)) {
			addStep(t);
		}
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
	for (auto &a : m.moves) {
		board->addStep(a);
	}
	win.getMainLayout().addChildElement(board);
	
	win.runGuiLoop();
}

BoardDisp::BoardDisp(GuiContext &cont) : 
	GuiElement(cont) 
{
	setSize(cont.getSize());
	makeProps();
	prepareDDs();
}

void BoardDisp::makeProps()
{
	props.N    = 15;
	props.mid  = getSize()/2;
	props.siz  = getSize().min();
	props.base = props.mid - vec2(props.siz)/2;
	props.gsz  = props.siz/props.N;
}

void BoardDisp::prepareDDs()
{
	float xin = .08;
	vec4 gclr = vec4(.5);
	vec4 oclr = vec4(.8,.6,.5);
	vec4 xclr = vec4(.5,.6,.8);
	
	Mesh m;
	for (int i=0;i<=props.N;++i) {
		m.pts.push_back(props.base + vec2(props.gsz*i,0));
		m.pts.push_back(props.base + vec2(props.gsz*i,props.siz));
		m.pts.push_back(props.base + vec2(0,props.gsz*i));
		m.pts.push_back(props.base + vec2(props.siz,props.gsz*i));
	}
	
	m.clr.resize(m.pts.size(),gclr);
	
	m_grid = m;
	m_grid.clearDraws();
	m_grid.addDraw(fg::Lines);
	
	float xins = xin/sqrt(2)*props.gsz;
	Mesh mx;
	mx.pts.push_back(vec2(0,xins));
	mx.pts.push_back(vec2(xins,0));
	mx.pts.push_back(vec2(props.gsz,props.gsz-xins));
	mx.pts.push_back(vec2(0,xins));
	mx.pts.push_back(vec2(props.gsz,props.gsz-xins));
	mx.pts.push_back(vec2(props.gsz-xins,props.gsz));
	
	mx.pts.push_back(vec2(props.gsz,xins));
	mx.pts.push_back(vec2(props.gsz-xins,0));
	mx.pts.push_back(vec2(0,props.gsz-xins));
	mx.pts.push_back(vec2(props.gsz,xins));
	mx.pts.push_back(vec2(0,props.gsz-xins));
	mx.pts.push_back(vec2(xins,props.gsz));
	
	mx.clr.resize(mx.pts.size(),xclr);
	
	m_X = mx;
	
	
	
	float oins = xin*props.gsz;
	Mesh mo;
	
	vector<vec2> lineo;
	int ko = 30;
	for (int i=0;i<ko;++i) {
		vec2 p = vec2(props.gsz/2) + vec2(pol2(props.gsz/2-oins/2,deg(i/(ko-1.f)*360)));
		lineo.push_back(p);
	}
	
	mo = Mesh::tesLineStrip(&lineo[0],lineo.size(),oins);
	
	mo.clr.resize(mo.pts.size(),oclr);
	
	m_O = mo;
}
	
void BoardDisp::onDraw(fg::ShaderManager &shader)
{
	shader.draw(m_grid);
	
	shader.getModelStack().push();
	for (auto &s : m_game) {
		DrawData &dd = (s.z ? m_X : m_O);
		
		shader.getModelStack().top(MATRIX::translation(props.base + vec2(s)*props.gsz));
		shader.draw(dd);
	}
	shader.getModelStack().pop();
}
	
void BoardDisp::addStep(vec2i p)
{
	int x = m_game.empty() ? 1 : 1-m_game.back().z;
	m_game.push_back(vec3i(p,x));
}

void BoardDisp::remStep(bool all)
{
	if (all) m_game.clear();
	else if (!m_game.empty()) {
		m_game.pop_back();
	}
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
