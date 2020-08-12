#version 330

layout (location = 0) in vec3 pos;
layout (location = 5) in uvec4 id;
layout (location = 6) in vec4 wgt;

uniform mat4 xform;
uniform mat4 light_view;
uniform mat4 light_prj;

uniform mat4 bones[128];

uniform bool animated;

void main()
{
	mat4 bone_mat = mat4(1);
	if(animated)
		bone_mat = bones[id.x] * wgt.x +  bones[id.y] * wgt.y + bones[id.z] * wgt.z + bones[id.w] * wgt.w;

  	gl_Position = light_prj *  light_view * xform *  bone_mat * vec4(pos, 1);
}
