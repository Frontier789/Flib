#include <Frontier.hpp>
#include <iostream>

using namespace std;

int main()
{
	cout << "left click: put new points" << endl
		 << "right click: clear" << endl
		 << "mouse wheel: change thickness" << endl
		 << "a: toggle antialiasing" << endl;
	
	GuiWindow win(vec2(640,480),"Thicc");
	
	bool running = true;
	
	vector<vec2> pts;
	
	DrawData dd;
	
	ShaderSource vertSource{130,{{"FRONTIER_MODEL ",""},
								 {"FRONTIER_VIEW   ",""},
								 {"FRONTIER_PROJ   ",""},
								 {"FRONTIER_POS    ",""},
								 {"FRONTIER_CLR    ",""},
								 {"FRONTIER_CUSTOM0",""},
								 {"FRONTIER_CLRMAT ",""}},
								{{"mat4 FRONTIER_MODEL   ","u_modelMat"},
								 {"mat4 FRONTIER_VIEW    ","u_viewMat "},
								 {"mat4 FRONTIER_CLRMAT  ","u_colorMat"},
								 {"mat4 FRONTIER_PROJ    ","u_projMat "}},
								{{"vec3 FRONTIER_POS     ","in_pos    "},
								 {"vec4 FRONTIER_CUSTOM0 ","in_distf  "},
								 {"vec4 FRONTIER_CLR     ","in_color  "}},
								{{"vec4","va_color "},
								 {"vec4","va_distf "}},
								"",
								{},
								R"(
		gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_pos,1.0);
		
		va_color = u_colorMat * in_color;
		va_distf = in_distf;
									)",
								fg::VertexShader};
	
	ShaderSource fragSource{130,{},
								{{"float","u_antia"}},
								{{"vec4","va_color"},
								 {"vec4","va_distf"}},
								{{"vec4","out_color"}},
								"",
								{},
								R"(
		float d = min(min(va_distf.x,va_distf.y),min(va_distf.z,va_distf.w));
		d = min(d/u_antia,1);
		out_color = vec4(va_color.rgb,va_color.a * d);
									)",
								fg::FragmentShader};
	
	ShaderManager shader;
	
	fm::Result res = shader.loadFromMemory(vertSource,fragSource);
	if (!res)
	{
		cout << res << endl;
		return 1;
	}
	
	win.setShader(&shader);
	
	shader.setUniform("u_antia",0.0001f);
	int antia = 0;
	
	float lineW = 2.3;
	bool rebuild = false;
	
	while (running)
	{
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::A)
				{
					float antis[] = {0.0001,1.1};
					antia = (antia+1)%(sizeof(antis) / sizeof(*antis));
					shader.setUniform("u_antia",antis[antia]);
				}
			}
			if (ev.type == Event::MouseWheelMoved)
			{
				lineW *= pow(1.1,ev.wheel.delta);
				rebuild = true;
			}
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					pts.push_back(ev.mouse);
					rebuild = true;
				}
				else
				{
					pts.clear();
					dd.reset();
				}
			}
		}
		
		if (rebuild)
		{
			Mesh m = Mesh::tesLineStrip(&pts[0],pts.size(),lineW,true);
			m.clr.resize(m.pts.size(),vec4::Black);
			
			dd = m;
			dd.clearDraws();
			dd.addDraw(0,m.pts.size(),fg::Triangles);
		}
		
		win.updateElements();
		
		win.clear();
		
		win.draw(dd);
		
		win.swapBuffers();
		win.applyFpsLimit();
	}
	
	win.setShader(nullptr,false);
}
