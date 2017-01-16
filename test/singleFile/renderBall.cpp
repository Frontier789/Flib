#include <Frontier.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class BallRenderer
{
public:
	class Ray
	{
	public:
		vec3 p;
		vec3 v;
		
		Ray(vec3 p = vec3(),vec3 v = vec3(1,1,1)) : p(p), v(v.sgn())
		{
			
		}
	};
	
	class Material
	{
	public:
		vec3 dif_color;
		vec3 amb_color;
		vec3 spc_color;
		vec3 ref_color;
		float spc_exp;
		float refrac;
		float density;
		
		Material(vec3 diffuse = vec3(),vec3 ambient = vec3(),vec3 specular = vec3(),vec3 reflect = vec3(),float exponent = 0,float refraction = 0,float opticDensity = 0) : dif_color(diffuse), amb_color(ambient), spc_color(specular), ref_color(reflect), spc_exp(exponent), refrac(refraction), density(opticDensity)
		{
			
		}
	};
	
	class Object
	{
	public:
		Material mat;
		int id;
		
		Object(Material mat = Material()) : mat(mat) 
		{
			
		}
		
		virtual vec3 getNormal(vec3 p,vec3 from) = 0;
		virtual float calcDist(Ray r) = 0;
	};
	
	class Sphere : public Object
	{
	public:
		vec3 o;
		float r;
		
		Sphere(vec3 o = vec3(),float r = 0,Material mat = Material()) : Object(mat), o(o), r(r)
		{
			
		}
		
		vec3 getNormal(vec3 p,vec3 from)
		{
			if ((from-o).LENGTH() < r*r + 0.5)
			{
				return (o-p).sgn();
			}
			
			return (p-o).sgn();
		}
		
		float calcDist(Ray ray)
		{
			vec3 oToP = ray.p - o;
		
			vec3 k = oToP - oToP.projTo(ray.v);
			
			if (k.LENGTH() > r*r)
			{
				return -1;
			}
			
			float dCI = std::sqrt(r*r - k.LENGTH());
			float dCP = std::sqrt(oToP.LENGTH() - k.LENGTH());
			
			bool inside = (oToP.LENGTH() <= r*r + 0.5);
			bool before = (ray.v.dot(oToP) <= 0);
			
			if (inside)
			{
				if (before)
				{
					return dCI + dCP;
				}
				
				return dCI - dCP;
			}
			else
			{
				if (before)
				{
					return dCP - dCI;
				}
				
				return -1;
			}
		}
	};
	
	class Plane : public Object
	{
	public:
		vec3 p;
		vec3 n;
		
		Plane(vec3 p = vec3(),vec3 n = vec3(),Material mat = Material()) : Object(mat), p(p), n(n)
		{
			
		}
		
		vec3 getNormal(vec3 pt,vec3 from)
		{
			if ((p-from).dot(n) > 0) return -n;
			
			return n;
		}
		
		float calcDist(Ray r)
		{
			float d = r.v.dot(n);
			
			if (d == 0)
				return -1;
				
			return -(r.p - p).dot(n) / d;
		}
	};
	
private:
	vec2 m_canvasSize;
	vector<Object*> m_objs;
	float m_minRayTravel;
	vec3 m_camP;
	
public:
	BallRenderer(vec2 canvasSive = vec2()) : m_canvasSize(canvasSive),
											 m_minRayTravel(0.5)
	{
		m_camP = vec3(0,0,-800);
	}
	
	~BallRenderer()
	{
		for (int i=0;i<m_objs.size();++i)
		{
			delete m_objs[i];
		}
	}
	
	void setCanvasSize(vec2 canvasSive)
	{
		m_canvasSize = canvasSive;
	}
	
	void addObject(Object *obj)
	{
		obj->id = m_objs.size();
		m_objs.push_back(obj);
	}
	
	Image render()
	{
		Image img;
		
		img.create(m_canvasSize);
		
		Cv(m_canvasSize)
		{
			if ( int(p.x * m_canvasSize.h + p.y)%int(m_canvasSize.area()/5) == 0) cout << (p.x * m_canvasSize.h + p.y)/(m_canvasSize.area())*100 << "%" << endl;
			
			vec3 p3d = vec3(p - m_canvasSize/2.f,0);
			
			vec4 color = calcColor(Ray(p3d,p3d-m_camP));
			
			img.set(p,clampColor(color)*255);
		}
		
		return img;
	}
	
	float clamp(float mn,float mx,float val)
	{
		if (val < mn) return mn;
		if (val > mx) return mx;
		
		return val;
	}
	
	vec4 clampColor(vec4 c)
	{
		return vec4(clamp(0,1,c.r),
					clamp(0,1,c.g),
					clamp(0,1,c.b),
					clamp(0,1,c.a));
	}
	
	void calcVecs(Ray r,vec3 &pt,vec3 &norm,vec3 &tangent,bool &hit,Object *&obj)
	{
		float mnDist = -1;
		int objectIndex;
		
		for (int i=0;i<m_objs.size();++i)
		{
			float d = m_objs[i]->calcDist(r);
			
			if (d > m_minRayTravel)
			{
				if (mnDist < 0 || mnDist > d)
				{
					mnDist = d;
					objectIndex = i;
				}
			}
		}
		
		if (mnDist > 0)
		{
			hit = true;
			pt  = r.p + r.v * mnDist;
			norm = m_objs[objectIndex]->getNormal(pt,r.p);
			tangent = r.v.cross(norm).cross(norm).sgn();
			obj = m_objs[objectIndex];
			
		}
		else
		{
			hit = false;
		}
	}
	
	vec3 calcLightColorAt(vec3 p,vec3 n,vec3 v,Material mat,vec3 pToLight)
	{
		vec3 seenP, normal, tangent;
		Object *obj;
		bool hit;
		
		int testCount = 50;
		float hitCount = testCount;
		
		for (int i=0;i<testCount;++i)
		{
			float ratio = i/float(testCount-1);
			float displacement = ratio*ratio*350;
			
			vec3 rvec = vec3(rand()%1001-500,rand()%1001-500,rand()%1001-500).projToPlane(pToLight).sgn() * displacement;
			
			calcVecs(Ray(p,pToLight + rvec),seenP,normal,tangent,hit,obj);
			
			if (hit)
			{
				if ((seenP-p).LENGTH() < (pToLight + rvec).LENGTH())
					--hitCount;
			}			
		}
		
		float dotp = max(0.f,n.dot(pToLight.sgn()));
		
		float specDot = max(0.f,(-v.reflectOn(n)).dot(pToLight.sgn()));
		
		vec3 color = mat.dif_color + dotp * mat.amb_color + mat.spc_color * pow(specDot,mat.spc_exp);
	
		
		return color * hitCount/testCount / 1.5;
	}
	
	vec3 calcColorAt(vec3 p,vec3 n,vec3 v,Material mat)
	{
		vec3 pToLight1 = (vec3(0,400,-100)-p);
		vec3 pToLight2 = (vec3(-300,600,1000)-p);
		
		return calcLightColorAt(p,n,v,mat,pToLight1) + calcLightColorAt(p,n,v,mat,pToLight2);
	}
	
	vec3 recCalcColor(Ray r,int depth,vector<bool> &insideObjs)
	{
		vec3 seenP, normal, tangent;
		Object *obj;
		Material mat;
		bool hit;
		
		calcVecs(r,seenP,normal,tangent,hit,obj);
		
		vec3 color;
		
		if (hit && depth != 0)
		{
			mat = obj->mat;
			
			vec3 refl = -r.v.reflectOn(normal);
			
			color += calcColorAt(seenP,normal,r.v,mat);
			
			if (mat.ref_color.LENGTH())
			{
				Ray reflRay = Ray(seenP,refl);
				color += recCalcColor(reflRay,depth-1,insideObjs) * mat.ref_color;				
			}
			
			if (mat.refrac)
			{
				float relDensity = mat.density;
				
				if (insideObjs[obj->id]) relDensity = 1.f / relDensity;
				
				insideObjs[obj->id] = !insideObjs[obj->id];
				
				float sinIn  = std::sqrt(1-std::pow((-r.v).dot(normal),2.f));
				float sinOut = sinIn * relDensity;
				float cosOut = std::sqrt(1-std::pow(sinOut,2.f));
				
				Ray refrRay = Ray(seenP,(-normal*cosOut + -tangent*sinOut));
				color += recCalcColor(refrRay,depth-1,insideObjs) * mat.refrac;	
			}
		}
		
		return vec4(color,1);
	}
	
	vec4 calcColor(Ray r)
	{
		vector<bool> insideBuf(m_objs.size(),false);
		
		return vec4(recCalcColor(r,5,insideBuf),1);
	}
};

int main()
{
	srand(42);
	
	BallRenderer renderer(vec2(1000,1000));
	
	renderer.addObject(new BallRenderer::Sphere(vec3(-300,-200,1200),200,BallRenderer::Material(vec3(.1,.1,.1),vec3(.3,.5,.8),vec3(1,1,1),vec3(.7),142)));															
	renderer.addObject(new BallRenderer::Sphere(vec3( 200, 250,1400),400,BallRenderer::Material(vec3(.1,.1,.1),vec3(.8,.2,.1),vec3(1,1,1),vec3(.8),15)));
	renderer.addObject(new BallRenderer::Sphere(vec3( 300,-350, 800),150,BallRenderer::Material(vec3(.3,.3,.3),vec3(.6,.6,.6),vec3(),vec3(),15)));
	//renderer.addObject(new BallRenderer::Sphere(vec3( 200, 250,1400),400,BallRenderer::Material(vec3(),vec3(),vec3(),vec3(),0,1,.45)));
	renderer.addObject(new BallRenderer::Plane( vec3(   0,-500,    0),vec3( 0, 1, 0).sgn(),BallRenderer::Material(vec3(.1,.2,.1),vec3(.4,.5,.4),vec3(),vec3(),0)));
	renderer.addObject(new BallRenderer::Plane( vec3( 700,   0,    0),vec3(-1, 0, 0).sgn(),BallRenderer::Material(vec3(.1,.1,.2),vec3(.4,.4,.5),vec3(),vec3(),0)));
	renderer.addObject(new BallRenderer::Plane( vec3(-500,   0,    0),vec3( 1, 0, 0).sgn(),BallRenderer::Material(vec3(.3,.1,.1),vec3(.4,.4,.4),vec3(),vec3(),0)));
	renderer.addObject(new BallRenderer::Plane( vec3(   0,   0, 2200),vec3( 0, 0,-1).sgn(),BallRenderer::Material(vec3(.2,.2,.1),vec3(.5,.5,.3),vec3(.8,.9,.8),vec3(),80)));
	renderer.addObject(new BallRenderer::Plane( vec3(   0, 800,    0),vec3( 0,-1, 0).sgn(),BallRenderer::Material(vec3(.1,.1,.1),vec3(.8,.8,.7),vec3(),vec3(),0)));
	renderer.addObject(new BallRenderer::Plane( vec3(   0,   0,-1500),vec3( 0, 0, 1).sgn(),BallRenderer::Material(vec3(.03,.05,.07),vec3(.2,.2,.2),vec3(),vec3(),0)));
	
	Image img = renderer.render();
	
	Result r = img.flipVertically().saveToFile("ball.png");
	
	if (!r)
	{
		cout << r << endl;
	}
}
