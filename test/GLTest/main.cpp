#include <FRONTIER/OpenGL.hpp>
#include <Frontier.hpp>
#include <iostream>

using namespace std;

bool bufferTest(ostream &out)
{
	out << "--- -- --- Buffer test --- -- ---" << endl;
	vec2 data[3] = {vec2(1,2),vec2(3,5),vec2(0,1)};
	bool ok;
	
	Buffer buf;
	
	fm::Result res;
	
	// --- //
	res += buf.setData(data);
	
	ok = (glIsBuffer(buf.getGlId()) == GL_TRUE);
	out << "Buffer.setData creates buffer: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	GLint size;
	glBindBuffer(GL_ARRAY_BUFFER, buf.getGlId());
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	ok = (size == sizeof(data));
	out << "Buffer.setData uploads correct size: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	float *ptr;
	res += buf.map(ptr,BufferReadOnly);
	
	ok = (ptr != nullptr);
	out << "Buffer.map doesnt return nullptr: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	bool allmatch = true;
	for (fm::Size i=0;i<sizeof(data)/sizeof(*data);++i)
		allmatch = allmatch && data[i].x == ptr[2*i+0] && data[i].y == ptr[2*i+1];
	
	ok = allmatch;
	out << "Buffer.map maps correct values: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	res += buf.unMap();
	
	// --- //
	float upd[] = {1,8,9};
	data[0].y = upd[0]; data[1].x = upd[1]; data[1].y = upd[2];
	res += buf.updateData(upd,sizeof(upd),sizeof(float));
	
	ptr = buf.map<float>(BufferReadOnly);
	allmatch = true;
	for (fm::Size i=0;i<sizeof(data)/sizeof(*data);++i)
		allmatch = allmatch && data[i].x == ptr[2*i+0] && data[i].y == ptr[2*i+1];
	
	ok = allmatch;
	out << "Buffer.updateData updates values: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	res += buf.unMap();
	
	// --- //
	if (!res)
	{
		out << "Some operation resulted in GLError: " << res << endl;
		return false;
	}
	
	return true;
}

bool textureTest(ostream &out)
{
	out << "--- -- --- Texture test --- -- ---" << endl;
	
	FloatTexture ftex;
	Texture tex;
	fm::Result res;
	bool ok;
	
	vec4 cv(.128,.159,.85,.75);
	Color cc = cv*255;
	
	// --- //
	res += tex.create(vec2(123,26),cv);
	
	ok = glIsTexture(tex.getGlId());
	out << "Texture.create creates texture: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	Image img = tex.copyToImage();
	
	ok = (img.getSize() == vec2(123,26));
	out << "Texture.copyToImage reads correct size: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	bool allmatch = true;
	img.forEach([&](vec2s,Color c) { allmatch = allmatch && c == cc; } );
	
	ok = allmatch;
	out << "Texture.copyToImage reads good values: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	res += ftex.create(vec2(70,70));
	
	ok = (glIsTexture(ftex.getGlId()) == GL_TRUE);
	out << "FloatTexture.create creates texture: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	vec4 ft[4*3];
	for (vec4 &v : ft) v = vec4(1.78,8.13,300.1);
	res += ftex.loadFromArray((float*)ft,vec2s(4,3));
	
	for (vec4 &v : ft) v = vec4(-1,-2,-3,-4);
	res += ftex.copyToArray((float*)ft);
	
	allmatch = true;
	for (vec4 v : ft) allmatch = allmatch && v == vec4(1.78,8.13,300.1);
	
	ok = allmatch;
	out << "FloatTexture.copyToArray copies correct values: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	if (!res)
	{
		out << "Some operation resulted in GLError: " << res << endl;
		return false;
	}
	
	return true;
}

bool fboTest(ostream &out)
{
	out << "--- -- --- FrameBuffer test --- -- ---" << endl;
	
	FrameBuffer fbo;
	fm::Result res;
	bool allmatch;
	Texture tex;
	Image img;
	bool ok;
	
	// --- //
	res += tex.create(vec2(640,480),vec4(.5,.65,.75));
	res += fbo.create(tex);
	
	ok = glIsFramebuffer(fbo.getGlId());
	out << "FrameBuffer.create creates fbo: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	// --- //
	vec4 c = vec4(.1,.1,.1);
	Color cc = c*255;
	fbo.setClearColor(c);
	res += fbo.bind();
	fbo.clear();
	
	img = tex.copyToImage();
	allmatch = true;
	
	img.forEach([&](vec2s,Color c) { allmatch = allmatch && c == cc; } );
	
	ok = allmatch;
	out << "FrameBuffer.clear works correctly: " << boolalpha << ok << endl;
	if (!ok) return false;
	
	
	// --- //
	if (!res)
	{
		out << "Some operation resulted in GLError: " << res << endl;
		return false;
	}
	
	return true;
}

bool clrMatch(Color c1,Color c2)
{
	vec4 d = vec4(c1.rgba()) - vec4(c2.rgba());

	return (abs(d.x) + abs(d.y) + abs(d.z) + abs(d.w)) < 4;
}

bool shaderTest(ostream &out,bool createPics = true)
{
	out << "--- -- --- Shader test --- -- ---" << endl;
	
	vec2 pts[] = {vec2(0,0),vec2(0,1),vec2(1,0),vec2(0,1),vec2(1,0),vec2(1,1)};
	vec4 pts2[] = {vec4(0,-1,-1,0),vec4(0,-1,0,0),vec4(0,0,-1,0),vec4(0,-1,0,0),vec4(0,0,-1,0),vec4(0,0,0,0)};
	float dat3[] = {-1,-1, 1,0,0,  -1,1, 1,1,0,  1,-1, 0,1,1,  1,1, 0,0,1};
	
	FrameBuffer fbo;
	Buffer buf,buf2,buf3;
	fm::Result res;
	Shader shader2;
	Shader shader;
	Texture tex;
	bool ok = true;
	
	// --- //
	res += shader.loadFromMemory(R"(
#version 330

in vec2 in_pos;

void main()
{
    gl_Position = vec4(in_pos,0,1);
})",R"(
#version 330

out vec4 out_color;

uniform vec4 u_clr;

void main()
{
    out_color = u_clr;
})");
	
	ok = ok && glIsProgram(shader.getGlId());
	out << "Shader.create creates shader program: " << boolalpha << ok << endl;
	//if (!ok && res) return false;
	
	// --- //
	res += buf.setData(pts);
	res += shader.setAttribute("in_pos",buf);
	res += shader.setUniform("u_clr",vec4(.25,.5,.75,1));
	
	res += tex.create(vec2(128,128),vec4::White);
	res += fbo.create(tex);
	
	res += fbo.bind();
	res += shader.bind();
	res += glCheck(glDrawArrays(GL_TRIANGLES,0,sizeof(pts)/sizeof(*pts)));
	
	Image img = tex.copyToImage();
	if (createPics) img.saveToFile("s0.png");
	
	bool allmatch = true;
	img.forEach([&](vec2s p,Color c){
		Color correct = ((p.x >= 64 && p.y >= 64) ? Color(64,128,191,255) : Color::White);
		allmatch = allmatch && clrMatch(c,correct);
	});
	
	ok = ok && allmatch;
	out << "Shader.setAttribPointer works with buffer: " << boolalpha << ok << endl;
	// if (!ok && res) return false;
	
	// --- //
	res += buf2.setData(pts2);
	res += shader.setAttribute<vec2>("in_pos",buf2,sizeof(vec4),sizeof(float));
	shader.setUniform("u_clr",vec4(.95,.22,.18,1));
	
	// shader.draw maybe?
	// vao.setAttrib to value
	res += glCheck(glDrawArrays(GL_TRIANGLES,0,sizeof(pts2)/sizeof(*pts2)));
	
	img = tex.copyToImage();
	if (createPics) img.saveToFile("s1.png");
	
	allmatch = true;
	img.forEach([&](vec2s p,Color &c){
		Color correct = ((p.x >= 64 && p.y >= 64) ? Color(64,128,191,255) : Color::White);
		if (p.x < 64 && p.y < 64) correct = Color(242,56,46,255);
		allmatch = allmatch && clrMatch(c,correct);
	});
	
	ok = ok && allmatch;
	out << "Shader.setAttribPointer works on interleaved attributes: " << boolalpha << ok << endl;
	// if (!ok && res) return false;
	
	// --- //
	res += shader2.loadFromMemory(R"(
#version 330

in vec2 in_pos;
in vec3 in_clr;

out vec3 va_clr;

void main()
{
    gl_Position = vec4(in_pos,0,1);
	va_clr = in_clr;
})",R"(
#version 330

in vec3 va_clr;

out vec4 out_color;

uniform vec4 u_clr;

void main()
{
    out_color = u_clr * vec4(va_clr,1);
})");

	res += buf3.setData(dat3);
	res += shader2.setAttribute<vec2>("in_pos",buf3,sizeof(float)*5);
	res += shader2.setAttribute<vec3>("in_clr",buf3,sizeof(float)*5,sizeof(float)*2);
	shader2.setUniform("u_clr",vec4(1,0,1,1));
	shader2.bind();
	res += glCheck(glDrawArrays(GL_TRIANGLE_STRIP,0,4));
	
	img = tex.copyToImage();
	if (createPics) img.saveToFile("s2.png");
	
	allmatch = true;
	for (fm::Size x=0;x<img.getSize().w;++x)
	{
		for (fm::Size y=1;y<img.getSize().h;++y)
		{
			allmatch = allmatch && img.getTexel(vec2s(x,y)) == img.getTexel(vec2s(x,y-1));
		}
		if (x)
			allmatch = allmatch && img.getTexel(vec2s(x,0)).r <= img.getTexel(vec2s(x-1,0)).r
								&& img.getTexel(vec2s(x,0)).b >= img.getTexel(vec2s(x-1,0)).b;
		
		allmatch = allmatch && img.getTexel(vec2s(x,0)).g == 0;
	}
	
	ok = ok && allmatch;
	out << "Interpolating Shader with float attrs works: " << boolalpha << ok << endl;
	// if (!ok && res) return false;
	
	// --- //
	res += shader.bind();
	res += glCheck(glDrawArrays(GL_TRIANGLES,0,sizeof(pts)/sizeof(*pts)));
	
	img = tex.copyToImage();
	if (createPics) img.saveToFile("s3.png");
	
	allmatch = true;
	for (fm::Size x=0;x<img.getSize().w;++x)
	{
		for (fm::Size y=0;y<img.getSize().h;++y)
		{
			if (x < 64 && y < 64)
				allmatch = allmatch && img.getTexel(vec2s(x,y)) == Color(242,56,46,255);
			else if ((x >= 64 && y) || y > 64)
				allmatch = allmatch && img.getTexel(vec2s(x,y)) == img.getTexel(vec2s(x,y-1));
		}
		if (x >= 64)
		{
			allmatch = allmatch && img.getTexel(vec2s(x,0)).r <= img.getTexel(vec2s(x-1,0)).r
								&& img.getTexel(vec2s(x,0)).b >= img.getTexel(vec2s(x-1,0)).b;
		
			allmatch = allmatch && img.getTexel(vec2s(x,0)).g == 0;
		}
	}
	
	ok = ok && allmatch;
	out << "Shaders keep their set attributes: " << boolalpha << ok << endl;
	// if (!ok && res) return false;
	
	// --- //
	res += shader2.setAttribute("in_clr",vec4::Red);
	res += shader2.bind();
	res += glCheck(glDrawArrays(GL_TRIANGLE_STRIP,0,4));
	
	img = tex.copyToImage();
	if (createPics) img.saveToFile("s4.png");
	
	allmatch = true;
	img.forEach([&](vec2s,Color c) { allmatch = allmatch && clrMatch(c,Color::Red); } );
	
	ok = ok && allmatch;
	out << "Shader.setAttribute works with constants: " << boolalpha << ok << endl;
	// if (!ok && res) return false;
	
	// --- //
	if (!res)
	{
		out << "Some operation resulted in GLError: " << res << endl;
		return false;
	}
	
	return ok;
}
/*
bool shaderManagerTest(ostream &out,bool createPics = true)
{
	out << "--- -- --- ShaderManager test --- -- ---" << endl;
	
	ShaderManager shader;
	fm::Result res;
	bool ok;
	
	// --- //
	
	
	return true;
}
*/
void testContext(ostream &out)
{
	out << "OpenGL version:    " << glGetString(GL_VERSION) << endl;
	out << "OpenGL vendor:     " << glGetString(GL_VENDOR) << endl;
	out << "OpenGL renderer:   " << glGetString(GL_RENDERER) << endl;
	out << "OpenGL SL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	out << endl;
	
	out << "--- -- --- Flib Graphics test --- -- ---" << endl;
	out << "Buffers available: " << boolalpha << Buffer::isAvailable() << endl;
	out << "Framebuffer available: " << boolalpha << FrameBuffer::isAvailable() << endl;
	out << "Vertexarray available: " << boolalpha << VertexArray::isAvailable() << endl;
	out << "Compute shader available: " << boolalpha << ComputeShader::isAvailable() << endl;
	out << endl;
	
	if (!Buffer::isAvailable()) return;
	
	bool ok;
	
	ok = bufferTest(out);
	if (!ok) out << "Error: Buffer test failed" << endl;
	out << endl;
	
	ok = textureTest(out);
	if (!ok) out << "Error: Texture test failed" << endl;
	out << endl;
	
	ok = fboTest(out);
	if (!ok) out << "Error: FrameBuffer test failed" << endl;
	out << endl;
	
	ok = shaderTest(out);
	if (!ok) out << "Error: Shader test failed" << endl;
}

int main()
{
	Window win(vec2(640,480),"GL",Window::Default | Window::Hidden);
	
	testContext(cout);
}
