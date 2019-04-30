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

	// ne lógjon ki
	// Ha felfele rántom az egeret se váltson vissza mutatóra, az I-ről
	
	get(win,true,50)->setActive();
	get(win,false,vec2(300,50));

	win.runGuiLoop();
}
