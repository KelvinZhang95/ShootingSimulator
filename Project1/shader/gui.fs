#version 330 core
in float f_circle;

out vec4 FragColor;
uniform float ox;
uniform float oy;

void main(){
	//FragColor = vec4(0.0, 0.0, 0.0, 0.5);
	if(f_circle < 0.1)
	{
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		vec2 uv = gl_FragCoord.xy - vec2(ox, oy);
  		float dist =  sqrt(dot(uv, uv));

  		FragColor = mix(vec4(.90, .90, .90, 0.0), vec4(.0, .0, .0, 1.0), smoothstep(399.0, 401.0, dist));

  		//FragColor = (dist < min(ox, oy)) ? vec4(0.0, 0.0, 0.0, 0.0) : vec4(0.0, 0.0, 0.0, 1.0);
	}
}