#version 330

layout (location = 0) in vec3 aPos;

uniform mat4 PVM;

void main(){
	gl_Position = PVM * vec4(aPos, 1.0);
}