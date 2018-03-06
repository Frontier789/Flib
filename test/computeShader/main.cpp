#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

/*
https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shader_storage_buffer_object.txt
*/

void check(fm::Result res)
{
	if (!res)
	{
		cout << res << endl;
		std::exit(1);
	}
}

int main()
{
	GLContext cont;
	fm::Result res;
	
	res += cont.create();
	res += cont.setActive();
	check(res);
	
	Texture inTex,outTex;
	res += inTex.loadFromFile("rose.png");
	res += outTex.create(inTex.getSize());
	check(res);
	
	ComputeShader cshader;
	res += cshader.loadFromFile("compute.glsl");
	check(res);

	cshader.bind();
	cshader.setUniform("u_kernSize",5);
	res += cshader.setUniform("u_inTex",inTex);
	res += cshader.setUniform("u_outTex",outTex);
	check(res);
	
	res += cshader.dispatch(inTex.getSize());
	check(res);
	
	fg::Image img;
	res += outTex.copyToImage(img);
	check(res);

	img.saveToFile("pic.png");
}
