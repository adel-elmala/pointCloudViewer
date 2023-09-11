#version 330 core


in vec3 theColor;		// Color value came from the vertex shader (smoothed) 
out vec4 FragColor;	    // Color that will be used for the fragment


float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
//   FragColor =  vec4(theColor.xyz, 1.0f);
   FragColor = vec4(1.0f,0.0f,0.0f, 1.0f);   // Add alpha value of 1.0.
}


