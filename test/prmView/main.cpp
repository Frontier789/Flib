#include <Frontier.hpp>

#include <iostream>
using namespace std;

int main()
{
	GuiWindow win(vec2(640,480));
	
	auto cb = []{
		cout << "Clicked!" << endl;
	};
	
	Font f;
	cout << f.loadFromFile("gothic.ttf") << endl;
	cout << f.isLoaded() << endl;
	
	f.setCharacterSize(50);
	auto ss = f.getSprite('a');
	ss.setColor(vec4::Black);
	cout << ss.getPosition() << "  " << ss.getSize() << "   " << ss.getColor() << endl;
	
	auto btn = new GuiText(win,"Press me!");
	btn->setFont(f);
	// btn->getGuiText().setColor(vec4::White);
	// btn->getGuiText().setCharacterSize(30);
	btn->setPosition(vec2(0,200));
	win.addElement(btn);
	
	auto lay = new GridLayout(win);
	
	lay->addChildElement(new PushButton(win,"Press me! 1",cb));
	lay->addChildElement(new PushButton(win,"Press me! 2",cb));
	lay->addChildElement(new PushButton(win,"Press me! 3",cb));
	lay->addChildElement(new PushButton(win,"Press me! 4",cb));
	lay->addChildElement(new PushButton(win,"Press me! 5",cb));
	
	lay->setPadding(vec2(10));
	
	lay->setPosition(win.getSize()/2 - lay->getSize()/2);
	
	
	{
		Image img;
		img.loadFromFile("hy.jpg");
		win.addSprite("hy_image",img);
		
	}
	
	Sprite s = win.getSprite("hy_image");
	s.setSize(s.getSize()*.1);
	Sprite s2 = s;
	s2.setPosition(vec2(100,0));
	
	win.addElement(lay);
	win.addElement(new GuiText(win, "Made by Csilla"));
	
	win.runGuiLoop();
}

