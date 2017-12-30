#version 330 core

out vec4 FragColor;

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
	return 1.0-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(dist,dist)*4.0);
}

void main(){
	vec2 u_resolution = vec2(100, 100);
	vec2 st = gl_FragCoord.xy/u_resolution.xy;

	vec3 color = vec3(circle(st,0.9));

	gl_FragColor = vec4( color, 1.000 );
	//gl_FragColor = vec4(1.0,0.0,1.0,1.0);
}
