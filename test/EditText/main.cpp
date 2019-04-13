#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

#include "EditText.hpp"

GuiElement *get(GuiContext &cont,bool free,vec2 p) {

	auto editor = new EditText(cont,200);
	editor->setPosition(p);
	editor->setCharSize(14);
	editor->setFreeView(free);
	editor->setRows(free ? 10 : 15);

	cont.addElement(editor);

	return editor;
}

int main()
{
	GuiWindow win(vec2(640,480),"GuiEditText");
	win.enableKeyRepeat();
	win.setDefaultFont(win.getFont("consola.ttf"));
	win.setMainLayout(new GridLayout(win));

	// sorok számát beállíthatni
	// ne lógjon ki

	auto btn = new PushButton(win,"hello there",[&]{cout << "hy" << endl;});
	auto btn2 = new PushButton(win,"Disable",[&](GuiButton &b){
		btn->setEnabled(!btn->getEnabled());
		b.setText(btn->getEnabled() ? "Disable" : "Enable",false);
	});
	btn->setSize(btn->getSize()*1.5);
	btn2->setSize(btn->getSize()*1.5);
	btn->setBgColor(vec4::Red);
	win.addElement(btn);
	win.addElement(btn2);

	get(win,true,50)->setActive();
	get(win,false,vec2(300,50));

	win.runGuiLoop();
}
