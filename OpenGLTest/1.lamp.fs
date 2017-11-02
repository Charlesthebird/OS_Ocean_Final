#version 330 core
out vec4 fragColor;  
//in vec3 ourColor;
in vec2 texCoord;
in float time;
in vec4 pos;


uniform vec3 lightColor;

uniform sampler2D ourTexture;
uniform vec4 addColor;

uniform sampler2D texture1;     // wood 
uniform sampler2D texture2;     // face

void main()
{
    vec2 coord = texCoord;//vec2(time + (sin(texCoord.y * 20) / 40) + texCoord.x, (sin(time * 1) * 2) + texCoord.y);
    vec4 color = mix(texture(texture1, vec2(coord.x /* 2.0f*/, coord.y)),          // wood 
                    texture(texture2, vec2(1 - (coord.x), (coord.y))),  //face 
                                                                        /*sin(time * 5) + */.3f)
                    //* vec4(ourColor, 1.0) 
                    + (.2 * addColor);
    const float LOG2 = 1.442695;
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = exp2(-.03 * // negative fog density
                        .03 * // fog density
                        z * z * LOG2);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    gl_FragColor = mix(vec4(.5, .6, .8, 1), color * lightColor, fogFactor);

}
