#version 330 core
out vec4 color;

in vec2 TexCoords;
uniform sampler2D material;

uniform vec2 pos1;

uniform vec2 pos2[10];
uniform float size2;

uniform vec2 n1;

void main()
{    
    color = vec4(texture(material, TexCoords).rgb, 0.9);

    for(int i = 0; i < size2; i++)
    {
    	vec2 uv = gl_FragCoord.xy - pos2[i];
    	float dist = dot(uv, uv);
    	color = (dist <= 3*3) ? mix(vec4(1, 0, 0, 1.0), vec4(.0, .0, .0, 1.0), smoothstep(2*2, 3*3, dist)) : color;
    }

    vec2 uv = gl_FragCoord.xy - pos1;
    float dist = dot(uv, uv);
    color = (dist <= 3*3) ? mix(vec4(0, 1, 0, 1.0), vec4(.0, .0, .0, 1.0), smoothstep(2*2, 3*3, dist)) : color;

    //vec2 n1 = vec2(0, 1);
    uv = normalize(uv);
    float cos = (dot(n1, uv));
    if(cos > 0.342)
    {
    	cos = min(0.9999, cos);
    	float sin = max(0, min(1, sqrt(1 - cos * cos)));
    	float t_cos = 0.342 * cos + 0.93969262 * sin;
    	if(t_cos * sqrt(dist) <= 5)
    		color = vec4(0, 1, 0, 1.0);
    }
}  