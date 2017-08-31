#include <Frontier.hpp>
#include <iostream>

using namespace std;

class CollRock : public Sprite
{
	vec2 m_vel;
public:
	Clock lastCollided;
	
	CollRock(SpriteManager &manager) : Sprite(manager)
	{
		
	}
	
	vec2 getVelocity() const {return m_vel;}
	
	void setVelocity(vec2 vel) {m_vel = vel;}
	
	bool collidedRecently() const
	{
		return lastCollided.getSeconds() < 0.3;
	}
	
	void onCollide()
	{
		if (!collidedRecently())
		{
			lastCollided.restart();
			setImageID("colliding");
		}
	}
};

class CollisionTester
{
public:
	SpriteManager spriteManager;
	deque<CollRock> rocks;
	vec2 winSize;
	float rockSize;
	
	CollisionTester(vec2 winSize) : winSize(winSize), rockSize(6)
	{
		setupImages();
	}
	
	void setupImages()
	{
		Image img;
		img.create(vec2(rockSize*2+4));
		
		vec3 clrs[] = {vec4::White,vec4::Blue,vec4::Red};
		String names[] = {"normal","fast","colliding"};
		
		C(sizeof(clrs)/sizeof(*clrs))
		{
			Cv(img.getSize())
			{
				float d = (p - img.getSize()/2.f).length();
				d = (2-abs(rockSize-d))/2.f;
				d = min(max(d,0.f),1.f);
				img.setTexel(p,vec4(clrs[i],d)*255);
			}
			
			spriteManager.addImage(img,names[i]);
		}
		
		spriteManager.getAtlas().getTexture().setSmooth(true);
	}
	
	void addRock(vec2 p,vec2 v)
	{
		rocks.emplace_back(spriteManager);
		CollRock &rock = rocks.back();
		
		rock.setImageID("normal");
		rock.setPosition(p);
		rock.setVelocity(v);
	}
	
	float bounceWall(float x,float mn,float mx)
	{
		while (x < mn || x > mx)
		{
			if (x < mn) x = 2*mn-x;
			
			if (x > mx) x = 2*mx-x;
		}
		
		return x;
	}
	
	void calcNewPos(vec2 curp,CollRock &rock,vec2 &newp,bool &colliding)
	{
		colliding = false;
		
		vec2 vel = rock.getVelocity();
		vec2 s   = rock.getSize();
		
		if (curp.x < 0 && vel.x < 0) curp.x = -curp.x, vel.x *= -1, colliding = true;
		if (curp.y < 0 && vel.y < 0) curp.y = -curp.y, vel.y *= -1, colliding = true;
		
		if (curp.x+s.w > winSize.w && vel.x > 0) curp.x = winSize.w*2 - s.w*2 - curp.x, vel.x *= -1, colliding = true;
		if (curp.y+s.h > winSize.h && vel.y > 0) curp.y = winSize.h*2 - s.h*2 - curp.y, vel.y *= -1, colliding = true;
		
		rock.setVelocity(vel);
		
		newp = curp;
	}
	
	void updateRocks(fm::Time dt)
	{
		fm::Size rockCount = rocks.size();
		
		for (fm::Size index = 0;index < rockCount;++index)
		{
			CollRock &rock = rocks[index];
			
			vec2 oldp = vec2(rock.getPosition()) + rock.getVelocity() * dt.asSecs();
			vec2 newp;
			bool colliding;
			
			calcNewPos(oldp,rock,newp,colliding);
			
			if (colliding)
				rock.onCollide();
			
			rock.setPosition(newp);
			
			for (fm::Size i=index+1;i<rockCount;++i)
			{
				if (i == index) continue;
				
				CollRock &other = rocks[i];
				
				vec2 d = other.getPosition() - rock.getPosition();
				
				if (d.LENGTH() < rockSize * rockSize * 2 * 2)
				{
					rock.onCollide();
					other.onCollide();
					
					vec2 n = d.perp().sgn();
					
					if (rock.getVelocity().dot(d) > 0)
						rock.setVelocity(rock.getVelocity().reflectOn(n));
					
					if (other.getVelocity().dot(d) < 0)
						other.setVelocity(other.getVelocity().reflectOn(n));
				}
			}
				
			if (!rock.collidedRecently() && rock.getImageID() == "colliding")
				rock.setImageID("normal");
		}
	}
};

int main()
{
	GuiWindow win(vec2(640,480),"bsp");
	
	win.setClearColor(vec4(vec3(0),1));
	
	CollisionTester tester(win.getSize());
	
	Clock addClk;
	fm::Size rockCount = 0;
	
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
				if (ev.key.code == Keyboard::L)
				{
					for (CollRock &rock : tester.rocks)
					{
						cout << rock.getVelocity().length() << "  ";
					}
					cout << endl;
					cout << endl;
					cout << endl;
				}
				if (ev.key.code == Keyboard::Escape)
				{
					running = false;
				}
			}
		}
		
		if (addClk.getSeconds() > 0.3 + rockCount/10.0)
		{
			tester.addRock(win.getSize()/2,pol2(50,rad(rockCount / 10.0)));
			++rockCount;
			addClk.restart();
		}
		
		tester.updateRocks(win.getUpdateInterval());
		
		win.clear();
		
		win.draw(tester.spriteManager);
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}
