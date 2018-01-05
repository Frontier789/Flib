#include <Frontier.hpp>
#include "PixyGame.hpp"
#include <deque>

class MyPixyGameTest : public PixyGame
{
	deque<vec2i> bodyPoints;
	fm::Size bodyLength;
	vector<vec2i> foods;
	vec2i v;

public:
	/////////////////////////////////////////////////////////////
	MyPixyGameTest() : PixyGame("PixySnake",vec2(640,480) / 20), bodyLength(5), v(1,0) {
		
		loadImage("brick.png","brick");
		loadImage("apple.png","apple");

		bodyPoints.push_front(vec2i(0,0));

		setMapColor(bodyPoints.front(),vec4::Red);
		setMapImage(bodyPoints.front(),"brick");
	}

	/////////////////////////////////////////////////////////////
	void onTick(fm::Size tick) override
	{
		if (tick % 10 == 0) moveForward();

		if (tick % 100 == 0) randomFood();
	}

	/////////////////////////////////////////////////////////////
	void randomFood()
	{
		vec2i p = vec2(random(),random()) * getMapSize();

		if (getMapColor(p) == vec4::Black)
			putFood(p);
	}

	/////////////////////////////////////////////////////////////
	void setBodyLength(fm::Size newLength)
	{
		bodyLength = newLength;

		setTitle("PixySnake ==== score: " + toString(newLength));
	}

	/////////////////////////////////////////////////////////////
	void moveForward()
	{
		vec2i np = (bodyPoints.front() + v + getMapSize()) % getMapSize();
		
		if (validPoint(np))
		{
			putHead(np);

			for (int db=0;bodyPoints.size() > bodyLength && db<2;++db)
			{
				remTail();
			}
		}
	}

	/////////////////////////////////////////////////////////////
	void onKey(fw::KeyboardEvent key) override
	{
		vec2i nv;
		if (key.code == Keyboard::W || key.code == Keyboard::Up)    nv = vec2i(0,-1);
		if (key.code == Keyboard::A || key.code == Keyboard::Left)  nv = vec2i(-1,0);
		if (key.code == Keyboard::S || key.code == Keyboard::Down)  nv = vec2i(0,1);
		if (key.code == Keyboard::D || key.code == Keyboard::Right) nv = vec2i(1,0);	
		if (key.code == Keyboard::G) setBodyLength(bodyLength+1);	

		if (nv != -v && nv.LENGTH())
			v = nv;
	}

	/////////////////////////////////////////////////////////////
	void cutBody(vec2i p)
	{
		bool found = false;
		int newLength = 1;

		for (fm::Size i=1;i<bodyPoints.size();++i)
		{
			if (bodyPoints[i] == p)
			{
				found = true;
				newLength = i;
			}

			if (found)
			{
				setMapColor(bodyPoints[i],vec4::Black);
				setMapImage(bodyPoints[i],"");
			}
		}

		setBodyLength(newLength+1);
		bodyPoints.resize(newLength);
	}

	/////////////////////////////////////////////////////////////
	void putHead(vec2i p)
	{
		for (fm::Size i=0;i<foods.size();++i)
		{
			if (foods[i] == p)
			{
				setBodyLength(bodyLength + 2);
				remFood(i);
				break;
			}
		}

		if (getMapColor(p) == vec4::Blue)
			cutBody(p);

		setMapColor(bodyPoints.front(),vec4::Blue);
		setMapColor(p,vec4::Red);
		setMapImage(p,"brick");
		bodyPoints.push_front(p);
	}

	/////////////////////////////////////////////////////////////
	void remFood(fm::Size i)
	{
		foods[i] = foods.back();
		foods.pop_back();
	}

	/////////////////////////////////////////////////////////////
	void putFood(vec2i p)
	{
		foods.push_back(p);
		setMapColor(p,vec4::White);
		setMapImage(p,"apple");
	}

	/////////////////////////////////////////////////////////////
	void remTail()
	{
		setMapColor(bodyPoints.back(),vec4::Black);
		setMapImage(bodyPoints.back(),"");

		bodyPoints.pop_back();
	}
};

int main()
{
	MyPixyGameTest().run();
}

