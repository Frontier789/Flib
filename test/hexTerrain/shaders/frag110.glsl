#version 110

uniform int u_doGrid;

varying vec4 va_color;
varying vec2 va_texpos;

void main()
{
	float scl = 1.0;
	
	if (u_doGrid == 1)
	{
		float lenV = length(va_texpos);
		float cosV = va_texpos.x / lenV;
		float val  = (cos(acos(cosV)*6.0) + 1.0)/2.0*0.06 + 0.32;
		
		scl = min(1.0 - (lenV - val)*3.5,1.0);
		scl = pow(scl,0.3);
	}
	
	gl_FragColor = vec4(vec3(va_color)*scl,1.0);
}