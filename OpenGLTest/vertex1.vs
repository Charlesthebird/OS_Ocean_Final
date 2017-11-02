#version 330 core
layout(location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout(location = 1) in vec3 aColor; // the color variable has attribute position 1
layout(location = 1) in vec2 atexCoord; // the color variable has attribute position 1
  
out vec3 ourColor; // output a color to the fragment shader
out vec2 texCoord;


out float time;
out vec4 pos;

uniform float atime;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
        //vec4(aPos.x + (sin(atime * aPos.y * 2) / 10.0)
        //, aPos.y + (sin(atime * aPos.x * 10) / 5.0), 
        //aPos.z, 
        //1.0);
    //ourColor = aColor; // set ourColor to the input color we got from the vertex data
    texCoord = atexCoord;
    time = atime;
    pos = gl_Position;
}