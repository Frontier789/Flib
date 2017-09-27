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

Image imageToSDF(const Image &img,int maxDist,Delegate<bool,Color> inF = [](Color c){return c.a > 127;})
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

int main()
{
	GuiWindow win(vec2(640,480),"sdf");
	win.setClearColor(vec4::White);
	
	Image img;
	Result r = img.loadFromFile("bunny.png");
	if (!r) cout << r << endl;
	
	fw::MouseCursor cur(img,img.getSize()/2);
	
	PushButton *pb = new PushButton(win,"lol",[&](){
		Mouse::setCursor(cur);
	});
	pb->setPosition(vec2(0,40));
	
	win.getMainLayout().addChildElement(pb);
	
	Image Aimg;
	{
		ifstream in("a.png");
		
		if (!in || true)
		{
			win.getDefaultFont().setCharacterSize(500);
			
			Aimg = win.getDefaultFont().renderGlyph(fm::String("ß")[0]);
			
			Aimg.saveToFile("a_ori.png");
			fm::Clock clk;
			Aimg = imageToSDF(Aimg,25);
			cout << clk.getSeconds() << endl;
			Aimg.saveToFile("am.png");
			// return 0;
			// return 42;
			/*
			Aimg = imageToSDF(Aimg,30);
			Aimg.saveToFile("a_big.png");
			*/
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
	
	if (!res) cout << res << endl;
	
	win.getMainLayout().addChildElement(new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		shader.setUniform("u_thickness",sb.getScrollState());
	}));
	
	ScrollBar *sb = new ScrollBar(win,vec2(100,18),[&](GuiScrollBar &sb){
		shader.setUniform("u_pov",sb.getScrollState());
	});
	
	sb->setPosition(vec2(0,18));
	
	win.getMainLayout().addChildElement(sb);
	
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

