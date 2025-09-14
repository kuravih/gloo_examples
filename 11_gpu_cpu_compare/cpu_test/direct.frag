#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D in_image;
uniform sampler2D in_gradient;
uniform sampler2D in_mask;

void main() {
  vec4 image_color = texture(in_image, vert_texCoord);
  vec4 gradient_color = texture(in_gradient, vert_texCoord);
  vec4 mask_color = texture(in_mask, vert_texCoord);
  out_fragColor = vec4(image_color.r+gradient_color.r+(1-mask_color.r), image_color.g+gradient_color.r+(1-mask_color.r), image_color.b+gradient_color.r+(1-mask_color.r), 1.0);
}