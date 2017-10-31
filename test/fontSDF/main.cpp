#include <Frontier.hpp>
#include <iostream>
#include <fstream>

using namespace std;


class SDFData
{
public:
	fm::vec2i nearestPix;
	bool processed;
	bool inShape;
	
	SDFData() : processed(false),
				inShape(false)
	{
		
	}
};

class SDFDataField
{
public:
	std::vector<SDFData> data;
	fm::vec2s size;
	SDFData dummy;
	
	SDFDataField(fm::vec2s s) : data(s.area(),SDFData()),
								size(s)
	{
		dummy.processed = true;
	}
	
	SDFData &at(fm::vec2i p)
	{
		if (p.x < 0 || p.y < 0 || p.x >= (int)size.w || p.y >= (int)size.h)
			return dummy;
		
		return data[p.y * size.w + p.x];
	}
	
	inline SDFData &operator[](fm::vec2i p)
	{
		return at(p);
	}
	
	bool isEdge(fm::vec2i p)
	{
		fm::Size neighboursIn = 0;
		neighboursIn += (int)at(p+vec2i(1,0)).inShape;
		neighboursIn += (int)at(p+vec2i(0,1)).inShape;
		neighboursIn += (int)at(p+vec2i(-1,0)).inShape;
		neighboursIn += (int)at(p+vec2i(0,-1)).inShape;
		
		if (neighboursIn > 0 && neighboursIn < 4) return true;
		
		return (neighboursIn == 0 &&  at(p).inShape) || 
			   (neighboursIn == 4 && !at(p).inShape);
	}
};

#include <queue>
Image imageToSDF_manhattan(const Image &img,int maxDist,Delegate<bool,Color> inF = [](Color c){return c.a > 127;}) 
{
	auto distFunc = [](vec2i v) -> float {
		return abs(v.x) + abs(v.y);
	};
	
	SDFDataField data(img.getSize() + vec2(maxDist*2));
	
	Cv(img.getSize())
		data[p + vec2(maxDist)].inShape = inF(img.getTexel(p));
	
	std::queue<fm::vec2i> sorOut;
	std::queue<fm::vec2i> sorIn;
	
	Cv(data.size)
	{
		if (data[p].inShape && data.isEdge(p))
			sorOut.push(p);
			
		if (!data[p].inShape && data.isEdge(p))
			sorIn.push(p);
	}
	
	fm::vec2i ns[] = {fm::vec2i(1,0),fm::vec2i(0,1),fm::vec2i(-1,0),fm::vec2i(0,-1)};
	
	for (int K=0;K<2;++K)
	{
		std::queue<fm::vec2i> &sor = K ? sorIn : sorOut;
			
		while (!sor.empty())
		{
			fm::vec2i p = sor.front();
			sor.pop();
			
			
			for (auto n : ns)
			{
				SDFData &d = data[p + n];
				
				if (!d.processed && ((!K && !d.inShape) || (K && d.inShape)))
				{
					fm::vec2i np = data[p].nearestPix - n;
					
					if (distFunc(np) <= maxDist)
					{
						d.processed  = true;
						d.nearestPix = np;
						sor.push(p+n);
					}
				}
			}
		}
	}
	
	Image ret;
	ret.create(data.size);
	
	Cv(data.size)
	{
		bool ins = data[p].inShape;
		
		if (data[p].processed)
		{
			vec2i np = data[p].nearestPix;
			
			float dist = distFunc(np);
			
			float v;
			
			if (ins)
				v = dist/maxDist * .5 + .5;
			else
				v = (1 - dist/maxDist) * .5;
			
			ret.setTexel(p,Color(255,max(min(v,1.f),0.f)*255));
		}
		else
		{
			ret.setTexel(p,Color(255,ins ? 255 : 0));
		}
	}
	
	return ret;
}

Image imageToSDF_brute(const Image &img,int maxDist,Delegate<bool,Color> inF = [](Color c){return c.a > 127;})
{
	SDFDataField data(img.getSize() + vec2(maxDist*2));
	
	Cv(img.getSize())
		data[p + vec2(maxDist)].inShape = inF(img.getTexel(p));
	
	Cv(data.size)
	{
		if (data[p].inShape && data.isEdge(p))
		{
			Cxy(maxDist*2,maxDist*2)
			{
				vec2 d(float(x) - maxDist,float(y) - maxDist);
				
				if (!data[p + d].processed || vec2(data[p + d].nearestPix).length() > d.length())
				{
					data[p + d].nearestPix = d;
					data[p + d].processed = true;
				}
			}
		}
	}
	
	Image ret;
	ret.create(data.size);
	
	srand(time(0));
	Cv(data.size)
	{
		bool ins = data[p].inShape;
		
		if (data[p].processed)
		{
			float dist = vec2(data[p].nearestPix).length();
			
			float v;
			
			if (ins)
				v = dist/maxDist * .5 + .5;
			else
				v = (1 - dist/maxDist) * .5;
			
			ret.setTexel(p,Color(255,max(min(v,1.f),0.f)*255));
		}
		else
		{
			ret.setTexel(p,fg::Color(255,ins ? 255 : 0));
		}
	}
	
	return ret;
}

Image imageToSDF_kdTree(const Image &img,int maxDist,Delegate<bool,Color> inF = [](Color c){return c.a > 127;})
{
	SDFDataField data(img.getSize() + vec2(maxDist*2));
	
	Cv(img.getSize())
		data[p + vec2(maxDist)].inShape = inF(img.getTexel(p));
	
	std::vector<vec2> pts;
	
	Cv(data.size)
		if (data[p].inShape && data.isEdge(p))
			pts.push_back(p);
	
	fm::KDTree2 kdTree(std::move(pts)); 
	
	Image ret;
	ret.create(data.size);
	
	Cv(data.size)
	{
		bool ins = data[p].inShape;
		float dist = (kdTree(p) - p).length();
		
		float v;
		
		if (ins)
			v = dist/maxDist * .5 + .5;
		else
			v = (1 - dist/maxDist) * .5;
		
		ret.setTexel(p,Color(255,max(min(v,1.f),0.f)*255));
	}
	
	return ret;
}

Image imageToSDF_gauss(const Image &img,int maxDist,Delegate<bool,Color> inF = [](Color c){return c.a > 127;})
{
	Image tmpImg(img.getSize() + vec2(maxDist*2),Color::Black);
	
	Cv(img.getSize())
		tmpImg.setTexel(p + vec2(maxDist),inF(img.getTexel(p)) ? Color::White : Color::Black);
	
	Texture tex(tmpImg);
	/*
	{1/4,1/2,1/4}
	{1/16.0,1/8.0,5/8.0,1/8.0,1/16.0}
	*/
	TextureConvolution conv({.1,1.,1.,1.,1.,1.,1.,1.,.1},nullptr,true);
	
	conv.applyTo(tex,maxDist);
	
	tex.copyToImage(tmpImg);
	
	tmpImg.forEach([](fm::vec2s,Color &c){
		c.a = c.r;
		c.rgb() = vec3(255);
	});
	
	return tmpImg;
}


DrawData buildDrawData(vec2 A,vec2 B,vec2 S)
{
	DrawData ret;
	
	vec2 N = A - B;
	vec2 R = N.sgn().perp() * S.w / S.h * N.length();
	
	vec2 tpt[] = {vec2(0,0),vec2(1,0),vec2(1,1),
				  vec2(0,0),vec2(1,1),vec2(0,1)};
	
	vec2 pts[6];
	vec2 uvs[6];
	
	C(6)
		pts[i] = (tpt[i].x - .5) * R + tpt[i].y * N + B,
		uvs[i] = vec2(1-tpt[i].x,tpt[i].y);
	
	ret.positions = pts;
	ret.texPositions = uvs;
	
	return ret; 
}

class TextTester : public GuiElement
{
	SpriteManager m_spriteMgr;
	std::vector<Sprite> m_sprites;
	fm::String m_sampletext;
public:
	TextTester(GuiContext &owner) : GuiElement(owner,owner.getSize())
	{
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
				cout << source << endl;
			
		});
		if (!res) cout << res << endl;
		
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
	
	void buildSprites()
	{
		// render glyphs
		Font font = getOwnerContext().getDefaultFont();
		font.setCharacterSize(500);
		std::vector<fg::Image *> renderImgs(m_sampletext.size());
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			fg::Image img = font.renderGlyph(m_sampletext[i]);
			renderImgs[i] = new fg::Image(std::move(img));
		}
		
		fm::Delegate<Image,const Image &,int,Delegate<bool,Color> > imgToSDF = imageToSDF_manhattan;
		std::string keyName = "manhattan";
		
		std::vector<fm::Time > times(m_sampletext.size());
		
		// render sdfs
		std::vector<fg::Image *> sdfImgs(m_sampletext.size());
		cout << "rendering " << keyName << endl;
		fm::Clock clkAll,clkOne;
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			fg::Image img = imgToSDF(*renderImgs[i],25,[](Color c){return c.a > 127;});
			sdfImgs[i] = new fg::Image(std::move(img));
			times[i] = clkOne.getTime();
			clkOne.restart();
		}
		fm::Time t = clkAll.getTime();
		for (fm::Size i=0;i<m_sampletext.size();++i)
			cout << i << ": " << times[i].asSecs() << "s\n";
		cout << "all: " << t.asSecs() << "s\n";
		cout << endl;
		
		// upload sdfs
		float maxH = 0;
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			*sdfImgs[i] = std::move(sdfImgs[i]->scale(sdfImgs[i]->getSize()*.1));
			m_spriteMgr.addImage(*sdfImgs[i],fm::toString(i)+"_"+keyName);
			maxH = max<float>(maxH,sdfImgs[i]->getSize().h);
		}
		
		for (fm::Size i=0;i<m_sampletext.size();++i) delete sdfImgs[i];
		for (fm::Size i=0;i<m_sampletext.size();++i) delete renderImgs[i];
		
		vec2 offset(0,maxH*2.5);
		// generate sprites
		float mul = .5;
		font.setCharacterSize(500);
		for (int sizes=0;sizes<20;++sizes)
		{
			for (fm::Size i=0;i<m_sampletext.size();++i)
			{
				m_sprites.emplace_back(m_spriteMgr,fm::toString(i)+"_"+keyName);
				vec2 s = m_sprites.back().getSize() * mul;
				m_sprites.back().setSize(s);
				m_sprites.back().setPosition(offset + font.getGlyph(m_sampletext[i]).leftdown * mul * .1);
				offset.x += s.x+1;
			}
			offset.x = 0;
			offset.y += maxH * mul;
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
	
	fw::MouseCursor cur(img,img.getSize()/2);
	
	PushButton *pb = new PushButton(win,"lol",[&](){
		Mouse::setCursor(cur);
	});
	pb->setPosition(vec2(0,54));
	
	win.getMainLayout().addChildElement(pb);
	
	Image Aimg;
	{
		ifstream in("a.png");
		
		if (!in || true)
		{
			win.getDefaultFont().setCharacterSize(500);
			
			fm::Clock clkR;
			Aimg = win.getDefaultFont().renderGlyph(fm::String("$")[0]);
			cout << "stdtt_t: "<< clkR.getSeconds() << endl;
			
			Aimg.saveToFile("a_ori.png");
			fm::Clock clk;
			Aimg = imageToSDF_kdTree(Aimg,25);
			cout << clk.getSeconds() << endl;
			Aimg.saveToFile("am.png");
			Aimg = Aimg.scale(Aimg.getSize()*.1);
			Aimg.saveToFile("a.png");
		}
		else
		{
			Aimg.loadFromFile("a.png");
		}
	}
	
	Texture Atex(Aimg);
	Atex.setSmooth(true);
	
	DrawData dd = buildDrawData(win.getSize()*vec2(.5,0) + vec2(0,100),win.getSize()*vec2(.5,0),Aimg.getSize());
	
	ShaderManager shader;
	fm::Result res = shader.loadFromFiles("shaders/font1.vert","shaders/font1.frag");
	shader.regTexture("u_tex");
	
	if (!res)
	{
		cout << "Font Shader failed to load" << endl;
		cout << res << endl;
	}
	
	win.getMainLayout().addChildElement(new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		shader.setUniform("u_thickness",sb.getScrollState());
		tester->setThickness(sb.getScrollState());
	}));
	
	ScrollBar *sb = new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		shader.setUniform("u_pov",sb.getScrollState());
		tester->setPov(sb.getScrollState());
	});
	
	sb->setPosition(vec2(0,18)); 
	
	win.getMainLayout().addChildElement(sb);
	
	ScrollBar *sb2 = new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		tester->setDscale(sb.getScrollState());
	});
	
	sb2->setPosition(vec2(0,36)); 
	
	win.getMainLayout().addChildElement(sb2);
	
	bool inMoveState = false;
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
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Right)
				{
					dd = buildDrawData(vec2(ev.mouse),win.getSize()*vec2(.5,0),Aimg.getSize());
					inMoveState = true;
				} 
			}
			if (ev.type == Event::ButtonReleased)
			{
				if (ev.mouse.button == Mouse::Right)
					inMoveState = false;
			}
			if (ev.type == Event::MouseMoved)
			{
				if (inMoveState)
					dd = buildDrawData(vec2(ev.motion),win.getSize()*vec2(.5,0),Aimg.getSize());
			}
		}
		
		win.clear();
		
		shader.getModelStack().top(win.getShader().getModelStack().top());
		shader.getViewStack().top(win.getShader().getViewStack().top());
		shader.getProjStack().top(win.getShader().getProjStack().top());
		shader.bind();
		shader.useTexture(Atex);
		shader.draw(dd);
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}

