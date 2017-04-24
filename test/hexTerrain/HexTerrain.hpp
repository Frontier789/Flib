#ifndef HEX_TERRAIN_GUARD_HPP
#define HEX_TERRAIN_GUARD_HPP

#include <Frontier.hpp>

#include "HexArray.hpp"
#include "HexPlotter.hpp"

class HexTerrain : public Drawable
{
public:
	HexArray<float> data;
	HexPlotter plotter;
	int colorStyle;

	class TriangleFiller
	{
	public:
		vec2i Ap,Bp,Cp;
		float mxDif;
		float difScale;
		
		TriangleFiller(vec2i Ap,vec2i Bp,vec2i Cp,float mxDif = 0.5,float difScale = 0.8);
		
		void fill(HexTerrain &ter);
		void extendToTwice();
	};
	
	deque<TriangleFiller> triangleFillers;
	
	HexTerrain(int size,bool useGeomShader);
	
	void gen();
	void extendToTwice();
	void reduceToHalf();
	
	void setColorStyle(int style);
	void loadColors();
	
	void onDraw(ShaderManager &shader) override;
	void onUpdate(const fm::Time &dt) override;
};

#endif
