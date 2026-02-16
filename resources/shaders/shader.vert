#version 450

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projview;
} ubo;

layout(push_constant) uniform Push {
	mat4 model;
} push;

void main() {
	fragColor = color;
	gl_Position = ubo.projview * push.model * vec4(v_pos, 1.0f);
}