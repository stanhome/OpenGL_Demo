#version 330

in vec2 TexCoordOut;

uniform sampler2D gShadowMap;

out vec4 FragColor;

void main()
{
	float depth = texture(gShadowMap, TexCoordOut).x;
	depth = 1.0 - (1.0 - depth) * 25.0;
	FragColor = vec4(depth);
}