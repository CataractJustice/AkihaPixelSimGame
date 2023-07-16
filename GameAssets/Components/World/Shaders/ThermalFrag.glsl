#version 330 core

layout (location = 0) out float TemperatureOut;

in vec2 TexCoord;

uniform vec2 resolution;
uniform sampler2D thermalInfo[9];

vec4 thermalInfoAt(vec2 uv) 
{
	int thermalInfoIndex = 0;
	if(uv.x > 0.0f) thermalInfoIndex++;
	if(uv.x > 1.0f) thermalInfoIndex++;
	if(uv.y > 0.0f) thermalInfoIndex += 3;
	if(uv.y > 1.0f) thermalInfoIndex += 3;
	vec2 fuv = fract(uv);
	return texture(thermalInfo[thermalInfoIndex], uv);
}

void main()
{
    vec4 r = thermalInfoAt(TexCoord + vec2(1.0f / resolution.x, 0.0f));
    vec4 l = thermalInfoAt(TexCoord - vec2(1.0f / resolution.x, 0.0f));
    vec4 u = thermalInfoAt(TexCoord + vec2(0.0f, 1.0f / resolution.y));
    vec4 d = thermalInfoAt(TexCoord - vec2(0.0f, 1.0f / resolution.y));
    vec4 o = thermalInfoAt(TexCoord);

	float outTemp = o.r;
	outTemp += (r.r - o.r) * r.g * 0.25f;
	outTemp += (l.r - o.r) * l.g * 0.25f;
	outTemp += (u.r - o.r) * u.g * 0.25f;
	outTemp += (d.r - o.r) * d.g * 0.25f;

	TemperatureOut = outTemp;
}