#version 330 core
layout (location = 0) in vec3 vertPos;	   // Position in attribute location 0
//layout (location = 1) in vec3 vertColor;   // Color in attribute location 1
out vec3 theColor;					       // Output a color to the fragment shader

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
   // model -> world -> camera -> projection -> NDC
//   gl_Position = vec4( -1.0 + (2* rand(vertPos.xy)),
//                       -1.0 + (2* rand(vertPos.yz)),
//                       -1.0 + (2* rand(vertPos.zx)),
//                       -1.0 + (2* rand(vertPos.xz)));
   
gl_Position = proj*view*model*vec4(vertPos.xyz, 1.0);

  theColor = vertPos;
}