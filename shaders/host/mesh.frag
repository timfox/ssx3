#version 450

layout(location = 0) in vec3 frag_normal;
layout(location = 1) in vec3 frag_color;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 n = normalize(frag_normal);
    vec3 light = normalize(vec3(0.35, 0.85, 0.4));
    float ndl = clamp(dot(n, light), 0.15, 1.0);
    vec3 ambient = vec3(0.12, 0.14, 0.18);
    vec3 lit = frag_color * ndl + ambient;
    out_color = vec4(lit, 1.0);
}
