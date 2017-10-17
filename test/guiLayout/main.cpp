#include <Frontier.hpp>
#include <iostream>

#include "ColorPicker.hpp"

using namespace std;

class SingletonDrawer
{
	DrawData dd;
public:
	SingletonDrawer()
	{
		vec2 pts[] = {vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,1)};
		vec4 clr[] = {vec4::Red,vec4::Red,vec4::Red,vec4::Red};
		
		dd.positions = pts;
		dd.colors = clr;
		dd.addDraw(fg::LineLoop);
	}
	
	void operator()(vec2 p,vec2 s,ShaderManager &shader)
	{
		shader.getModelStack().push().mul(MATRIX::translation(p) * MATRIX::scaling(s));
		
		shader.useTexture(nullptr);
		shader.draw(dd);
		
		shader.getModelStack().pop();
	}
};

int main()
{
	GuiWindow win(vec2(640,480),"gui");
	
	SingletonDrawer drawRedRect;
	
	int push_count = 0;
	
	GridLayout *l = new GridLayout(win);
	
	l->setCellCount(vec2(2,3));
	
	l->setChildElement2D(vec2(0,0),new GuiText(win,"R: "));
	l->setChildElement2D(vec2(0,1),new GuiText(win,"G: "));
	l->setChildElement2D(vec2(0,2),new GuiText(win,"B: "));
	
	Color colorRD(0,0,0);
	
	auto updateTitle = [&](){
		win.setTitle("gui (" + fm::toString(colorRD.r) + "," 
							 + fm::toString(colorRD.g) + ","
							 + fm::toString(colorRD.b) + ")");
	};
	
	l->setChildElement2D(vec2(1,0),new ScrollBar(win,vec2(50,15),[&](GuiScrollBar &sb){colorRD.r = sb.getScrollState()*255; updateTitle();}));
	l->setChildElement2D(vec2(1,1),new ScrollBar(win,vec2(50,15),[&](GuiScrollBar &sb){colorRD.g = sb.getScrollState()*255; updateTitle();}));
	l->setChildElement2D(vec2(1,2),new ScrollBar(win,vec2(50,15),[&](GuiScrollBar &sb){colorRD.b = sb.getScrollState()*255; updateTitle();}));
	
	l->setPadding(vec2(5,2));
	
	l->setPosition(win.getSize() - l->getSize());
	
	win.getMainLayout().addChildElement(l);
	
	C(10)
	{
		PushButton *pb = new PushButton(win,"push me");	
		pb->setPosition(vec2(0,i * pb->getSize().h + 5*i));
		
		pb->setCallback([&](GuiButton &b) {
			b.setText("yayy pushed " + fm::toString(++push_count) + " times");
			b.setSize(b.getGuiText().getSize() + vec2(16,10));
			win.setClearColor(vec4::White);
		});
		
		win.getMainLayout().addChildElement(pb);
	}
	
	ColorPicker *cp = new ColorPicker(win);
	cp->setPosition(vec2(150,150));
	cp->setCallback([&](ColorPicker &cp,Color c) {
		win.setClearColor(c.rgba()/255.0);
	});
	
	win.getMainLayout().addChildElement(cp);
	
	ScrollBar *ssb = new ScrollBar(win);
	ssb->setSize(vec2(25,300));
	ssb->setPosition(vec2(640 - 20 - ssb->getSize().w,0));
	ssb->setCallback([&](GuiScrollBar &sb){
		cp->raiseBrightness(sb.getScrollState());
	});
	
	ssb->setScrollSize(0.05);
	
	win.getMainLayout().addChildElement(ssb);
	
	bool showLayout = false;
	
	PushButton *showBtn = new PushButton(win,"ShowLayout",[&](GuiButton &pb){
		
		showLayout = !showLayout;
		
		if ( showLayout) pb.setText("HideLayout");
		if (!showLayout) pb.setText("ShowLayout");
		
	});
	
	showBtn->setPosition((win.getSize() - showBtn->getSize()) * vec2(0,1));
	
	win.getMainLayout().addChildElement(showBtn);
	
	bool running = true;
	while (running)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
		}
		
		win.clear();
		win.drawElements();
		if (showLayout)
			win.getMainLayout().traverseHierarchy([&](GuiElement &e) {
				drawRedRect(e.getPosition(),e.getSize(),win.getShader());
			});
		
		showBtn->GuiButton::traverseHierarchy([&](GuiElement &e) {
			drawRedRect(e.getPosition(),e.getSize(),win.getShader());
		});
		
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}




