#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <random>

using namespace std;

class Branch;
class Leaf;

class Tree : public fg::Drawable
{
public:
	DrawData branchDraw;
	DrawData leafDraw;
	fm::Size branchCount;
	Branch  *trunk;
	fm::Size seed;
	std::mt19937 mersenne;
    std::uniform_real_distribution<double> distribution;
	std::vector<Leaf*> leaves;
	float windX;
	float nextWindX;
	Clock windClock;
	Time windTransTime;
	Time nextWindTime;
	bool windEnabled;
	Delegate<float,float> funcThicknessRatio;
	Delegate<float,float> funcTargetSize;
	Delegate<float,float> funcMaxFoodHandover;
	Delegate<vec4f,vec4f> funcColor;
	Delegate<vec4f,vec4f> funcLeafDColor;
	Delegate<float,float> funcChildrenGoal;
	Delegate<float,float> funcMaxAngleChild;
	Delegate<float,float> funcMinGrowPoint;
	Delegate<float,float> funcLeafProbability;
	Delegate<vec4f,vec4f> funcLeafTurnYellow;
	
	Tree();
	~Tree();
	
	void onDraw(ShaderManager &shader) override;
	void onUpdate(const fm::Time &dt) override;
	
	void addBranch(Branch &branch);
	void remBranch(Branch &branch);
	
	void reset(fm::Size seed);
	
	void buildDrawData();
	void buildBranchDraw();
	void buildLeafDraw();
	void collectLeaves();
	void enableWind(bool enable);
	
	float getRandom();
	void letLeavesFall(fm::Time dt);
	
	bool isBlue() const {return seed & 0x1;}
	float getWindX();
};

class Leaf
{
public:
	Tree &tree;
	vec4 color;
	vec3 base;
	vec3 dir;
	vec3 left;
	fm::Size depth;
	float growPoint;
	float growSize;
	float health;
	
	Leaf(Tree &tree,vec4 color,fm::Size depth,float growPoint,float growSize);
	
	void turnYellow(fm::Time dt);
	
	fm::Size getVertCount(bool fromBranch = true) const;
	void fillVertices(vec3 *&ptsPtr,vec4 *&clrPtr,bool fromBranch = true) const;
};

class Branch
{
public:
	Tree &tree;
	vec3 offset;
	vec4 color;
	vec4 leafDColor;
	fm::Size depth;
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
		   fm::Size depth,
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
	
	void collectLeaves(std::vector<Leaf*> &leaves);
	
	void traverse(Delegate<void,Branch&,fm::Size> func,fm::Size depth = 0);
};

Tree::Tree() : branchCount(0),
			   trunk(nullptr),
			   windX(0),
			   nextWindX(0),
			   windTransTime(seconds(1)),
			   nextWindTime(seconds(5)),
			   windEnabled(false)
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
		
		vec4 targetColor = isBlue() ? vec4(.2,.2,.2) : vec4(.4,.24,.24,1.0);
		
		return (c + targetColor)/2;
	};
	funcLeafDColor = [this](vec4 c) -> vec4 {
		
		vec4 targetColor = isBlue() ? vec4(.2,.6,1) : vec4::Lime;
		vec3 coefs = isBlue() ? vec3(.3,.5,.3) : vec3(.5,.3,.3);
		
		return c * .8 + targetColor * .2 + vec4(vec3((getRandom()-.5),(getRandom()-.5),(getRandom()-.5))*coefs,0);
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
	funcLeafProbability = [](float p) -> float {
		return (p + 1)/2;
	};
	funcLeafTurnYellow = [this](vec4 c) -> vec4 {
		
		bool green = !(c.r > c.g && c.r > c.b*2);
		
		vec4 deadColor = isBlue() ? vec4(.5,.5,.1) : vec4(.8,.8,.2);
		
		vec4 target = green ? deadColor : vec4(.7,.3,.1);
		
		return c * .8 + target * .2 + vec4(vec3((getRandom()-.5)*.5,(getRandom()-.5)*.5,(getRandom()-.5)*.5),0);
	};
	
	reset(0);
}

void Tree::reset(fm::Size seed)
{
	static random_device dev;
	
	windX = 0;
	nextWindX = 0;
	branchDraw.reset();
	leafDraw.reset();
	branchCount = 0;
	delete trunk;
	this->seed = seed ? seed : dev();
	leaves.clear();
	mersenne = mt19937(this->seed);
	distribution = decltype(distribution)(0,1);
	
	cout << "seed: " << this->seed << endl;
	
	trunk = new Branch(*this,
					   vec3(0,1,0),
					   isBlue() ? vec4(.4,.3,.2) : vec4(.6,.1,.05,1.0),
					   isBlue() ? vec4::Blue : vec4::Green,
					   0,
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
{/*
	cout << branchDraw.m_attrs.size() << endl;
	for (auto it : branchDraw.m_attrs)
		cout << it.first << " -> " << it.second << ": " << 
			it.second->bufferUsage << "  " <<
			it.second->instancesPerUpdate << "  " <<
			it.second->componentType << "  " <<
			it.second->components << "  " <<
			it.second->buf << "  " <<
			it.second->stride << "  " <<
			it.second->count << "  " <<
			it.second->ownBuffer << endl;
			
	for (auto &it : branchDraw.m_drawCalls)
		cout << it.primitive << "  " <<
			    it.componentType << "  " <<
			    it.indexCount << "  " <<
			    it.instances << "  " <<
			    it.drawBeg << "  " <<
			    it.drawLen << "  " <<
			    it.buf << "  " <<
			    it.ownBuffer << endl;
		*/
	shader.draw(branchDraw);
	shader.draw(leafDraw);
}

void Tree::onUpdate(const fm::Time &dt)
{ 
	if (leaves.size())
		letLeavesFall(dt);
	else if (windEnabled)
		buildDrawData();
}

void Tree::letLeavesFall(fm::Time dt)
{
	for (auto ptr : leaves)
		ptr->turnYellow(dt);
	
	buildLeafDraw();
	buildBranchDraw();
}

void Tree::addBranch(Branch &)
{
	branchCount++;
}

void Tree::remBranch(Branch &)
{
	branchCount--;
}

void Tree::buildDrawData()
{
	buildBranchDraw();
	buildLeafDraw();
}

void Tree::buildBranchDraw()
{
	branchDraw.reset();
	
	fm::Size vertCount = trunk->getVertCount();
	
	vertCount += trunk->getLeafVertCount();
	
	vector<vec3> pts(vertCount,vec3());
	vector<vec4> clr(vertCount,vec4::Red);
	
	vec3 *ptsPtr = &pts[0];
	vec4 *clrPtr = &clr[0];
	
	trunk->fillVertices(vec3(),trunk->offset.sgn(),vec3(vec2(trunk->offset).perp(),0).sgn(),ptsPtr,clrPtr);
	trunk->fillLeafVertices(vec3(),trunk->offset.sgn(),vec3(vec2(trunk->offset).perp(),0).sgn(),ptsPtr,clrPtr);
	
	branchDraw.positions.set(&pts[0],pts.size());
	branchDraw.colors.set(&clr[0],clr.size());
	branchDraw.addDraw(0,vertCount,fg::Triangles);
}

void Tree::buildLeafDraw()
{
	leafDraw.reset();
	
	fm::Size vertCount = 0;
	
	for (auto ptr : leaves)
		vertCount += ptr->getVertCount(false);
	
	vector<vec3> pts(vertCount,vec3());
	vector<vec4> clr(vertCount,vec4::Red);
	
	vec3 *ptsPtr = &pts[0];
	vec4 *clrPtr = &clr[0];
	
	for (auto ptr : leaves)
		ptr->fillVertices(ptsPtr,clrPtr,false);
	
	leafDraw.positions.set(&pts[0],pts.size());
	leafDraw.colors.set(&clr[0],clr.size());
	leafDraw.addDraw(0,vertCount,fg::Triangles);
}

void Tree::collectLeaves()
{
	leaves.clear();
	trunk->collectLeaves(leaves);
	buildBranchDraw();
	
	for (auto ptr : leaves)
		ptr->health *= getRandom();
}

void Tree::enableWind(bool enable)
{
	windEnabled = enable;
	windClock.restart();
	
	windX = 0;
	nextWindX = 0;
	
	buildDrawData();
}

float Tree::getWindX()
{
	if (!windEnabled) return 0;
	
	auto mix = [](float a,float b,float r) {
		
		r = 3*r*r - 2*r*r*r;
		
		return a * (1-r) + b * r;
		
	};
	
	Time secs = windClock.getTime();
	if (secs > nextWindTime)
	{
		windX = nextWindX;
		nextWindX = getRandom() * 400 - 200;
		windClock.setTime(secs-nextWindTime);
		
		windTransTime = seconds(getRandom() * 2 + 0.6);
		nextWindTime = windTransTime + seconds(getRandom());
		
		return mix(windX,nextWindX,(secs - nextWindTime) / windTransTime);
	}
	else if (secs < windTransTime)
	{
		return mix(windX,nextWindX,secs / windTransTime);
	}
	
	return nextWindX;
}

float Tree::getRandom()
{
	return distribution(mersenne);
}

Leaf::Leaf(Tree &tree,
		   vec4 color,
		   fm::Size depth,
		   float growPoint,
		   float growSize) : 
tree(tree),
color(color),
base(vec3()),
dir(vec3(0,1,0)),
left(vec3(-1,0,0)),
depth(depth),
growPoint(growPoint),
growSize(growSize),
health(100)
{
	
}

fm::Size Leaf::getVertCount(bool fromBranch) const
{
	if ((fromBranch && health > 0) || 
		(!fromBranch && health < 0))
			return 6;
	
	return 0;
}

void Leaf::fillVertices(vec3 *&ptsPtr,vec4 *&clrPtr,bool fromBranch) const
{
	if (!((fromBranch && health > 0) || 
		  (!fromBranch && health < 0)))
			return;
	
	static const vec3 tpt[] = {vec3(0,0),vec3(-.4,.5),vec3(0,1),
							   vec3(0,0),vec3(0,1),vec3(.4,.5)};
	
	for (vec3 v : tpt)
	{
		*ptsPtr++ = base + left * v.x * growSize + dir * v.y * growSize;
		*clrPtr++ = color;
	}
}

void Leaf::turnYellow(fm::Time dt)
{
	if (base.y > tree.getRandom()*10 && health < 0)
	{
		base.y -= dt.asSecs() * (100 - tree.getRandom() * 50) * 4;
		base.x += tree.getWindX() * dt.asSecs();
	}
	
	if (health > 0)
	{
		health += 0.0005;
		health -= tree.getRandom() * pow(abs(tree.getWindX()),2) / 100000.0;
	}
	
	vec4 nColor = tree.funcLeafTurnYellow(color);
	
	float r = dt / seconds(.2);
	
	color = color * (1 - r) + nColor * r;
}



Branch::Branch(Tree &tree,
			   vec3 offset,
			   vec4 color,
			   vec4 leafDColor,
			   fm::Size depth,
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
depth(depth),
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
		leaves.push_back(new Leaf(tree,leafDColor,depth,1,10));
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
							 depth+1,1,1,
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
	
	dir = (dir * 2000 + vec3(1,0,0) * tree.getWindX() * depth * depth).sgn();
	left = dir.cross(left).cross(dir);
	
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
	dir = (dir * 2000 + vec3(1,0,0) * tree.getWindX() * depth * depth).sgn();
	left = dir.cross(left).cross(dir);
	
	vec3 realOffset = dir * offset.y + left * offset.x;
	vec3 realPerp   = vec3(vec2(realOffset).perp(),0).sgn();
	
	for (Leaf *ptr : leaves)
		if (ptr->health > 0)
		{
			ptr->base = base + realOffset * ptr->growPoint;
			ptr->dir  = (realOffset + vec3(1,0,0) * tree.getWindX() * tree.getRandom() / 35.0).sgn();
			ptr->left = realPerp.sgn();
			ptr->fillVertices(ptsPtr,clrPtr);
		}
	
	for (Branch *ptr : children)
		ptr->fillLeafVertices(base + realOffset * ptr->growPoint,realOffset.sgn(),realPerp.sgn(),ptsPtr,clrPtr);
}

void Branch::collectLeaves(std::vector<Leaf*> &leaves)
{
	for (auto ptr : this->leaves)
		leaves.push_back(ptr);
	
	for (auto ptr : this->children)
		ptr->collectLeaves(leaves);
}

void Branch::traverse(Delegate<void,Branch&,fm::Size> func,fm::Size depth)
{
	func(*this,depth);
	
	for (Branch *ptr : children)
		ptr->traverse(func,depth+1);
}

void printUsage()
{
	cout << "Usage:" << endl;
	cout << "\tJ: grow tree" << endl;
	cout << "\tR: new tree" << endl;
	cout << "\tA: toggle wind" << endl;
	cout << "\tB: start fall" << endl;
}

int main()
{
	printUsage();
	
	GuiWindow win(vec2(640,480),"tree omg");
	win.enableKeyRepeat();
	 
	cout << glGetString(GL_VERSION) << endl;
	
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
				if (ev.key.code == Keyboard::A)
				{
					tree.enableWind(!tree.windEnabled);
				}
				if (ev.key.code == Keyboard::B)
				{
					tree.collectLeaves();
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
					
					tree.trunk->traverse([&](Branch &,fm::Size depth){
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
		
		tree.onUpdate(win.getUpdateInterval());
		
		if (needRebuild)
			tree.buildDrawData();
		
		win.clear();
		tree.onDraw(win.getShader());
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}
