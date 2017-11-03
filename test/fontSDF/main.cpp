#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
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

class TextTester : public GuiElement
{
	SpriteManager m_spriteMgr;
	std::vector<Sprite> m_sprites;
	fm::String m_sampletext;
public:
	TextTester(GuiContext &owner) : GuiElement(owner,owner.getSize())
	{
		m_sampletext = u8"árvíztűrő tükörfúrógép :)";
		
		buildSprites(Manhattan);
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
	
	enum SDFMethod {
		Gauss     = 0,
		KDTree    = 1,
		Brute     = 2,
		Manhattan = 3,
		MethodCount
	};
	
	string methodToString(SDFMethod method)
	{
		if (method == Gauss)  return "Gauss";
		if (method == KDTree) return "KDTree";
		if (method == Brute)  return "Brute";
		if (method == Manhattan) return "Manhattan";
		
		return "";
	}
	
	void buildSprites(SDFMethod method)
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
		
		// render glyphs
		Font font = getOwnerContext().getDefaultFont();
		font.setCharacterSize(200);
		std::vector<fg::Image *> renderImgs(m_sampletext.size());
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			fg::Image img = font.renderGlyph(m_sampletext[i]);
			renderImgs[i] = new fg::Image(std::move(img));
		}
		
		fm::Delegate<Image,const Image &,int,Delegate<bool,Color> > imgToSDF;
		std::string keyName = methodToString(method);
		
		if (method == Gauss     ) imgToSDF = imageToSDF_gauss    ;
		if (method == KDTree    ) imgToSDF = imageToSDF_kdTree   ;
		if (method == Brute     ) imgToSDF = imageToSDF_brute    ;
		if (method == Manhattan ) imgToSDF = imageToSDF_manhattan;
		
		std::vector<fm::Time > times(m_sampletext.size());
		
		// render sdfs
		float maxH = 0;
		std::vector<fg::Image *> sdfImgs(m_sampletext.size());
		std::vector<vec2> sdfSizes(m_sampletext.size());
		cout << "rendering " << keyName << endl;
		fm::Clock clkAll,clkOne;
		int maxDist = 10;
		for (fm::Size i=0;i<m_sampletext.size();++i)
		{
			if (method != Gauss)
			{
				fg::Image img = imgToSDF(*renderImgs[i],10,[](Color c){return c.a > 127;});
				maxH = max<float>(maxH,img.getSize().h*.1);
				
				sdfImgs[i] = new fg::Image(std::move(img));
				*sdfImgs[i] = std::move(sdfImgs[i]->scale(sdfImgs[i]->getSize()*.1));
				m_spriteMgr.addImage(*sdfImgs[i],fm::toString(i)+"_"+keyName);
				sdfSizes[i] = sdfImgs[i]->getSize();
			}
			else
			{
				/*
				Delegate<bool,Color> inF = [](Color c){return c.a > 127;};
				
				Image tmpImg(renderImgs[i]->getSize() + vec2(maxDist*2),Color(255,255,255,0));
	
				Cv(renderImgs[i]->getSize())
					tmpImg.setTexel(p + vec2(maxDist),inF(renderImgs[i]->getTexel(p)) ? Color(255,255,255,255) : Color(255,255,255,0));
				
				Texture tex(tmpImg);
				*/
				
				Texture tex;
				tex.create(renderImgs[i]->getSize() + vec2(maxDist*2));
				FrameBuffer fbo(tex);
				fbo.bind();
				fbo.setClearColor(vec4(1,1,1,0));
				fbo.clear();
				tex.update(*renderImgs[i],vec2(maxDist));
				
				/*
				{1/4,1/2,1/4}
				{1/16.0,1/8.0,5/8.0,1/8.0,1/16.0}
				*/
				TextureConvolution conv({1,1,1,1,1,1,1,1,1,1,1},nullptr,true);
				
				conv.applyTo(tex,maxDist);
				
				maxH = max<float>(maxH,tex.getSize().h*.1);
				
				m_spriteMgr.addImage(tex,fm::toString(i)+"_"+keyName);
				sdfSizes[i] = tex.getSize()*.1;
			}
			
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
	
	TextTester::SDFMethod method = TextTester::Manhattan;
	
	pb = new PushButton(win,tester->methodToString(method),[&](){
		method = TextTester::SDFMethod((((int)method)+1)%TextTester::MethodCount);
		tester->buildSprites(method);
		pb->setText(tester->methodToString(method));
		
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

