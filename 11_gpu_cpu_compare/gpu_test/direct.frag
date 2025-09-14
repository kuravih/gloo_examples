#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: radius
uniform float in_radius;

// Gets the Texture Unit from the main function
uniform sampler2D in_image; // image

// Circle signed distance function
float sdCircle(in vec2 center, in float radius) {
  return length(center)-radius;
}

vec3 color_balck = vec3(0.0,0.0,0.0);
vec3 color_white = vec3(1.0,1.0,1.0);

void main() {
  vec2 p = vert_texCoord-in_center;
  float d = sdCircle(p, in_radius);
  vec4 image_color = texture(in_image, vert_texCoord);
  vec3 mask_color = (d>0.0) ? color_balck : color_white;
  vec3 gradient_color = vec3(vert_texCoord.x,vert_texCoord.x,vert_texCoord.x);
  out_fragColor = vec4(image_color.r+gradient_color.r+(1-mask_color.r), image_color.g+gradient_color.r+(1-mask_color.r), image_color.b+gradient_color.r+(1-mask_color.r), 1.0);
}