#version 330 core
layout (location = 0) in vec3 aPos;

uniform float aTime;
uniform float waveHeight;
uniform float waveSpeed;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out float time;

out vec4 localPos;
out vec4 fragPosView;
out vec4 fragPosWorld;


vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Value Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/lsf3WH
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ), 
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}
float getOceanYVal(float x, float z){ 
	float valueNoiseAmount = .2;
	float rollingWaveAmount = 1;
	float vnSlow = waveSpeed * .5;
	float vnHeight = waveHeight * 10;

	return (noise(vec2((x + aTime * vnSlow) * vnHeight,(z + aTime * vnSlow) * vnHeight)) ) * valueNoiseAmount + 
			((waveHeight * sin(waveSpeed * (x - aTime))) - 
			(waveHeight * cos(waveSpeed * (z - aTime)))) * rollingWaveAmount;
}

void main()
{
	// get the local position using the ocean function
	vec4 rawWorldPos = model * vec4(aPos, 1);
	localPos = vec4(aPos.x, getOceanYVal(rawWorldPos.x, rawWorldPos.z), aPos.z, 1.0f);

	// output the gl_Position by transforming the local position from local to world to view to clip space
    gl_Position = projection * view * model * localPos;
	
	// set other variables for the fragment shader to use
	fragPosView = projection * view * model * vec4(aPos, 1.0);
	fragPosWorld =  vec4(aPos.x, 
		aPos.y * aPos.y * (aPos.y / 10.0f),
		aPos.z, 1.0);
	time = aTime;
} 

