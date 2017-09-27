#version 110

uniform sampler2D u_tex;
uniform float u_thickness;
uniform float u_pov;

varying vec2 va_texp;

float contour(in float d,in float w) 
{
    return smoothstep(0.5 - w, 0.5 + w, d * u_thickness * 2.0);
}

float samp(in vec2 uv,in float w)
{
    return contour(texture2D(u_tex, uv).a, w);
}


void main()
{
/*
	vec4 c = texture2D(u_tex,va_texp);
	vec4 c2 = texture2D(u_tex,va_texp+vec2(-0.02,-0.04));
	c.a = (c.a < u_thickness || c.a > u_thickness+0.2) ? 0.0 : 1.0;
	c2 = c2.a < u_thickness*1.7 ? vec4(0.0,0.0,1.0,c2.a/u_thickness/1.7) : vec4(0.0,0.0,1.0,1.0);
	gl_FragColor = c*c.a + c2*c2.a;
	*/


	vec4 c = texture2D(u_tex,va_texp);
	
    float dist = c.a;

	float width = pow(fwidth(dist),0.2 / u_pov);
    //float width = 0.7 * length(vec2(dFdx(dist), dFdy(dist)));

    float alpha = contour( dist, width );
    //float alpha = aastep( 0.5, dist );

    // ------- (comment this block out to get your original behavior)
    // Supersample, 4 extra points
    float dscale = 0.354; // half of 1/sqrt2; you can play with this
    vec2 duv = vec2(dscale) * (dFdx(va_texp) + dFdy(va_texp));
    vec4 box = vec4(va_texp-duv, va_texp+duv);

    float asum = samp( box.xy, width )
               + samp( box.zw, width )
               + samp( box.xw, width )
               + samp( box.zy, width );

    // weighted average, with 4 extra points having 0.5 weight each,
    // so 1 + 0.5*4 = 3 is the divisor
	alpha = (alpha + 0.4 * asum) / (1.0 + 0.4*4.0);

    // -------

	gl_FragColor = vec4(vec3(0.0), alpha);
}