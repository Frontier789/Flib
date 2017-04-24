#ifndef HEX_PLOTTER_GUARD_HPP
#define HEX_PLOTTER_GUARD_HPP

#include <Frontier.hpp>
#include "HexArray.hpp"

class HexPlotter : public Drawable
{
public:
	DrawData drawData;
	bool useGeomShader;
	
	HexPlotter(bool useGeomShader);
	
	template<class T>
	void load(const HexArray<T> &hex,Delegate<vec4,T> dataToColor);
	
	void onDraw(ShaderManager &shader) override;
	void onUpdate(const fm::Time &dt) override;
	fm::Size getVertexCountPerHex() const;

	void addHex(vec2 midp,vec4 color,vec3 normal,vec2 *&pts,vec4 *&clr,vec2 *&tpt,vec3 *&nrm);
};

#endif

#include "HexPlotter.inl"
