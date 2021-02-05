#version 330 core

out vec4 FragColor;  
in vec3 vertexColor;
in vec2 vertexUV;
 
uniform vec4 myColor; // getting some random color

uniform sampler2D mainTexture; // using a texture
uniform sampler2D secondTexture; // using a texture

void main()
{
    
    FragColor = vec4(vertexColor, 1.0) * myColor; 
    FragColor = vec4(vertexUV, 0,1); // coloring with the uvs
    
    vec4 col1 = texture(mainTexture, vec2(vertexUV.x, 1-vertexUV.y)) * myColor; // using a texture sampling and myColor
    vec4 col2 = texture(secondTexture, vertexUV); // using a texture sampling and myColor


    
    FragColor = mix(col1, col2, 0.3) ;// linear interpolation between two colors
}