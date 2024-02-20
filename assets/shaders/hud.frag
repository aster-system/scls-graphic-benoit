#version 330 core

in vec2 tex_pos;

out vec4 FragColor;

uniform vec4 border_color;
uniform vec4 border_width;
uniform sampler2D texture_0;

void main()
{
	vec2 texture_pos = tex_pos;
	vec4 color = vec4(0, 0, 0, 1);
	
	if(tex_pos.x < border_width.x || tex_pos.x > 1.0 - border_width.z || tex_pos.y < border_width.y || tex_pos.y > 1.0 - border_width.w)
	{
		color = border_color;
	}
	else
	{
		texture_pos.x -= border_width.x;
		texture_pos.y -= border_width.y;
		texture_pos.x /= 1.0 - (border_width.x + border_width.z);
		texture_pos.y /= 1.0 - (border_width.y + border_width.w);
		
		color = texture(texture_0, texture_pos);
	}
	
	FragColor = color;
}