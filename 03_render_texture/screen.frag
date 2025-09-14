#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: texture unit from main program
uniform sampler2D in_texture;

void main() {
  out_fragColor = texture(in_texture, vert_texCoord);
}