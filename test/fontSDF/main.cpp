#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <fstream>

using namespace std;

class TextTester : public GuiElement
{
	SpriteManager m_spriteMgr;
	std::vector<Sprite> m_sprites;
	fm::String m_sampletext;
public:
	TextTester(GuiContext &owner) : GuiElement(owner,owner.getSize())
	{
		m_sampletext = u8"árvíztűrő tükörfúrógép :)";
		
		buildSprites();
	}
	
	void setThickness(float v)
	{
		m_spriteMgr.getShader().setUniform("u_thickness",v);
	}
	
	void setPov(float v)
	{
		m_spriteMgr.getShader().setUniform("u_pov",v);
	}
	
	void setDscale(float v)
	{
		m_spriteMgr.getShader().setUniform("u_dscale",v);
	}
	
	void buildSprites(bool fastSDF = true)
	{
		// setup shader
		fm::Result res = m_spriteMgr.loadShader([](ShaderSource &source){
			
			if (source.type == fg::VertexShader)
			{
				source.outputs.push_back(fg::ShaderSource::OutputData{"vec2","va_texpEnv[4]"});
				source.mainBody += R"(
	va_texpEnv[0] = vec2(u_texMat * vec4(in_uvp + in_tpt * in_uvs + vec2(1.0,0.0),0.0,1.0));
	va_texpEnv[1] = vec2(u_texMat * vec4(in_uvp + in_tpt * in_uvs + vec2(0.0,1.0),0.0,1.0));
	va_texpEnv[2] = vec2(u_texMat * vec4(in_uvp + in_tpt * in_uvs + vec2(-1.,0.0),0.0,1.0));
	va_texpEnv[3] = vec2(u_texMat * vec4(in_uvp + in_tpt * in_uvs + vec2(0.0,-1.),0.0,1.0));
									 )";
				
			}
			if (source.type == fg::FragmentShader)
			{
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_thickness"});
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_pov"});
				source.uniforms.push_back(fg::ShaderSource::UniformData{"float","u_dscale"});
				source.inputs.  push_back(fg::ShaderSource::OutputData {"vec2","va_texpEnv[4]"});
				source.globals += R"(
float contour(in float d,in float w) 
{
	return smoothstep(0.5 - w, 0.5 + w, d * u_thickness * 2.0);
}

float samp(in vec2 uv,in float w)
{
	return contour(texture2D(u_tex, uv).a, w);
}
									)";
				source.mainBody =/* R"(
				
				
				vec4 c = texture2D(u_tex,va_texp);
	
    float dist = c.a;

	float width = pow(fwidth(dist),0.2 / u_pov);

    float alpha = contour( dist, width );

    float dscale = 0.354;
    vec2 duv = vec2(dscale) * (dFdx(va_texp) + dFdy(va_texp));
    vec4 box = vec4(va_texp-duv, va_texp+duv);

    float asum = samp( box.xy, width )
               + samp( box.zw, width )
               + samp( box.xw, width )
               + samp( box.zy, width );

	alpha = (alpha + 0.4 * asum) / (1.0 + 0.4*4.0);

	out_color = vec4(vec3(0.0), alpha);
				
				)";*/
				
				R"(
	float dist = texture2D(u_tex,va_texp).a;
	float distEnv[4] = float[4]( texture2D(u_tex,va_texpEnv[0]).a, 
								 texture2D(u_tex,va_texpEnv[1]).a, 
								 texture2D(u_tex,va_texpEnv[2]).a, 
								 texture2D(u_tex,va_texpEnv[3]).a );

	float width = pow( abs(distEnv[0] - dist) + abs(distEnv[1] - dist),0.4 / u_pov);

	float alpha = contour( dist, width );

	float dscale = u_dscale;
	vec2 duv = vec2(dscale) * (abs(va_texpEnv[0] - va_texp) + abs(va_texpEnv[1] - va_texp));
	vec4 box = vec4(va_texp-duv, va_texp+duv);

	float asum = samp( box.xy, width )
			   + samp( box.zw, width )
			   + samp( box.xw, width )
			   + samp( box.zy, width );

	alpha = (alpha + 0.4 * asum) / (1.0 + 0.4*4.0);

	out_color = vec4(vec3(0.0), alpha);
									)";
			}
			
		});
		if (!res) cout << res << endl;
		
		m_spriteMgr.getAtlas().reset();
		m_sprites.clear();
		
		// render glyphs
		Font font = getOwnerContext().getDefaultFont();
		font.setCharacterSize(200);
		std::vector<fg::Image *> renderImgs(m_sampletext.size());
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			fg::Image img = font.renderGlyph(m_sampletext[i]);
			renderImgs[i] = new fg::Image(std::move(img));
		}
		
		std::string keyName = fastSDF ? "fast" : "nice";
		
		std::vector<fm::Time > times(m_sampletext.size());
		
		// render sdfs
		float maxH = 0;
		std::vector<fg::Image *> sdfImgs(m_sampletext.size());
		std::vector<vec2> sdfSizes(m_sampletext.size());
		cout << "rendering " << keyName << endl;
		fm::Clock clkAll,clkOne;
		
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			fg::Image img = renderImgs[i]->convertToSDF(10,[](Color c){return c.a > 127;},fastSDF ? 0 : 100);
			maxH = max<float>(maxH,img.getSize().h*.1);
			
			sdfImgs[i] = new fg::Image(std::move(img));
			*sdfImgs[i] = std::move(sdfImgs[i]->scale(sdfImgs[i]->getSize()*.1));
			m_spriteMgr.addImage(*sdfImgs[i],fm::toString(i)+"_"+keyName);
			sdfSizes[i] = sdfImgs[i]->getSize();
		
			times[i] = clkOne.getTime();
			clkOne.restart();
		}
		fm::Time t = clkAll.getTime();
		for (fm::Size i=0;i<m_sampletext.size();++i)
			cout << i << ": " << times[i].asSecs() << "s\n";
		cout << "all: " << t.asSecs() << "s\n";
		cout << endl;
		
		for (fm::Size i=0;i<m_sampletext.size();++i) delete sdfImgs[i];
		for (fm::Size i=0;i<m_sampletext.size();++i) delete renderImgs[i];
		
		vec2 offset(0,200);
		// generate sprites
		float mul = .5;
		float scale = 2;
		for (int sizes=0;sizes<20;++sizes)
		{
			for (fm::Size i=0;i<m_sampletext.size();++i)
			{
				m_sprites.emplace_back(m_spriteMgr,fm::toString(i)+"_"+keyName);
				vec2 s = sdfSizes[i] * mul * scale;
				m_sprites.back().setSize(s);
				m_sprites.back().setPosition(offset + font.getGlyph(m_sampletext[i]).leftdown * mul * .1 * scale);
				offset.x += s.x+1;
			}
			offset.x = 0;
			offset.y += maxH * mul * scale;
			mul *= .9;
		}
	}
	
	void onDraw(fg::ShaderManager &shader) override
	{
		m_spriteMgr.onDraw(shader);
	}
};

int main()
{
	GuiWindow win(vec2(640,480),"sdf");
	win.setClearColor(vec4::White);

	TextTester *tester = new TextTester(win);
	win.getMainLayout().addChildElement(tester);
	
	Image img;
	Result r = img.loadFromFile("bunny.png");
	if (!r) cout << r << endl;
	
	Texture bunnyTex(img);
	
	fw::MouseCursor cur(img,img.getSize()/2);
	
	PushButton *pb = new PushButton(win,"lol",[&](){
		Mouse::setCursor(cur);
	});
	pb->setPosition(vec2(0,54));
	
	win.getMainLayout().addChildElement(pb);
	
	
	ScrollBar *sbP = new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		tester->setThickness(sb.getScrollState());
	});
	
	win.getMainLayout().addChildElement(sbP);
	
	ScrollBar *sb = new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		tester->setPov(sb.getScrollState());
	});
	
	sb->setPosition(vec2(0,18)); 
	
	win.getMainLayout().addChildElement(sb);
	
	ScrollBar *sb2 = new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		tester->setDscale(sb.getScrollState());
	});
	
	sb2->setPosition(vec2(0,36)); 
	
	win.getMainLayout().addChildElement(sb2);
	
	bool fastSDF = true;
	
	pb = new PushButton(win,fastSDF ? "fastSDF" : "niceSDF",[&](){
		fastSDF = !fastSDF;
		tester->buildSprites(fastSDF);
		pb->setText(fastSDF ? "fastSDF" : "niceSDF");
		
		tester->setThickness(sbP->getScrollState());
		tester->setPov(sb->getScrollState());
		tester->setDscale(sb2->getScrollState());
	});
	pb->setPosition(vec2(0,80));
	
	win.getMainLayout().addChildElement(pb);
	
	FrameBuffer fbo;
	Texture tex;
	
	bool running = true;
	for (;running;)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::FocusLost) running = false;
		}
		
		win.clear();
		
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}

