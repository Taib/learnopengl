#version 330 core

out vec4 FragColor;  
in vec3 vertexColor;
in vec2 vertexUV;
  
uniform float time; // getting the glfw_time

uniform sampler2D mainTexture; // using a texture
uniform sampler2D secondTexture; // using a texture

void main()
{
    
    FragColor = vec4(vertexColor, 1.0); return;
    //FragColor = vec4(vertexUV, 0,1); // coloring with the uvs
     
    
	float sinTime = (sin(time) /2.0) - 0.5; // we vary the color in the range of [0.0 - 1.0] 
	float cosTime = (cos(time) /2.0) - 0.5; // we vary the color in the range of [0.0 - 1.0] 

    vec4 col1 = texture(mainTexture, vertexUV*vec2(pow(2, sinTime+1), pow(2, cosTime+1)) + vec2(cosTime, sinTime) ) ; // using a texture sampling and myColor
    vec4 col2 = texture(secondTexture, vertexUV); // using a texture sampling and myColor


    
    FragColor = mix(col1, col2, 0.0) ;// linear interpolation between two colors
}