#ifndef BOARDDISP_INCLUDED
#define BOARDDISP_INCLUDED
#include <Frontier.hpp>
#include <iostream>

using namespace std;

class BoardDisp : public GuiElement
{
	DrawData m_grid,m_O,m_X;
	vector<vec3i> m_game;
public:
	struct Props {
		int N;
		vec2  mid;
		float siz;
		vec2 base;
		float gsz;
	};
	Props getProps() const {return props;}
private:
	Props props;
	void makeProps();
public:
	BoardDisp(GuiContext &cont);
	
	void prepareDDs();
	
	void addStep(vec2i p);
	void remStep(bool all = false);
	
	void onDraw(fg::ShaderManager &shader) override;

	vec2i cellFromPix(vec2 p) const;
};

#endif // BOARDDISP_INCLUDED
