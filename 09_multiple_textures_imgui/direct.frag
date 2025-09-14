#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D in_texture0;
uniform sampler2D in_texture1;
uniform sampler2D in_texture2;

void main() {
  // out_fragColor = texture(in_texture0, vert_texCoord);
  vec4 tex0_color = texture(in_texture0, vert_texCoord);
  vec4 tex1_color = texture(in_texture1, vert_texCoord);
  vec4 tex2_color = texture(in_texture2, vert_texCoord);
  out_fragColor = vec4(tex0_color.r+tex1_color.r+(1-tex2_color.r), tex0_color.g+tex1_color.r+(1-tex2_color.r), tex0_color.b+tex1_color.r+(1-tex2_color.r), tex0_color.w);
}