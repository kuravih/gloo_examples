#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: canvas aspect ratio
uniform float in_aspectRatio;

// input: center
uniform vec2 in_center;

// input: radius
uniform float in_radius;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: time
uniform float in_time;

// Gets the Texture Unit from the main function
uniform sampler2D in_texture0;

// Circle signed distance function
float sdCircle(in vec2 p, in float radius) {
  return length(p)-radius;
}

void main() {
  float frequency = 500.0;
  float boundary_thickness = 0.005;
  float rate = -20.0;

  vec2 p = vert_texCoord-in_center;
  vec2 xy = vec2(p.x,p.y*in_aspectRatio);
  float xygrad = xy.x + xy.y;
  float d = sdCircle(xy, in_radius);

  float zebra = 0.8 + 0.2*cos(frequency*xygrad + rate*in_time);
  float ripple = 0.8 + 0.2*cos(frequency*d + rate*in_time);
  
  // coloring
  vec3 col = (d>0.0) ? in_color1*ripple : in_color2*zebra;
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  out_fragColor = vec4(col,1.0);

  // vec4 tex0_color = texture(in_texture0, vert_texCoord);

  // out_fragColor = vec4(tex0_color.r, tex0_color.g, tex0_color.b, tex0_color.w);
}