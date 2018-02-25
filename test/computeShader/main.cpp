#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
#include <iostream>

using namespace std;

float fix_float(float f)
{
	if (f < 0) return 1-fix_float(abs(f));

	f = fmod(f,2);
	if (f > 1) f = 2-f;

	return f;
}

int main()
{
	GLContext cont;
	
	fm::Result res = cont.create(vec2(100,100));
	if (res)   res = cont.setActive();

	if (!res)
	{
		cout << res << endl;
		return 1;
	}
	
	FloatTexture tex;
	tex.create(vec2(512,512),vec4::Red);
	
	FloatTexture addTex;
	addTex.create(vec2(512,512),vec4::Green);

	ComputeShader cshader;
	res = cshader.loadFromFile("compute.glsl");

	if (!res)
	{
		cout << res << endl;
		return 1;
	}

	cshader.bind();
	res += cshader.setUniform("destTex",tex);
	res += cshader.setUniform("addTex",addTex);

	if (!res)
	{
		cout << res << endl;
	}

	for (int i = 0; i < 1024; ++i) 
	{
		cshader.bind();

		cshader.setUniform("roll",i*0.1f);
		auto res = cshader.dispatch(vec3(512/16,512/16,1)); // 512^2 threads in blocks of 16^2
		
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		if (!res) cout << res << endl;
	}

	fg::Image img;
	res = tex.copyToImage(img);

	if (!res)
	{
		cout << res << endl;
		return 1;
	}

	img.saveToFile("pic.png");
}
