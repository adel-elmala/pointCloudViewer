
// Sets the position and color of a vertex.
//   This implementations just copies the position with no transformations.
//   It copies the color to "theColor" so that the fragment shader can access it.

#beginglsl vertexshader vertexShader_PosColorOnly
#version 330 core
layout (location = 0) in vec3 vertPos;	   // Position in attribute location 0
//layout (location = 1) in vec3 vertColor;   // Color in attribute location 1
//out vec3 theColor;					       // Output a color to the fragment shader

uniform float time;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453 * time);
}

void main()
{
   // model -> world -> camera -> projection -> NDC
   gl_Position = vec4( -1.0 + (2* rand(vertPos.xy)),
                       -1.0 + (2* rand(vertPos.yz)),
                       -1.0 + (2* rand(vertPos.zx)),
                       -1.0 + (2* rand(vertPos.xz)));
   
//   gl_Position = vec4(vertPos.x, vertPos.y, 0.5f, 1.0);
//   theColor = vertColor;
}
#endglsl

// Set a general color using a fragment shader. (A "fragment" is a "pixel".)
//    The color value is passed in, obtained from the color(s) on the vertice(s).
//    Color values range from 0.0 to 1.0.
//    First three values are Red/Green/Blue (RGB).
//    Fourth color value (alpha) is 1.0, meaning there is no transparency.

#beginglsl FragmentShader fragmentShader_ColorOnly
#version 330 core
//in vec3 theColor;		// Color value came from the vertex shader (smoothed) 
out vec4 FragColor;	    // Color that will be used for the fragment
void main()
{
   FragColor = vec4(1.0f,0.0f,0.0f, 1.0f);   // Add alpha value of 1.0.
}
#endglsl

