#version 330 core
layout( location = 0 ) in vec3 vPos_model;
uniform mat4 MVP;

void main() {
	vec4 v = vec4( vPos_model, 1 );
	gl_Position = MVP * v;
}