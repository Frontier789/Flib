#include <Frontier.hpp>
#include <iostream>
#include <fstream>

using namespace std;

#include "SpriteClasses.hpp"


/*
 - disable/enable only incorrectly set vertex attrib array settings
 
 
*/

class SpriteTester : public Drawable
{
	std::vector<test::Sprite> m_sprites;
	test::SpriteManager *m_manager;
	fm::Size m_usedSprites;
	bool m_recreateSprites;
	float m_moveSprites;
	fm::Time m_elapsed;
	Image m_snowImg;
	vec2u m_winSize;
	
public:
	/////////////////////////////////////////////////////////////
	SpriteTester(test::SpriteManager *newManager,fm::vec2 winSize) : m_manager(nullptr),
																	 m_usedSprites(10),
																	 m_recreateSprites(false),
																	 m_moveSprites(0),
																	 m_winSize(winSize)
	{
		Result res = m_snowImg.loadFromFile("sheet.png");
		
		if (!res) cout << res << endl;
		
		setManager(newManager);
	}
	
	/////////////////////////////////////////////////////////////
	void setMovingSprites(float moving)
	{
		m_moveSprites = moving;
	}
	
	/////////////////////////////////////////////////////////////
	void removeSpriteAt(fm::Size index)
	{
		if (index+1 != m_sprites.size())
		{
			m_sprites[index].swap(m_sprites.back());
		}
		
		m_sprites.pop_back();
	}
	
	/////////////////////////////////////////////////////////////
	~SpriteTester()
	{
		delete m_manager;
	}
	
	/////////////////////////////////////////////////////////////
	void setManager(test::SpriteManager *newManager)
	{
		delete m_manager;
		m_manager = newManager;
		m_sprites = std::vector<test::Sprite>();
		
		C(6)
			m_manager->addImage(m_snowImg.getSubImage(vec2(i*100,0),vec2(100,100)),i);
	}

	/////////////////////////////////////////////////////////////
	void onDraw(ShaderManager &shader)
	{
		m_manager->onDraw(shader);
	}
	
	/////////////////////////////////////////////////////////////
	void setSpriteCount(fm::Size num)
	{
		m_usedSprites = num;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size getSpriteCoount() const
	{
		return m_sprites.size();
	}
	
	/////////////////////////////////////////////////////////////
	void randSizePos(vec2 &s,vec2 &p) 
	{	
		s = vec2(5 + rand()%50);
		p = vec2(rand()%int(m_winSize.w - s.w),rand()%int(m_winSize.h - s.h));
	};
	
	/////////////////////////////////////////////////////////////
	void addSprite()
	{
		vec2 s,p;
		randSizePos(s,p);
		
		test::Sprite spr = m_manager->getSprite(rand()%6);
		spr.setPosition(p).setSize(s);
		m_sprites.emplace_back(spr);
	}
	
	/////////////////////////////////////////////////////////////
	void onUpdate(const fm::Time &dt)
	{
		m_elapsed += dt;
		
		float dif = m_usedSprites - float(m_sprites.size());
		
		while (m_sprites.size() < m_usedSprites && m_elapsed.asSecs() > 1 / dif)
		{
			m_elapsed -= seconds(1 / dif);
			
			addSprite();
		}
		
		for (fm::Size index=0;index<m_sprites.size()*m_moveSprites;++index)
		{
			test::Sprite &spr = m_sprites[index];
			vec2 siz = spr.getSize();
			
			if (siz.area() < 10)
			{
				if (m_sprites.size() > m_usedSprites)
				{
					removeSpriteAt(index);
					--index;
					continue;
				}
				else
				{
					vec2 s,p;
					randSizePos(s,p);
					
					spr.setPosition(p).setSize(s);						
				}
			}
			else
			{
				spr.setSize(siz * 0.992);
				
				vec2 p = spr.getPosition();
				vec2 toOrigo = m_winSize/2 - p;
				vec2 perp = toOrigo.perp().sgn();
				
				spr.setPosition(spr.getPosition() + toOrigo / toOrigo.LENGTH() * 8 + perp * 4);					
			}
		}
	}
};

#include <FRONTIER/OpenGL.hpp>

int main()
{
	srand(time(0));
	
	GuiWindow win(vec2(640,480),"sprite");
	
	cout << glGetString(GL_VERSION) << endl;
	cout << glGetString(GL_VENDOR) << endl;
	
	SpriteTester tester(new test::PerOneSpriteManager,win.getSize());
	
	GuiText *scText = new GuiText(win,"10");
	scText->setPosition(vec2(100,0));
	
	GuiText *spText = new GuiText(win,"0");
	spText->setPosition(vec2(100 + scText->getSize().w*3,0));
	
	fm::Size spriteCount = 10;
	
	ScrollBar *sb1 = new ScrollBar(win,vec2(100,20),[&](GuiScrollBar &sb){
		spriteCount = 10 + pow(3000-9,sb.getScrollState()) - 1;
		scText->setText(fm::toString(spriteCount));
		tester.setSpriteCount(spriteCount);
	});
	
	GuiText *mvText = new GuiText(win,"0%");
	
	ScrollBar *pb1 = new ScrollBar(win,vec2(100,20),[&](GuiScrollBar &sb){
		mvText->setText(fm::toString(int(sb.getScrollState()*100)) + "%");
		tester.setMovingSprites(sb.getScrollState());
	});
	
	pb1->setPosition(sb1->getPosition() + sb1->getSize() * vec2(0,1));
	mvText->setPosition(pb1->getPosition() + pb1->getSize() * vec2(1.5,0));
	
	win.getMainLayout().addChildElement(sb1);
	win.getMainLayout().addChildElement(scText);
	win.getMainLayout().addChildElement(spText);
	win.getMainLayout().addChildElement(pb1);
	win.getMainLayout().addChildElement(mvText);
	
	Clock fpsDispClk;
	bool running = true;
	for (fm::Size frameCounter=0;running;++frameCounter)
	{
		if (fpsDispClk.getSeconds() > 1)
		{
			fpsDispClk.setTime(fpsDispClk.getSeconds() - 1);
			win.setTitle("sprite " + fm::toString(frameCounter));
			frameCounter = 0;
		}
		spText->setText(fm::toString(tester.getSpriteCoount()));
		 
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
		}
		
		tester.onUpdate(win.getUpdateInterval());
		
		
		win.clear();
		win.draw(tester);
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}
