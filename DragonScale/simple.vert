#version 330 core
layout( location = 0 ) in vec3 vPos_model;

void main() {
	gl_Position.xyz = vPos_model;
	gl_Position.w = 1.0;
}