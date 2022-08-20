#type VERTEX

#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_TexCoord = a_TexCoord;
}

#type FRAGMENT

#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoord;
out vec4 FragColor;

void main() 
{
	FragColor = texture(u_Texture, v_TexCoord);
}