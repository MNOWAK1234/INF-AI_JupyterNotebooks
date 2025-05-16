#version 330

// Uniform variables
uniform sampler2D textureMap0;

// Output variable
out vec4 pixelColor;

// Interpolated variables
in vec4 ic;
in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;
in vec2 iTexCoord0;

void main(void) {
    // Normalize the interpolated vectors
    vec4 ml1 = normalize(l1); // Normalized vector to light source 1
    vec4 ml2 = normalize(l2); // Normalized vector to light source 2
    vec4 mn = normalize(n);   // Normalized normal vector
    vec4 mv = normalize(v);   // Normalized vector to the observer
    
    // Calculate the reflected vectors
    vec4 mr1 = reflect(-ml1, mn); // Reflected vector for light source 1
    vec4 mr2 = reflect(-ml2, mn); // Reflected vector for light source 2
    
    // Surface parameters
    vec4 kd = texture(textureMap0, iTexCoord0); // Diffuse texture color
    
    // Calculate the lighting model for light source 1
    float nl1 = clamp(dot(mn, ml1), 0, 1);                         // Diffuse component for light source 1
    float rv1 = pow(clamp(dot(mr1, mv), 0, 1), 25);                // Specular component for light source 1
    
    // Calculate the lighting model for light source 2
    float nl2 = clamp(dot(mn, ml2), 0, 1);                         // Diffuse component for light source 2
    float rv2 = pow(clamp(dot(mr2, mv), 0, 1), 25);                // Specular component for light source 2
    
    // Calculate the final pixel color by combining the lighting models for both light sources
    pixelColor = vec4(kd.rgb * (nl1 + nl2), kd.a);
}
