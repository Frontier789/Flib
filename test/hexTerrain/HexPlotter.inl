#ifndef HEX_PLOTTER_GUARD_INL
#define HEX_PLOTTER_GUARD_INL

#include <vector>

template<class T>
inline void HexPlotter::load(const HexArray<T> &hex,Delegate<vec4,T> dataToColor)
{
	drawData.reset();
	
	std::vector<vec2> pts(hex.getArea() * getVertexCountPerHex());
	std::vector<vec4> clr(hex.getArea() * getVertexCountPerHex());
	std::vector<vec2> tpt(hex.getArea() * getVertexCountPerHex());
	std::vector<vec3> nrm(hex.getArea() * getVertexCountPerHex());

	vec2 *ptsPointer = &pts[0];
	vec4 *clrPointer = &clr[0];
	vec2 *tptPointer = &tpt[0];
	vec3 *nrmPointer = &nrm[0];
	
	hex.forAll([&](const T &h,vec2i p){
		
		addHex(hex.getDrawPos(p),dataToColor(h),hex.getNormal(p),ptsPointer,clrPointer,tptPointer,nrmPointer);
		
	});
	
	drawData.positions.set(&pts[0],pts.size());
	drawData.colors.set(&clr[0],clr.size());
	drawData.normals.set(&nrm[0],nrm.size());
	
	if (!useGeomShader) drawData.texPositions.set(&tpt[0],tpt.size());
	
	drawData.addDraw(0,pts.size(),useGeomShader ? Points : Triangles);
}

#endif
