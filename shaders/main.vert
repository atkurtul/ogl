#version 330
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 tex;
layout (location = 5) in uvec4 id;
layout (location = 6) in vec4 wgt;

layout (location = 0) out vec2 tex_coord;
layout (location = 1) out vec4 pix_pos;
layout (location = 2) out vec4 pp_light;
layout (location = 3) out mat3 out_norm;

uniform mat4 prj;
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

	pix_pos = xform * bone_mat * vec4(pos, 1);
	pp_light = light_prj * light_view * pix_pos;
	gl_Position = prj * pix_pos;
	tex_coord = tex;

	mat3 xf = mat3(xform);
	vec3 T = normalize(xf * tangent);
    vec3 B = normalize(xf * bitangent);
    vec3 N = normalize(xf * normal);
	out_norm = mat3(T, B, N);
}
