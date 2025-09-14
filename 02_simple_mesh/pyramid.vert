#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 in_position;
// Texture Coordinates
layout (location = 1) in vec2 in_texCoord;

// Outputs the texture coordinates to the fragment shader
out vec2 vert_texCoord;

// Inputs the matrices needed for 3D viewing with perspective
uniform mat4 in_model;
uniform mat4 in_view;
uniform mat4 in_projection;

void main() {
  // Outputs the positions/coordinates of all vertices
  gl_Position = in_projection * in_view * in_model * vec4(in_position, 1.0);
  // Assigns the texture coordinates from the Vertex Data to "vert_texCoord"
  vert_texCoord = in_texCoord;
}