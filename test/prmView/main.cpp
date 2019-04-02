#include <Frontier.hpp>

#include <iostream>
using namespace std;

int main()
{
	GuiWindow win(vec2(640,480));
	
	auto btn = new PushButton(win,"hello");
	btn->setBgColor(vec4::Red);
	
	win.addElement(btn);
	
	win.runGuiLoop();
}

