#include <Frontier.hpp>
#include <iostream>
#include <fstream>

using namespace std;


class Particle : fg::Drawable
{
	vec2 m_vel;
	vec2 m_acc;
	Anglef m_angVel;
	Anglef m_angAcc;
	Time m_lifeTime;
	Time m_originalLife;
	FontSprite *m_sprite;
	vec3 m_baseColor;
	
public:
	static Glyph::Style particleStyle;
	
	Particle(vec2 pos,vec2 vel,vec2 acc,Anglef angVel,Anglef angAcc,Time lifeTime,FontSprite &&spr,vec3 baseColor) : 
			 m_vel(vel),
			 m_acc(acc),
			 m_angVel(angVel),
			 m_angAcc(angAcc),
			 m_lifeTime(lifeTime),
			 m_originalLife(lifeTime),
			 m_sprite(new FontSprite(std::move(spr))),
			 m_baseColor(baseColor)
	{
		m_sprite->setPosition(pos - m_sprite->getSize() / 2);
		m_sprite->setDirection(vel.sgn());
	}
	
	~Particle() {delete m_sprite;}
	
	/////////////////////////////////////////////////////////////
	void onDraw(ShaderManager &/* shader */) override {}
	
	/////////////////////////////////////////////////////////////
	void applyGravity(vec2 o,float w,const fm::Time &dt)
	{
		if (!m_sprite) return;
		
		vec2 p = m_sprite->getPosition() + m_sprite->getSize()/2;
		
		m_vel += (o-p).sgn() * w * dt.asSecs();
	}
	
	/////////////////////////////////////////////////////////////
	void onUpdate(const fm::Time &dt) override 
	{
		if (!m_sprite) return;
		
		m_vel += m_acc * dt.asSecs();
		m_angVel += m_angAcc * dt.asSecs();
		m_lifeTime -= dt.asSecs();
		
		pol2 polVel = m_vel;
		polVel.angle += m_angVel * dt.asSecs();
		m_vel = polVel;
		
		vec2 p = m_sprite->getPosition() + m_sprite->getSize()/2;
		
		p += m_vel * dt.asSecs();
		
		m_sprite->setPosition(p - m_sprite->getSize()/2);
		m_sprite->setDirection(-m_vel.sgn());
		
		float r = m_lifeTime / m_originalLife;
		m_sprite->setColor(vec4(m_baseColor - vec3(sin(r*3.14159265358979)*.3,0,0),sin(3.14159265358979*sqrt(1-r))));
		
		if (m_lifeTime.asSecs() < 0)
		{
			delete m_sprite;
			m_sprite = nullptr;
		}
	}
};

Glyph::Style Particle::particleStyle = Glyph::SigDistField;

int main()
{
	GuiWindow win(vec2(640,480),"sdf");
	win.setClearColor(vec4(.5,1));
	
	Font f = win.getDefaultFont();
	
	vector<Particle*> particles;
	
	f.setCharacterSize(13);
	auto &sprMgr = f.getSpriteManager(Particle::particleStyle);
	
	
	if (!sprMgr.getShaderLoadResult())
	{
		cout << "failed load, code:" << endl;
		
		f.loadSigDistFieldShader([](fg::ShaderSource &s){
			cout << s << endl;
		});
		
		cout << sprMgr.getShaderLoadResult() << endl;
		return 1;
	}
	
	GuiText *capText = new GuiText(win,"cap: \ncount:");
	win.getMainLayout().addChildElement(capText);
	
	vec2 lastMouseP;
	bool gravity = false;
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
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::D)
				{
					gravity = true;
					lastMouseP = Mouse::getPosition();
				}
			}
			if (ev.type == Event::MouseMoved)
			{
				lastMouseP = vec2(ev.motion) + win.getPosition();
			}
			if (ev.type == Event::KeyReleased)
			{
				if (ev.key.code == Keyboard::D)
				{
					gravity = false;
				}
			}
			if (ev.type == Event::ButtonPressed)
			{
				if (ev.mouse.button == Mouse::Left) 
				{
					int db = 10 + rand()%10;
					if (Keyboard::isKeyHeld(Keyboard::LShift))
						db *= 100;
					
					f.setCharacterSize(13);
					
					String s = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ_FFFFFFFF";
					
					String letters(db,' ');
					
					for (auto &c : letters)
						c = s[rand()%s.size()];
					
					std::vector<FontSprite> sprs = f.getSprites(letters,Particle::particleStyle,db);
					
					C(db)
					{
						particles.push_back(new Particle(vec2(ev.mouse) + win.getPosition(),
														 pol2(rand()%300+200,deg(i/float(db)*360)),
														 vec2(rand()%11-5,rand()%21-10),
														 deg(5 + rand()%180),
														 deg(rand()%50),
														 seconds((rand()%500)/100.0+2),
														 std::move(sprs[i]),
														 vec3(1)));
					}
				}
			}
		}
		
		if (gravity)
		{
			for (Particle *particle : particles)
				particle->applyGravity(lastMouseP,500,win.getUpdateInterval() * (min<int>(sprMgr.getSpriteCount(),2100) / 1000.0 + 0.1));
		}
		
		capText->setText("cap: " + fm::toString(sprMgr.getCapacity()) + "\ncount: " + fm::toString(sprMgr.getSpriteCount()) + "\nparticles: " + fm::toString(particles.size()));
		
		for (auto *ptr : particles)
			ptr->onUpdate(win.getUpdateInterval() * (min<int>(sprMgr.getSpriteCount(),2100) / 1000.0 + 0.1));
		
		win.clear();
		
		win.getShader().getModelStack().push().mul(MATRIX::translation(-vec2(win.getPosition())));
		f.forEachSpriteManager([&](FontSpriteManager &mgr){
			win.draw(mgr);
		});
		win.getShader().getModelStack().pop();
		
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
	
	for (auto *ptr : particles)
		delete ptr;
}

