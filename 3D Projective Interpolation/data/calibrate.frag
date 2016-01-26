// fragment shader
// http://www.reedbeta.com/blog/2012/05/26/quadrilateral-interpolation-part-1/
#version 450 core

uniform sampler2D tex;

in vec3 varyingtexcoord;

out vec4 outputColor;

void main()
{
	outputColor = texture2D(tex, varyingtexcoord.xy/varyingtexcoord.z);

}