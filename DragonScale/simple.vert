#version 330 core
layout( location = 0 ) in vec3 vPos_model;
layout( location = 1 ) in vec3 vColor;
uniform mat4 PROJ;
uniform mat4 MODELVIEW;

out vec3 fColor;

void main() {
	vec4 v = vec4( vPos_model, 1 );
	gl_Position = PROJ * MODELVIEW * v;

	fColor = vColor;
}