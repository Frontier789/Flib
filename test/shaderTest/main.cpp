#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <fstream>

using namespace std;

/*
 *    BSP -> KDTRee
 *  Glob. Ill.
 *  -save approximation when camera is still
 *  -keep track of pos on texture when recursing
.*  Sprite -> Text
 *  Fourier test
 *    tree upgrade
.*  SDF font/text
 *    custom pic cursor
 *  bring unique sprite back
 *  rework old tests
 *  EditText
 *    FIX CAMERA
 *    glowing tetrarot
 *    convolution applier
 *    fm::toString(float)
 *  pump up Transition<T>
.*  image to SDF converter
 *    fproject: use $FPATH
 *  make install target
 */

class Frame : public fg::Drawable
{
	fg::ShaderManager &m_shader;
	fg::DrawData m_rectDraw;
	fm::Clock m_fadeInClk;
	fm::Size m_shaderId;
	bool m_leftFadeIn;
public:
	
	Frame(fm::Size shaderId,fg::ShaderManager &shader,bool leftFadeIn,bool fade,GridLayout *layout);
	
	void  setupRectDraw();
	float getFadeInAmount();
	bool  getLeftFadeIn();
	fm::Size getShaderId();
	void buildLayout(GridLayout *layout);
	
	void onUpdate(const fm::Time &/* dt */) override {};
	void onDraw(fg::ShaderManager &/* shader */) override;
};
	
Frame::Frame(fm::Size shaderId,
			 fg::ShaderManager &shader,
			 bool leftFadeIn,
			 bool fade,
			 GridLayout *layout) : m_shader(shader),
								   m_shaderId(shaderId),
								   m_leftFadeIn(leftFadeIn)
{
	if (!fade)
		m_fadeInClk.setTime(seconds(60));
	
	buildLayout(layout);
}

void Frame::setupRectDraw()
{
	float fadeAm = getFadeInAmount();
	if (m_leftFadeIn)
	{
		m_rectDraw.positions = {fm::vec2(1-fadeAm,0),fm::vec2(1,0),fm::vec2(1,1),fm::vec2(1-fadeAm,1)};
	}
	else
	{
		m_rectDraw.positions = {fm::vec2(0,0),fm::vec2(fadeAm,0),fm::vec2(fadeAm,1),fm::vec2(0,1)};
	}
	
	m_rectDraw.clearDraws();
	m_rectDraw.addDraw(0,4,fg::TriangleFan);
}

float Frame::getFadeInAmount()
{
	float secs = m_fadeInClk.getSeconds();
	
	const float transitionTime = .3;
	
	float val = 1;
	if (secs < transitionTime)
		val = secs/transitionTime;
	
	return (3 - 2*val)*val*val;
}

bool Frame::getLeftFadeIn()
{
	return m_leftFadeIn;
}

fm::Size Frame::getShaderId()
{
	return m_shaderId;
}

void Frame::buildLayout(GridLayout *layout)
{
	std::vector<std::string> uniforms;
	
	m_shader.forAllUniforms([&](std::string name,Shader::UniformData data){
		if (name.find("_userdef") != string::npos)
		{
			uniforms.push_back(name);
		}
	});
	
	std::map<std::string,float> uni_vals;
	
	C(layout->getCellCount().h)
	{
		GuiText *nameText  = (GuiText*)layout->getChildElement2D(vec2(0,i));
		ScrollBar *scrollb = (ScrollBar*)layout->getChildElement2D(vec2(1,i));
		uni_vals[nameText->getText().str()] = scrollb->getScrollState();
	}
	
	layout->setCellCount(vec2(3,0));
	
	C(uniforms.size())
	{
		GuiText *nameText = new GuiText(layout->getOwnerContext(),uniforms[i]);
		nameText->setColor(vec4::White);
		
		layout->setChildElement2D(vec2(0,i),nameText);
		
		GuiText *valText = new GuiText(layout->getOwnerContext(),"0%");
		valText->setColor(vec4::White);
		
		ScrollBar *scrollBar = new ScrollBar(layout->getOwnerContext(),vec2(70,16),[=](GuiScrollBar &sb) {
			valText->setText(fm::toString( int(sb.getScrollState()*100) ) + "%");
			
			m_shader.setUniform(uniforms[i],sb.getScrollState());
		});
		
		scrollBar->setScrollState(uni_vals[uniforms[i]]);
		
		layout->setChildElement2D(vec2(2,i),valText);
		layout->setChildElement2D(vec2(1,i),scrollBar);
	}
	
}

void Frame::onDraw(fg::ShaderManager &/* shader */)
{
	setupRectDraw();
	m_shader.draw(m_rectDraw);
}

class FrameStack : public fg::Drawable
{
	std::vector<fg::ShaderManager*> m_curManagers;
	std::deque<Frame*> m_frames;
	GridLayout *m_layout;
	fm::Size m_curFrame;
	fm::vec2 m_winSize;
	fm::Clock m_clk;
public:
	FrameStack(fm::vec2 winSize,GridLayout *layout);
	
	~FrameStack();
	
	void collectShaders(fm::String folder = "shaders");
	void deleteManagers();
	void resetClock();
	fm::Result loadShader(const std::string &vertFile,const std::string &fragFile);
	
	void addFrame(fm::Size shaderId,bool leftFade,bool fade = true);
	void setWinSize(fm::vec2 winSize);
	void nextFrame(bool right);
	
	void onUpdate(const fm::Time &/* dt */) override {};
	void onDraw(fg::ShaderManager &shader) override;
	
	ShaderManager &getTopManager();
};

FrameStack::FrameStack(fm::vec2 winSize,
					   GridLayout *layout) : m_layout(layout),
											 m_curFrame(0),
											 m_winSize(winSize)
{
	
}

FrameStack::~FrameStack()
{
	deleteManagers();
}

void FrameStack::collectShaders(fm::String folder)
{
	fm::Size activeFrame = 0;
	fm::Camera cam;
	cam.set3D(m_winSize,vec3());
	cam.setPitch(deg(80));
	cam.setYaw(deg(10));
	
	if (m_frames.size())
	{
		activeFrame = m_frames.back()->getShaderId();
		cam = m_curManagers[activeFrame]->getCamera();
	}
	
	deleteManagers();
	
	bool found = true;
	for (fm::Size i=0;found;++i)
	{
		fm::String vertFile = folder + "/vert_" + fm::toString(i) + ".glsl";
		fm::String fragFile = folder + "/frag_" + fm::toString(i) + ".glsl";
		ifstream in_vert(vertFile.str().c_str());
		ifstream in_frag(fragFile.str().c_str());
		
		if (in_vert && in_frag)
		{
			fm::Result res = loadShader(vertFile.str(),fragFile.str());
			if (!res)
				cout << "\tError loading shader nr. " << i << ", shader not added to stack: " << res << endl;
		}
		else
			found = false;
	}
	
	if (activeFrame < m_curManagers.size())
	{
		addFrame(activeFrame,true,false);
		m_curManagers[activeFrame]->getCamera() = cam;
	}
}

fm::Result FrameStack::loadShader(const std::string &vertFile,const std::string &fragFile)
{
	m_curManagers.push_back(new ShaderManager);
	
	fm::Result res = m_curManagers.back()->loadFromFiles(vertFile,fragFile);
	
	if (!res)
	{
		delete m_curManagers.back();
		
		m_curManagers.pop_back();
	}
	else
	{
		m_curManagers.back()->setUniform("u_winSize",fm::vec2(m_winSize));
		m_curManagers.back()->getClock().setTime(m_clk.getTime());
		m_curManagers.back()->getCamera().set3D(m_winSize,vec3());
	}
	
	return res;
}

void FrameStack::deleteManagers()
{
	for (auto ptr : m_curManagers) delete ptr;
	m_curManagers.clear();
	
	for (auto ptr : m_frames) delete ptr;
	m_frames.clear();
}

void FrameStack::resetClock()
{
	m_clk.setTime(seconds(0));
	
	for (auto ptr : m_curManagers)
		ptr->getClock().setTime(seconds(0));
}

void FrameStack::onDraw(fg::ShaderManager &shader)
{
	for (int i=m_frames.size();i>0;--i)
	{
		int index = i-1;
		m_frames[index]->onDraw(shader);
		
		if (m_frames[index]->getFadeInAmount() >= 1.f)
		{
			for (int j=0;j<index;++j) m_frames.pop_front();
			break;
		}
	}
	
	for (auto framePtr : m_frames)
		framePtr->onDraw(shader);
}

ShaderManager &FrameStack::getTopManager()
{
	return *m_curManagers[m_frames.back()->getShaderId()];
}

void FrameStack::addFrame(fm::Size shaderId,bool leftFade,bool fade)
{
	m_frames.push_back(new Frame(shaderId,*m_curManagers[shaderId],leftFade,fade,m_layout));
}

void FrameStack::setWinSize(fm::vec2 winSize)
{
	m_winSize = winSize;
	
	for (auto ptr : m_curManagers) 
		ptr->setUniform("u_winSize",fm::vec2(m_winSize));
}
	
void FrameStack::nextFrame(bool right)
{
	if (right)
		m_curFrame = (m_curFrame+1) % m_curManagers.size();
	else
		m_curFrame = (m_curFrame+m_curManagers.size()-1) % m_curManagers.size();
	
	addFrame(m_curFrame,!right);
}

int main()
{
	GuiWindow win(vec2(640,480),"shady");
	cout<<glGetString(GL_VERSION)<<endl;
	
	win.enableKeyRepeat();
	
	GridLayout *layout = new GridLayout(win);
	win.getMainLayout().addChildElement(layout);
	
	FrameStack frameStack(win.getSize(),layout);
	frameStack.collectShaders();
	
	bool btnPressed[6];
	C(sizeof(btnPressed)/sizeof(*btnPressed)) btnPressed[i] = false;
	bool rightPressed = false;
	fm::vec2 lastMousP;
	
	Clock fpsDispClk;
	bool running = true;
	for (fm::Size frameCounter=0;running;++frameCounter)
	{
		if (fpsDispClk.getSeconds() > 1)
		{
			fpsDispClk.setTime(fpsDispClk.getSeconds() - 1);
			win.setTitle("shady " + fm::toString(frameCounter));
			frameCounter = 0;
		}
		
		
		fm::vec3 delta(btnPressed[3] - btnPressed[1],
					   btnPressed[5] - btnPressed[4],
					   btnPressed[0] - btnPressed[2]);
		
		Camera &cam = frameStack.getTopManager().getCamera();
		
		cam.movePosition((delta.x * -cam.r() + delta.y * vec3(0,1,0) + delta.z * cam.f()) * 0.1);
	
		
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::Resized)
			{
				cout << "window resized to " << vec2i(ev.size) << endl;
				frameStack.setWinSize(ev.size);
			}
			if (ev.type == Event::FocusGained)
			{
				frameStack.collectShaders();
			}
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::R)
				{
					frameStack.collectShaders();
				}
				if (ev.key.code == Keyboard::T)
				{
					frameStack.resetClock();
				}
				if (ev.key.code == Keyboard::L)
				{
					cout << "sprite count: " << win.getSpriteManager().getSpriteCount() << endl;
					
					auto &manager = win.getSpriteManager();
					
					C(manager.getSpriteCount())
					{
						auto &sprite = manager.getSpriteById(i);
						
						cout << i << ": " << sprite.getPosition() << " , " << sprite.getSize() << " , " << sprite.getDirection() << endl;
					}
					cout << "instancing: " << (manager.useInstancing() ? "on" : "off") << endl;
					cout << "model:" << endl;
					cout << win.getShader().getModelStack().top()<< endl;
					cout << "view:" << endl;
					cout << win.getShader().getViewStack().top() << endl;
					cout << "proj:" << endl;
					cout << win.getShader().getProjStack().top() << endl;
					
					cout << endl;
					
					cout << "campos: " << cam.getPosition() << endl;
					cout << "camdir: " << cam.getViewDir() << endl;
					cout << "3d:     " << (cam.getProjection()==Camera::Perspective) << endl;
				}
				if (ev.key.code == Keyboard::Left)
				{
					frameStack.nextFrame(true);
				}
				if (ev.key.code == Keyboard::Right)
				{
					frameStack.nextFrame(false);
				}
				
				if (ev.key.code == Keyboard::W) btnPressed[0] = true;
				if (ev.key.code == Keyboard::A) btnPressed[1] = true;
				if (ev.key.code == Keyboard::S) btnPressed[2] = true;
				if (ev.key.code == Keyboard::D) btnPressed[3] = true;
				if (ev.key.code == Keyboard::LShift) btnPressed[4] = true;
				if (ev.key.code == Keyboard::Space)  btnPressed[5] = true;
			}
			
			if (ev.type == Event::KeyReleased)
			{
				if (ev.key.code == Keyboard::W) btnPressed[0] = false;
				if (ev.key.code == Keyboard::A) btnPressed[1] = false;
				if (ev.key.code == Keyboard::S) btnPressed[2] = false;
				if (ev.key.code == Keyboard::D) btnPressed[3] = false;
				if (ev.key.code == Keyboard::LShift) btnPressed[4] = false;
				if (ev.key.code == Keyboard::Space)  btnPressed[5] = false;
			}
			
			if (ev.type == Event::MouseMoved)
			{
				if (rightPressed)
				{
					vec2 delta = vec2(ev.motion) - win.getSize()/2;
					
					if (delta.LENGTH() > 2)
					{
						cam.addPitch(delta.y * -0.01);
						cam.addYaw(delta.x * -0.01);
						
						Mouse::setPosition(win.getSize() / 2,win);
					}
				}
			}
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Right)
				{
					rightPressed = true;
					win.showCursor(false);
					Mouse::setPosition(win.getSize() / 2,win);
					
					lastMousP = ev.mouse;
				}
			}
			
			if (ev.type == Event::ButtonReleased)
			{
				if (ev.mouse.button == Mouse::Right)
				{
					rightPressed = false;
					
					Mouse::setPosition(lastMousP,win);
					win.showCursor(true);
				}
			}
		}
		
		win.clear();
		
		frameStack.onDraw(win.getShader());
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}

