#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

void check(fm::Result res)
{
	if (!res)
	{
		cout << res << endl;
		std::exit(1);
	}
}

float gauss(float dx,float dy,float sigma2)
{
	float d = dx*dx + dy*dy;
	return exp(-d / 2.0 / sigma2) / sqrt(2*3.1415*sigma2);
}

int main()
{
	GLContext cont;
	fm::Result res;
	
	res += cont.create();
	res += cont.setActive();
	check(res);
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR)  << endl;
	
	Texture inTex,outTex;
	res += inTex.loadFromFile("rose.png");
	res += outTex.create(inTex.getSize());
	check(res);
	
	ComputeShader cshader;
	res += cshader.loadFromFile("compute.glsl");
	check(res);

	cshader.bind();
	cshader.setUniform("u_kernSize",12);
	res += cshader.setUniform("u_inTex",inTex);
	res += cshader.setUniform("u_outTex",outTex);
	check(res);
	
	cshader.bind();
	float gaussData[12][12];
	fg::Buffer gaussbuf;
	Cxy(12,12) gaussData[x][y] = gauss(x,y,8);
	gaussbuf.setData(gaussData);
	res += cshader.setStorageBuf(3,gaussbuf);
	
	res += cshader.dispatch(inTex.getSize());
	check(res);
	
	fg::Image img;
	res += outTex.copyToImage(img);
	check(res);

	img.saveToFile("pic.png");
}
