
#version 330 core

out vec4 FragColor;

in float time;

in vec4 localPos;
in vec4 fragPosView;
in vec4 fragPosWorld;

void main()
{
    //FragColor = vec4( fragPosView.y * .1f, fragPosView.y * .2f, .5 + fragPosView.y * .5, 1.0);
    //FragColor = vec4(max(0, (fragPosView.y)) + fragPosWorld.y * .1f, 
		//max(0, (fragPosView.y * .01))+ .5 + fragPosWorld.y * .4f, 
		//max(0, (fragPosView.y * .03)) + .8 + (fragPosWorld.y * .5), 1.0);


	// get the ocean color
    vec4 oceanColor = vec4( .2 , 
		(localPos.y + .2) + .3 ,
		((localPos.y + .4) * 2) + .3, 
		1.0);


	// get the fog amount
	float fogDensity = .1f;
    const float LOG2 = 1.442695;
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = exp2(-fogDensity * // negative fog density
                        fogDensity * // fog density
                        z * z * LOG2);
    fogFactor = clamp(fogFactor, 0.0, 1.0);


	// output the final fragment color
	gl_FragColor = mix(vec4(.4, .7, .9, 1), oceanColor, fogFactor);
}  