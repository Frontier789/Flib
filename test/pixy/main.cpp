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
	MyPixyGameTest() : PixyGame("MyGameTest",vec2(640,480) / 16), bodyLength(5), v(1,0) {
		
		bodyPoints.push_front(vec2i(0,0));

		setMapColor(bodyPoints.front(),vec4::Red);
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
	void moveForward()
	{
		vec2i np = bodyPoints.front() + v;
		
		if (validPoint(np))
		{
			putHead(np);

			if (bodyPoints.size() > bodyLength) remTail();
		}
	}

	/////////////////////////////////////////////////////////////
	void onKey(fw::KeyboardEvent key) override
	{
		vec2i nv;
		if (key.code == Keyboard::W) nv = vec2i(0,-1);
		if (key.code == Keyboard::A) nv = vec2i(-1,0);
		if (key.code == Keyboard::S) nv = vec2i(0,1);
		if (key.code == Keyboard::D) nv = vec2i(1,0);		

		if (nv != -v)
			v = nv;
	}

	/////////////////////////////////////////////////////////////
	void putHead(vec2i p)
	{
		for (fm::Size i=0;i<foods.size();++i)
		{
			if (foods[i] == p)
			{
				bodyLength++;
				remFood(i);
				break;
			}
		}

		setMapColor(bodyPoints.front(),vec4::Blue);
		setMapColor(p,vec4::Red);
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
		setMapColor(p,vec4::Forest);
	}

	/////////////////////////////////////////////////////////////
	void remTail()
	{
		setMapColor(bodyPoints.back(),vec4::Black);

		bodyPoints.pop_back();
	}
};

int main()
{
	MyPixyGameTest().run();
}

