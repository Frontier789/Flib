#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <random>

using namespace std;

class Branch;

class Tree : public fg::Drawable
{
public:
	DrawData drawData;
	fm::Size branchCount;
	Branch  *trunk;
	fm::Size seed;
	std::mt19937 mersenne;
    std::uniform_real_distribution<double> distribution;
	Delegate<float,float> funcThicknessRatio;
	Delegate<float,float> funcTargetSize;
	Delegate<float,float> funcMaxFoodHandover;
	Delegate<vec4f,vec4f> funcColor;
	Delegate<vec4f,vec4f> funcLeafDColor;
	Delegate<float,float> funcChildrenGoal;
	Delegate<float,float> funcMaxAngleChild;
	Delegate<float,float> funcMinGrowPoint;
	Delegate<float,float> funcLeafProbability;
	
	Tree();
	~Tree();
	
	void onDraw(ShaderManager &shader) override;
	void onUpdate(const fm::Time &dt) override {};
	
	void addBranch(Branch &branch);
	void remBranch(Branch &branch);
	
	void reset(fm::Size seed);
	
	void buildDrawData();
	
	float getRandom();
	
	bool isAutumn() const {return seed & 0x1;}
};

class Leaf
{
public:
	Tree &tree;
	vec4 color;
	float growPoint;
	float growSize;
	
	Leaf(Tree &tree,vec4 color,float growPoint,float growSize);
	
	fm::Size getVertCount() const;
	void fillVertices(vec3 base,vec3 dir,vec3 left,vec3 *&ptsPtr,vec4 *&clrPtr) const;
};

class Branch
{
public:
	Tree &tree;
	vec3 offset;
	vec4 color;
	vec4 leafDColor;
	float growPoint;
	float thickness;
	float thicknessRatio;
	float targetSize;
	float maxFoodHandover;
	float childrenGoal;
	float maxAngleChild;
	float minGrowPoint;
	float leafProbability;
	
	vector<Branch*> children;
	vector<Leaf*> leaves;
	
	Branch(Tree &tree,
		   vec3 offset,
		   vec4 color,
		   vec4 leafDColor,
		   float growPoint,
		   float thickness,
		   float thicknessRatio,
		   float targetSize,
		   float maxFoodHandover,
		   float childrenGoal,
		   float maxAngleChild,
		   float minGrowPoint,
		   float leafProbability);
	
	~Branch();
	
	Branch *growNewBranch();
	void grow(float food);
	
	void growSize(float food);
	
	fm::Size getVertCount() const;
	void fillVertices(vec3 base,vec3 dir,vec3 left,vec3 *&ptsPtr,vec4 *&clrPtr) const;
	
	fm::Size getLeafVertCount() const;
	void fillLeafVertices(vec3 base,vec3 dir,vec3 left,vec3 *&ptsPtr,vec4 *&clrPtr) const;
	
	void traverse(Delegate<void,Branch&,fm::Size> func,fm::Size depth = 0);
};

Tree::Tree() : branchCount(0),
			   trunk(nullptr)
{
	funcThicknessRatio = [](float f) {
		return f * .6;
	};
	funcTargetSize = [this](float f) -> float {
		return f * (.4 + getRandom() * .2);
	};
	funcMaxFoodHandover = [](float f) {
		return f;
	};
	funcColor = [this](vec4 c) {
		
		vec4 targetColor = isAutumn() ? vec4(.2,.2,.2) : vec4(.4,.24,.24,1.0);
		
		return (c + targetColor)/2;
	};
	funcLeafDColor = [this](vec4 c) {
		
		vec4 targetColor = isAutumn() ? vec4(.5,.4,.2) : vec4::Lime;
		
		return c * .8 + targetColor * .2 + vec4(vec3((getRandom()-.5)*.5,(getRandom()-.5)*.3,(getRandom()-.5)*.3),0);
	};
	funcChildrenGoal = [this](float c) -> float {
		return c + getRandom() / 2 - .2;
	};
	funcMaxAngleChild = [](float a) -> float {
		return (a + 180.f)/2;
	};
	funcMinGrowPoint = [this](float p) -> float {
		return (p + .95) / 2 - getRandom() / 2;
	};
	funcLeafProbability = [this](float p) -> float {
		return (p + 1)/2;
	};
	
	reset(0);
}

void Tree::reset(fm::Size seed)
{
	static random_device dev;
	
	drawData.reset();
	branchCount = 0;
	delete trunk;
	this->seed = seed ? seed : dev();
	mersenne = mt19937(this->seed);
	distribution = decltype(distribution)(0,1);
	cout << "seed: " << this->seed << endl;
	
	trunk = new Branch(*this,
					   vec3(0,1,0),
					   isAutumn() ? vec4(.4,.3,.2) : vec4(.6,.1,.05,1.0),
					   isAutumn() ? vec4::Red : vec4::Green,
					   1,1,.35,800,.95,
					   3+getRandom()*4,
					   110,
					   .1+getRandom()*.4,
					   .1);
	
	// trunk = new Branch(*this,vec3(0,1,0),vec4(.6,.1,.05,1.0),vec4::Green,1,1,getRandom()*.3 + .2,getRandom()*2000+200,getRandom()*.5+.5,getRandom()*4+1,getRandom()*120+30,.2);
}

Tree::~Tree()
{
	delete trunk;
}

void Tree::onDraw(ShaderManager &shader)
{
	shader.draw(drawData);
}

void Tree::addBranch(Branch &branch)
{
	branchCount++;
}

void Tree::remBranch(Branch &branch)
{
	branchCount--;
}

void Tree::buildDrawData()
{
	drawData.reset();
	
	fm::Size vertCount = trunk->getVertCount();
	
	vertCount += trunk->getLeafVertCount();
	
	vector<vec3> pts(vertCount,vec3());
	vector<vec4> clr(vertCount,vec4::Red);
	
	vec3 *ptsPtr = &pts[0];
	vec4 *clrPtr = &clr[0];
	
	trunk->fillVertices(vec3(),trunk->offset.sgn(),vec3(vec2(trunk->offset).perp(),0).sgn(),ptsPtr,clrPtr);
	trunk->fillLeafVertices(vec3(),trunk->offset.sgn(),vec3(vec2(trunk->offset).perp(),0).sgn(),ptsPtr,clrPtr);
	
	drawData.positions.set(&pts[0],pts.size());
	drawData.colors.set(&clr[0],clr.size());
	drawData.addDraw(0,vertCount,fg::Triangles);
}

float Tree::getRandom()
{
	return distribution(mersenne);
}

Leaf::Leaf(Tree &tree,
		   vec4 color,
		   float growPoint,
		   float growSize) : 
tree(tree),
color(color),
growPoint(growPoint),
growSize(growSize)
{
	
}

fm::Size Leaf::getVertCount() const
{
	return 6;
}

void Leaf::fillVertices(vec3 base,vec3 dir,vec3 left,vec3 *&ptsPtr,vec4 *&clrPtr) const
{
	static const vec3 tpt[] = {vec3(0,0),vec3(-.4,.5),vec3(0,1),
							   vec3(0,0),vec3(0,1),vec3(.4,.5)};
	
	for (vec3 v : tpt)
	{
		*ptsPtr++ = base + left * v.x * growSize + dir * v.y * growSize;
		*clrPtr++ = color;
	}
}



Branch::Branch(Tree &tree,
			   vec3 offset,
			   vec4 color,
			   vec4 leafDColor,
			   float growPoint,
			   float thickness,
			   float thicknessRatio,
			   float targetSize,
			   float maxFoodHandover,
			   float childrenGoal,
			   float maxAngleChild,
			   float minGrowPoint,
			   float leafProbability) : 
tree(tree),
offset(offset),
color(color),
leafDColor(leafDColor),
growPoint(growPoint),
thickness(thickness),
thicknessRatio(thicknessRatio),
targetSize(targetSize),
maxFoodHandover(maxFoodHandover),
childrenGoal(childrenGoal),
maxAngleChild(maxAngleChild),
minGrowPoint(minGrowPoint),
leafProbability(leafProbability)
{
	tree.addBranch(*this);
	
	if (tree.getRandom() < leafProbability)
		leaves.push_back(new Leaf(tree,leafDColor,1,10));
}

Branch::~Branch()
{
	for (Branch *ptr : children)
		delete ptr;
		
	for (Leaf *ptr : leaves)
		delete ptr;
	
	tree.remBranch(*this);
}

Branch *Branch::growNewBranch()
{
	Branch *ptr = new Branch(tree,offset.sgn(),
							 tree.funcColor(color),
							 tree.funcLeafDColor(leafDColor),
							 1,1,
							 tree.funcThicknessRatio(thicknessRatio),
							 tree.funcTargetSize(targetSize),
							 tree.funcMaxFoodHandover(maxFoodHandover),
							 tree.funcChildrenGoal(childrenGoal),
							 tree.funcMaxAngleChild(maxAngleChild),
							 tree.funcMinGrowPoint(minGrowPoint),
							 tree.funcLeafProbability(leafProbability));
	
	children.push_back(ptr);
	
	return ptr;
}

void Branch::grow(float food)
{
	if (targetSize < 10)
	{
		food = 0;
	}
	else if (thickness * offset.length() > targetSize * .8)
	{
		if (children.size() < childrenGoal + thickness * offset.length() / targetSize)
		{
			Branch *ptr = growNewBranch();
			
			float angle = tree.getRandom() * maxAngleChild + (180 - maxAngleChild) / 2;
			
			ptr->offset = vec2(pol2(1,deg(angle)));
			
			ptr->growPoint = tree.getRandom() * (1 - minGrowPoint) + minGrowPoint;
		}
	}
	
	if (children.size())
	{
		float thicknessSum = 0;
		for (Branch *ptr : children)
			thicknessSum += ptr->thickness;
		
		float foodHandover = min(thickness * offset.length() / targetSize,maxFoodHandover);
		
		float foodRatio = food * foodHandover / thicknessSum;
		food -= food * foodHandover;
			
		for (Branch *ptr : children)
			ptr->grow(foodRatio * ptr->thickness);
	}
	
	growSize(food);
}

void Branch::growSize(float food)
{
	float len = offset.length();
	
	thickness += food * thicknessRatio / len;
	
	len += food * (1 - thicknessRatio) / thickness;
	
	offset = offset.sgn() * len;
}

fm::Size Branch::getVertCount() const
{
	fm::Size ret = 6;
	
	for (Branch *ptr : children)
		ret += ptr->getVertCount();
	
	return ret;
}
	
void Branch::fillVertices(vec3 base,vec3 dir,vec3 left,vec3 *&ptsPtr,vec4 *&clrPtr) const
{
	static const vec3 tpt[] = {vec3(-.8,0),vec3(.8,0),vec3(.5,1),
							   vec3(-.8,0),vec3(.5,1),vec3(-.5,1)};
	
	vec3 realOffset = dir * offset.y + left * offset.x;
	vec3 realPerp   = vec3(vec2(realOffset).perp(),0).sgn();
	
	for (vec3 v : tpt)
	{
		*ptsPtr++ = base + realPerp * v.x * thickness + realOffset * v.y;
		*clrPtr++ = color;
	}
	
	for (Branch *ptr : children)
		ptr->fillVertices(base + realOffset * ptr->growPoint,realOffset.sgn(),realPerp.sgn(),ptsPtr,clrPtr);
}

fm::Size Branch::getLeafVertCount() const
{
	fm::Size ret = 0;
	
	for (Leaf *ptr : leaves)
		ret += ptr->getVertCount();
	
	for (Branch *ptr : children)
		ret += ptr->getLeafVertCount();
	
	return ret;
}

void Branch::fillLeafVertices(vec3 base,vec3 dir,vec3 left,vec3 *&ptsPtr,vec4 *&clrPtr) const
{
	vec3 realOffset = dir * offset.y + left * offset.x;
	vec3 realPerp   = vec3(vec2(realOffset).perp(),0).sgn();
	
	for (Leaf *ptr : leaves)
		ptr->fillVertices(base + realOffset * ptr->growPoint,realOffset.sgn(),realPerp.sgn(),ptsPtr,clrPtr);
	
	for (Branch *ptr : children)
		ptr->fillLeafVertices(base + realOffset * ptr->growPoint,realOffset.sgn(),realPerp.sgn(),ptsPtr,clrPtr);
}

void Branch::traverse(Delegate<void,Branch&,fm::Size> func,fm::Size depth)
{
	func(*this,depth);
	
	for (Branch *ptr : children)
		ptr->traverse(func,depth+1);
}


int main()
{
	GuiWindow win(vec2(640,480),"tree omg");
	win.enableKeyRepeat();
	
	win.setClearColor(vec4(vec3(1),0));
	
	Tree tree;
	tree.buildDrawData();
	
	win.getShader().getModelStack().push().mul(MATRIX::translation(win.getSize() * vec2(.5,1)) * MATRIX::scaling(vec3(1,-1,1)));
	
	bool running = true;
	for (;running;)
	{
		bool needRebuild = false;
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
			if (ev.type == Event::FocusLost)
			{
				running = false;
			}
			if (ev.type == Event::MouseWheelMoved)
			{
				float sc = pow(2,ev.wheel.delta);
				win.getShader().getModelStack().mul(MATRIX::scaling(sc,sc,1));
			}
			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::G)
				{
					C(5) tree.trunk->grow(tree.trunk->thickness);
					needRebuild = true;
				}
				if (ev.key.code == Keyboard::J)
				{
					C(50) tree.trunk->grow(tree.trunk->thickness);
					needRebuild = true;
				}
				if (ev.key.code == Keyboard::K)
				{
					C(500) tree.trunk->grow(tree.trunk->thickness);
				}
				if (ev.key.code == Keyboard::H)
				{
					tree.trunk->grow(pow(tree.trunk->thickness ,2));
					needRebuild = true;
				}
				if (ev.key.code == Keyboard::R)
				{
					tree.reset(0);
				}
				if (ev.key.code == Keyboard::Escape)
				{
					running = false;
				}
				if (ev.key.code == Keyboard::S)
				{
					win.capture().saveToFile("fa.png");
					cout << "saved!" << endl;
				}
				if (ev.key.code == Keyboard::L)
				{
					fm::Size maxDepth = 0;
					std::map<fm::Size,fm::Size> depthToCount;
					
					tree.trunk->traverse([&](Branch &b,fm::Size depth){
						maxDepth = max(maxDepth,depth);
						depthToCount[depth]++;
					});
					
					cout << "maximum depth: " << maxDepth << endl;
					cout << "depth distribution: " << endl;
					for (auto it : depthToCount)
						cout << "\t- " << it.first << " lvl: " << it.second << endl;
				}
			}
		}
		
		if (needRebuild)
			tree.buildDrawData();
		
		win.clear();
		tree.onDraw(win.getShader());
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}
