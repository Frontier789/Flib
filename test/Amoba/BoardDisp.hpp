#ifndef BOARDDISP_INCLUDED
#define BOARDDISP_INCLUDED
#include <Frontier.hpp>
#include <iostream>

using namespace std;

class BoardDisp : public Widget
{
	DrawData m_grid,m_O,m_X;
	vector<vec3i> m_moves;
	int m_size;
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
	BoardDisp(GuiContext &cont,int size);
	
	void prepareDDs();
	
	void addStep(vec2i p,int ply);
	void remStep(bool all = false);
	
	void onDraw(fg::ShaderManager &shader) override;
};

#endif // BOARDDISP_INCLUDED
