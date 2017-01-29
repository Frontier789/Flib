#ifndef HEX_PLOTTER_GUARD_INL
#define HEX_PLOTTER_GUARD_INL

template<class T>
inline void HexPlotter::load(const HexArray<T> &hex,Delegate<vec4,T> dataToColor)
{
	drawData.reset();
	
	vector<vec2> pts(hex.getArea() * getVertexCountPerHex());
	vector<vec4> clr(hex.getArea() * getVertexCountPerHex());
	vector<vec2> tpt(hex.getArea() * getVertexCountPerHex());

	vec2 *ptsPointer = &pts[0];
	vec4 *clrPointer = &clr[0];
	vec2 *tptPointer = &tpt[0];
	
	vec2 midpBase_i = pol2(1,deg(30));
	vec2 midpBase_j = pol2(1,deg(90));
	
	hex.forAll([&](const T &h,vec2i p){
		
		addHex(p.x * midpBase_i + p.y * midpBase_j,dataToColor(h),ptsPointer,clrPointer,tptPointer);
		
	});
	
	drawData.positions.set(&pts[0],pts.size());
	drawData.colors.set(&clr[0],clr.size());
	drawData.texPositions.set(&tpt[0],tpt.size());
	drawData.addDraw(0,pts.size(),Triangles);
}

#endif
