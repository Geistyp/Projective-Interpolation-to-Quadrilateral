// fragment shader

#version 150

uniform sampler2D tex;
uniform sampler2D grid;

in vec3 varyingtexcoord;

out vec4 outputColor;

void main()
{
	//outputColor = vec4( varyingtexcoord.xy / varyingtexcoord.z, 0.0, 1.0);
	outputColor = texture2D(tex, varyingtexcoord.xy / varyingtexcoord.z);
	outputColor = mix(outputColor, texture2D(grid, varyingtexcoord.xy / varyingtexcoord.z), 0.5);
	//outputColor = vec4(1.0, 0.0, 0.0, 1.0);
}