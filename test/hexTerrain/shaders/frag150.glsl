#version 150

uniform int u_doGrid;

in vec4 frag_color;
in vec2 frag_subp;

void main()
{
	float scl = 1.0;
	
	if (u_doGrid == 1)
	{
		float lenV = length(frag_subp);
		float cosV = frag_subp.x / lenV;
		float val  = (cos(acos(cosV)*6.0) + 1.0)/2.0*0.06 + 0.32;
		
		scl = min(1.0 - (lenV - val)*3.5,1.0);
		scl = pow(scl,0.3);
	}
	
	gl_FragColor = vec4(vec3(frag_color)*scl,1.0);
}
