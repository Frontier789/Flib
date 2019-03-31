#include <Frontier.hpp>

#include <iostream>
using namespace std;

class Plotter : public GuiElement {
	DrawData dd;
	ShaderManager shader;
	float scroll;
public:
	Plotter(GuiContext &cont) : GuiElement(cont,cont.getSize()), scroll(0) {
		dd = Mesh::getRectangle(getSize().w,getSize().h);
		
		auto vertSource = ShaderManager::getDefaultShaderSource(fg::VertexShader);
		auto fragSource = ShaderManager::getDefaultShaderSource(fg::FragmentShader);
		fragSource.uniforms.push_back(ShaderSource::UniformData{"ivec2","size"});
		fragSource.uniforms.push_back(ShaderSource::UniformData{"int","scroll"});
		fragSource.mainBody = R"(
			
			vec2 p = gl_FragCoord.xy;
			p.y = size.y - p.y - 1 + scroll;
			
			p = ivec2(p/2);
			
			int i = int(p.y * size.x + p.x);
			
			bool pr = true;
			for (int j=2;pr && j*j<=i;j++) {
				if (i % j == 0)
					pr = false;
			}
			
			out_color = vec4(pr ? 1 : 0,0,0,1);
		)";
		
		auto res = shader.loadFromMemory(vertSource, fragSource);
		if (!res) {
			cout << res << endl;
			exit(1);
		}
	}
	
	void onDraw(fg::ShaderManager &s) override
	{
		shader.getCamera() = s.getCamera();
		shader.setUniform("size",vec2i(getSize()));
		shader.setUniform("scroll",int(scroll));
		
		shader.draw(dd);
	}
	
	bool onEvent(fw::Event &ev) override {
		
		if (ev.type == Event::Resized) {
			setSize(ev.size);
			dd = Mesh::getRectangle(getSize().w,getSize().h);
		}
		
		if (ev.type == Event::MouseWheelMoved) {
			scroll += ev.wheel.delta*50;
			cout << scroll << endl;
		}
		
		return GuiElement::onEvent(ev);
	}
};

int main()
{
	GuiWindow win(vec2(200,200));
	
	win.addElement(new Plotter(win));
	
	win.runGuiLoop();
}

