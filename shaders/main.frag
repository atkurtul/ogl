#version 330
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec2 tex_coord;
layout (location = 1) in vec4 pix_pos;
layout (location = 2) in vec4 pp_light;
layout (location = 3) in mat3 normal_mat;
out vec4 out_color;

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D normal_map;
layout(binding = 2) uniform sampler2D metalic;
layout(binding = 3) uniform sampler2D shadow_map;

uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform mat4 light_view;
uniform mat4 light_prj;

uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 ambient;
uniform vec3 emissive;

float calculate_shadow(vec3 normal) {
    vec3 coords = pp_light.xyz / pp_light.w * 0.5 + 0.5;
    if(coords.z > 1.0) return 0;
    vec3 dir = normalize(light_pos - pix_pos.xyz);
    float bias = max(0.05 * (1.0 - dot(normal, dir)), 0.005);
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf_depth = texture(shadow_map, coords.xy + vec2(x, y) * texel_size).r; 
            shadow += coords.z - bias > pcf_depth  ? 1.0 : 0.0;        
        }    
    }
   return shadow / 9;
}

vec4 to_linear(vec4 sRGB) {
    bvec4 cutoff = lessThan(sRGB, vec4(0.04045));
    vec4 higher = pow((sRGB + vec4(0.055))/vec4(1.055), vec4(2.4));
    vec4 lower = sRGB/vec4(12.92);
    return mix(higher, lower, cutoff);
}

void main() {           
    vec3 color = texture(albedo, tex_coord).rgb;
    vec3 normal = to_linear(texture(normal_map, tex_coord)).xyz;
    normal = normalize(normal_mat * normal);
    vec3 light_dir = normalize(light_pos - pix_pos.xyz);
    float diff = max(dot(light_dir, normal), 0.0);

    vec3 view_dir = normalize(eye_pos - pix_pos.xyz);
    float spec = pow(max(dot(normal, normalize(light_dir + view_dir)), 0.0), 64.0);

    float shadow = calculate_shadow(normal);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diff + spec)) * color;    
    
    out_color = vec4(lighting, 1.0);
}
