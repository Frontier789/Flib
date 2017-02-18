#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
	Window win(vec2(640,480),"Font tester",Window::Default &~ Window::Resize &~ Window::Maximize);
	
	bool running = true;
	Clock fpsClock;
	
	FixedShaderManager shader;
	shader.getCamera().set2D(win.getSize());
	
	Font font;
	Result res = font.loadFromFile("arial.ttf");
	if (!res)
	{
		cout << res.toString() << endl;
		return 1;
	}
	font.setSmooth(true);
	font.setCharacterSize(25);
	
	String message = u8"Dzsulah nem szereti a BornandC++t";
	
	vector<Sprite> sprites;
	float w = 0;
	
	for (auto c : message)
	{
		Glyph g = font.getGlyph(c);
		sprites.push_back(Sprite(g));
		sprites.back().setPos(vec2(w,0) + g.leftdown);
		w += sprites.back().getSize().w;
	}
	
	vector<vec2> basePts;
	
	for (auto &s : sprites)
	{
		basePts.push_back(win.getSize()/2 - vec2(w/2,0) + s.getPos());
		s.setPos(basePts.back());
	}
	
	Clock waveClk;
	auto getWaveH = [&](fm::Size index,fm::Size count) -> vec2 {
		
		float s = waveClk.getSeconds();
		
		return vec2(0,sin(deg(index*15 + s*80)) * 25 / (sin(deg(index*2 + s*80))*0.3 + 0.7));
		
	};
	
	for (;running;)
	{
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				running = false;
			}
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Plus)
				{
					sprites.clear();
					w = 0;
					
					font.setCharacterSize(font.getCharacterSize()+1);
					
					for (auto c : message)
					{
						Glyph g = font.getGlyph(c);
						sprites.push_back(Sprite(g));
						sprites.back().setPos(vec2(w,0) + g.leftdown);
						w += sprites.back().getSize().w;
					}
					
					basePts.clear();
					
					for (auto &s : sprites)
					{
						basePts.push_back(win.getSize()/2 - vec2(w/2,0) + s.getPos());
						s.setPos(basePts.back());
					}
				}
				
				if (ev.key.code == Keyboard::Minus)
				{
					sprites.clear();
					w = 0;
					
					font.setCharacterSize(font.getCharacterSize()-1);
					
					for (auto c : message)
					{
						Glyph g = font.getGlyph(c);
						sprites.push_back(Sprite(g));
						sprites.back().setPos(vec2(w,0) + g.leftdown);
						w += sprites.back().getSize().w;
					}
					
					basePts.clear();
					
					for (auto &s : sprites)
					{
						basePts.push_back(win.getSize()/2 - vec2(w/2,0) + s.getPos());
						s.setPos(basePts.back());
					}
				}
			}
		}
		
		C(sprites.size())
		{
			sprites[i].onUpdate();
			sprites[i].setPos(basePts[i] + getWaveH(i,sprites.size()));
		}
		
		win.clear();
		for (auto &s : sprites) s.onDraw(shader);
		win.swapBuffers();
		
		Sleep(1.0 / 60.0 - fpsClock.getSeconds());
		fpsClock.restart();
	}
}

