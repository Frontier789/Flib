#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "HexArray.hpp"
#include "HexPlotter.hpp"
#include "HexTerrain.hpp"

template<class T>
class Interpoler
{
	fm::Time  m_duration;
	fm::Clock m_clock;
	T m_start;
	T m_end;
public:
	
	Interpoler();
	
	Interpoler(T begin,T end,fm::Time duration = fm::seconds(1));
	
	T get() const;
	fm::Time getTimeLeft() const;
	
	bool finished() const;
	
	Interpoler<T> &set(T begin,T end,fm::Time duration = fm::seconds(1));
	Interpoler<T> &retarget(T newEnd,fm::Time addDuration = fm::seconds(1),bool keepTimeLeft = true);
	Interpoler<T> &restart();
};

template<class T>
inline Interpoler<T>::Interpoler()
{
	
}

template<class T>
inline Interpoler<T>::Interpoler(T begin,T end,fm::Time duration) : m_duration(duration), m_start(begin), m_end(end)
{
	
}

template<class T>
inline T Interpoler<T>::get() const
{
	double r = m_clock.getTime() / m_duration;
	
	if (r > 1.0) return m_end;
	
	r = (3 - 2*r)*r*r;
	
	return (1.0 - r)*m_start + r*m_end;
}

template<class T>
inline fm::Time Interpoler<T>::getTimeLeft() const
{
	fm::Time t = m_clock.getTime();
	
	if (t > m_duration) return fm::seconds(0);
	
	return m_duration - t;
}

template<class T>
inline bool Interpoler<T>::finished() const
{
	return m_clock.getTime() >= m_duration;
}

template<class T>
inline Interpoler<T> &Interpoler<T>::set(T begin,T end,fm::Time duration)
{
	m_duration = duration;
	m_start    = begin;
	m_end      = end;
	
	m_clock.restart();
	
	return *this;
}

template<class T>
inline Interpoler<T> &Interpoler<T>::retarget(T newEnd,fm::Time addDuration,bool keepTimeLeft)
{
	m_duration = keepTimeLeft ? (getTimeLeft() + addDuration) : addDuration;
	m_start    = get();
	m_end      = newEnd;
	
	m_clock.restart();
	
	return *this;
}

template<class T>
inline Interpoler<T> &Interpoler<T>::restart()
{
	m_clock.restart();
	
	return *this;
}


void printControls()
{
	cout << "Controls:" << endl
		 << "\tR: regenerate map" << endl
		 << "\tS: save screenshot" << endl
		 << "\t+: increase deatil level" << endl
		 << "\t-: reduce detail level" << endl
		 << "\tl: toggle light" << endl
		 << "\tg: toggle grid" << endl
		 << "\tMouse: press left and move to look around" << endl
		 << "\tWheel: zoom in and out" << endl << endl;
}

int main()
{
	Window win(vec2(640,480),"Hexy",Window::Default /*&~ Window::Resize &~ Window::Maximize*/);
	
	printControls();
	
	srand(time(0));
	
	bool running = true;
	Clock fpsClock;
	
	FixedShaderManager fixedShader;
	ShaderManager shader110;
	ShaderManager shader150;
	
	int usedShader = 0;
	bool useGeomShader = false;
	bool doLight = true;
	bool doGrid  = false;
	
	if (Shader::isAvailable())
	{
		Result res150 = shader150.loadFromFiles("shaders/vert150.glsl","shaders/geom150.glsl","shaders/frag150.glsl");
		if (res150)
		{
			cout << "using shader ver 150" << endl;
			shader150.associate("in_pos","in_color","","in_normal");
			shader150.setMatrices("u_modelMat");
			shader150.setCamera(nullptr,"u_projMat","u_viewMat");
			
			usedShader = 2;
			useGeomShader = true;
		}
		else
		{
			cout << "load of shader ver 150 failed result: " << res150.toString() << endl;
		
			Result res110 = shader110.loadFromFiles("shaders/vert110.glsl","shaders/frag110.glsl");
			if (res110)
			{
				cout << "using shader ver 110" << endl;
				shader110.associate("in_pos","in_color","in_texpos","in_normal");
				shader110.setMatrices("u_modelMat");
				shader110.setCamera(nullptr,"u_projMat","u_viewMat");
				
				usedShader = 1;
			}
			else
			{
				cout << "load of shader ver 110 failed result: " << res110.toString() << endl;
				cout << "falling back to fixed pipeline" << endl;
				
				usedShader = 0;
			}
		}
	}
	else
	{
		cout << "shaders not supported, using fixed pipeline" << endl;
		
		usedShader = 0;
	}
	
	ShaderManager &shader = ((usedShader == 0) ? fixedShader : (usedShader == 1 ? shader110 : shader150));
	
	Camera cam;
	shader.changeCamera(cam);
	shader.setUniform("u_doLight",1);
	shader.setUniform("u_doGrid",0);
	cam.getProjStack().push(MATRIX::ortho(win.getSize().w*-0.5,win.getSize().h*-0.5,win.getSize().w*0.5,win.getSize().h*0.5));
	
	HexTerrain hex(1,useGeomShader);
	hex.gen();
	
	vec2  viewPos;
	float viewZoom = 50;
	bool  leftDown  = false;
	vec2  lastMouse;
	
	Interpoler<float> zoomInterpoler(50,50,seconds(0));
	Interpoler<vec2>  posInterpoler (vec2(),vec2(),seconds(0));
	
	Clock loopClk;
	Clock uClk;
	
	for (int loop=0;running;++loop)
	{
		if (loopClk.getSeconds() > 1)
		{
			loopClk.restart();
			
			win.setTitle("Hexy  " + fm::toString(loop/1) + "fps");
			
			loop = 0;
		}
		
		Event ev;
		while (win.popEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				running = false;
			}
			
			if (ev.type == Event::Resized)
			{
				cam.getProjStack().push(MATRIX::ortho(win.getSize().w*-0.5,win.getSize().h*-0.5,win.getSize().w*0.5,win.getSize().h*0.5));
				FrameBuffer::setViewport(rect2s(vec2(),win.getSize()));
			}
			
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::R)
				{
					hex.gen();
				}
				
				if (ev.key.code == Keyboard::S)
				{
					win.capture().saveToFile("hex.png");
				}
				
				if (ev.key.code == Keyboard::G)
				{
					doGrid = !doGrid;
					shader.setUniform("u_doGrid",doGrid ? 1 : 0);
					
				}
				
				if (ev.key.code == Keyboard::L)
				{
					doLight = !doLight;
					shader.setUniform("u_doLight",doLight ? 1 : 0);
				}
				
				if (ev.key.code == Keyboard::Plus)
				{
					hex.extendToTwice();
					
					viewZoom /= 1.8;
					zoomInterpoler.retarget(viewZoom,seconds(0.2),false);
				}

				if (ev.key.code == Keyboard::Minus)
				{
					hex.reduceToHalf();
					
					viewZoom *= 1.8;
					zoomInterpoler.retarget(viewZoom,seconds(0.2),false);
				}

				if ((ev.key.code >= Keyboard::Num0    && ev.key.code <= Keyboard::Num9) ||
					(ev.key.code >= Keyboard::Numpad0 && ev.key.code <= Keyboard::Numpad9))
				{
					int n;
					if (ev.key.code >= Keyboard::Num0 && ev.key.code <= Keyboard::Num9) n = ev.key.code - Keyboard::Num0;
					else n = ev.key.code - Keyboard::Numpad0;

					hex.setColorStyle(n);
				}
			}
			
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					leftDown  = true;
					lastMouse = Mouse::getPosition(win);
				}
			}
			
			if (ev.type == Event::ButtonReleased)
			{
				if (ev.mouse.button == Mouse::Left)
				{
					leftDown = false;
				}
			}
			
			if (ev.type == Event::MouseMoved)
			{
				if (leftDown)
				{
					vec2 p = vec2i(ev.motion);
					
					viewPos += (p-lastMouse)*vec2(1,-1);
					
					posInterpoler.retarget(viewPos,seconds(0.0),false);
					
					lastMouse = p;
				}
			}
			
			if (ev.type == Event::MouseWheelMoved)
			{
				float s = pow(1.2,ev.wheel.delta);
				
				vec2 p = ((Mouse::getPosition(win)-win.getSize()/2.0)*vec2(1,-1));
				
				vec2 delta = (1-s)*(p-viewPos);

				viewPos += delta;
				
				viewZoom *= s;
				
				zoomInterpoler.retarget(viewZoom,seconds(0.1),false);
				posInterpoler.retarget(viewPos,seconds(0.1),false);
			}
		}
		
		shader.setUniform("u_time",(float)uClk.getSeconds());
		
		win.clear();
		shader.getModelStack().push().mul(MATRIX::translation(posInterpoler.get())*MATRIX::scaling(vec2(zoomInterpoler.get())));
		hex.onDraw(shader);
		shader.getModelStack().pop();
		win.swapBuffers();
		
		Sleep(1.0 / 60.0 - fpsClock.restart());
	}
}
