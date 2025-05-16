#version 330

// Uniform variables
uniform mat4 P; // Projection matrix
uniform mat4 V; // View matrix
uniform mat4 M; // Model matrix

// Vertex attributes
in vec4 vertex;   // Vertex position in model space
in vec4 color;    // Vertex color
in vec4 normal;   // Vertex normal vector in model space
in vec2 texCoord0; // Texture coordinates

// Interpolated variables
out vec4 ic;       // Interpolated color
out vec4 l1;       // Vector to light source 1 in eye space
out vec4 l2;       // Vector to light source 2 in eye space
out vec4 n;        // Normal vector in eye space
out vec4 v;        // Vector to the observer in eye space
out vec2 iTexCoord0;// Interpolated texture coordinates

void main(void) {
    vec4 lp1 = vec4(-100, 100, -100, 1); // Light source 1 position in world space
    vec4 lp2 = vec4(100, 100, 100, 1); // Light source 2 position in world space
    
    // Transform light source positions to eye space
    vec4 lp1_eye = V * lp1;
    vec4 lp2_eye = V * lp2;
    
    // Calculate vectors to light sources in eye space
    l1 = normalize(lp1_eye - V * M * vertex);
    l2 = normalize(lp2_eye - V * M * vertex);
    
    // Transform normal vector to eye space
    n = normalize(V * M * normal);
    
    // Calculate vector to the observer in eye space
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);
    
    // Pass other variables to the fragment shader
    ic = color;
    iTexCoord0 = texCoord0;
    
    // Calculate the final vertex position
    gl_Position = P * V * M * vertex;
}