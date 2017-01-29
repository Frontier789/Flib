#include <cstdlib>
#include <ctime>

using namespace std;

#include "HexTerrain.hpp"

HexTerrain::HexTerrain(int size) : data(size,-200), colorStyle(0)
{
	
}

HexTerrain::TriangleFiller::TriangleFiller(vec2i Ap,vec2i Bp,vec2i Cp,float mxDif,float difScale) : Ap(Ap), Bp(Bp), Cp(Cp), mxDif(mxDif), difScale(difScale)
{
	
}

float getRandom(float mn,float mx)
{
	float val01 = ((double) rand() / (RAND_MAX));
	return val01*mx + (1.0-val01)*mn;
}

void HexTerrain::TriangleFiller::fill(HexTerrain &ter)
{
	auto middleFiller = [&](vec2i A,vec2i B) {
		
		vec2i mid = (A+B)/2;
		
		if (ter.data.getHeight(mid) == -200)
		{
			float avg = (ter.data.getHeight(A) + ter.data.getHeight(B))/2; 
			
			ter.data.getHeight(mid) = avg + getRandom(-mxDif,mxDif);
		}
	};
	
	if (((Ap-Bp)/2)*2 != Ap-Bp) return;
	
	middleFiller(Ap,Bp);
	middleFiller(Bp,Cp);
	middleFiller(Cp,Ap);
	
	vec2i A_B = (Ap+Bp)/2;
	vec2i B_C = (Bp+Cp)/2;
	vec2i C_A = (Cp+Ap)/2;
	
	TriangleFiller(Ap,A_B,C_A,mxDif*difScale,difScale).fill(ter);
	TriangleFiller(Bp,A_B,B_C,mxDif*difScale,difScale).fill(ter);
	TriangleFiller(Cp,C_A,B_C,mxDif*difScale,difScale).fill(ter);
	TriangleFiller(B_C,A_B,C_A,mxDif*difScale,difScale).fill(ter);
}

void HexTerrain::TriangleFiller::extendToTwice()
{
	Ap *= 2;
	Bp *= 2;
	Cp *= 2;
}

void HexTerrain::gen()
{
	data.clear();
	data.getHeight(vec2i(0,0)) = getRandom(.5,1);
	
	int size = data.getSize();
	
	data.getHeight(vec2i( size,    0)) = getRandom(-0.5,0.1);
	data.getHeight(vec2i(    0, size)) = getRandom(-0.5,0.1);
	data.getHeight(vec2i(-size, size)) = getRandom(-0.5,0.1);
	data.getHeight(vec2i(-size,    0)) = getRandom(-0.5,0.1);
	data.getHeight(vec2i(    0,-size)) = getRandom(-0.5,0.1);
	data.getHeight(vec2i( size,-size)) = getRandom(-0.5,0.1);
	
	float spread = getRandom(.6,.75);
	
	triangleFillers.push_back(TriangleFiller(vec2i(),vec2i( size,    0),vec2i(    0, size),.5,spread));
	triangleFillers.push_back(TriangleFiller(vec2i(),vec2i(    0, size),vec2i(-size, size),.5,spread));
	triangleFillers.push_back(TriangleFiller(vec2i(),vec2i(-size, size),vec2i(-size,    0),.5,spread));
	triangleFillers.push_back(TriangleFiller(vec2i(),vec2i(-size,    0),vec2i(    0,-size),.5,spread));
	triangleFillers.push_back(TriangleFiller(vec2i(),vec2i(    0,-size),vec2i( size,-size),.5,spread));
	triangleFillers.push_back(TriangleFiller(vec2i(),vec2i( size,-size),vec2i( size,    0),.5,spread));
	
	C(triangleFillers.size())
		triangleFillers[i].fill(*this);
	
	loadColors();
}

void HexTerrain::extendToTwice()
{
	if (data.getSize() == 1024) return;
	
	data = data.extendToTwice();
	
	C(triangleFillers.size())
	{
		triangleFillers[i].extendToTwice();
		triangleFillers[i].fill(*this);
	}
	
	loadColors();
}

void HexTerrain::reduceToHalf()
{
	if (data.getSize() == 1) return;
	
	data = data.reduceToHalf();

	C(triangleFillers.size())
	{
		triangleFillers[i].Ap /= 2;
		triangleFillers[i].Bp /= 2;
		triangleFillers[i].Cp /= 2;
	}

	loadColors();
}

void HexTerrain::setColorStyle(int style)
{
	colorStyle = style;

	loadColors();
}

void HexTerrain::loadColors()
{
	Delegate<vec4,float> colorGetters[] = {
	
	[](float d) -> vec4 {	
		if (d < -1)
		{
			return vec4(0,0,.5);
		}
		
		vec4 clrs[] = {vec4(0,0,.5),vec4(0,.1,.95),vec4(.4,.6,.7),vec4(0.2,0.6,0),vec4(.57,.57,0),vec4(.63,.34,0),vec4(.8,.57,.34),vec4::White,vec4(.87,.91,1)};
		float heights[] = {     -1,           -.5,             0,           0.13,            0.3,            0.6,             0.8,           1,           1.1};
		
		C(sizeof(heights)/sizeof(*heights) - 1)
		{
			if (d >= heights[i] && d < heights[i+1])
			{
				float rat = float(d-heights[i])/(heights[i+1] - heights[i]);
				
				return clrs[i+1] * rat + clrs[i] * (1.0 - rat);
			}
		}
		
		return vec4::White;
	},
	[](float d) -> vec4 {
		float v = max((d+1)/2,0.f);

		return (vec4(sin(v*9.5+0.5),cos(v*10.1+0.1),sin(v*8.2+4.3),1)+vec4(1,1,1,1))/2;
	},
	[](float d) -> vec4 {
		float v = (d+1)/2;
		v = v*v;
		return vec4(255,192,203,255)/255 * v + vec4(116,12,105,255)/255 * (1-v);
	},
	[](float d) -> vec4 {
		float v = (d+1)/2;
		
		float intensity1 = pow(max<float>((1-v*0.6),0.f),2);
		float intensity2 = pow(max<float>((1-v*0.3),0.f),2)*0.5;
		
		vec3 col = vec3(1,0,0)*intensity1 + vec3(.8,.6,.4)*intensity2;
		float overflow = max<float>(col.r-1,0) + max<float>(col.g-1,0) + max<float>(col.b-1,0);
		col += vec3(overflow); 
		
		return vec4(col,1);
	}
	};
	
	int colorGetterCount = sizeof(colorGetters)/sizeof(*colorGetters);

	plotter.load<float>(data,colorGetters[colorStyle % colorGetterCount]);
}

void HexTerrain::onDraw(ShaderManager &shader)
{
	plotter.onDraw(shader);
}

void HexTerrain::onUpdate(const fm::Time &dt)
{
	plotter.onUpdate(dt);
}
