#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: canvas aspect ratio
uniform float in_aspectRatio;

// input: center
uniform vec2 in_center;

// input: length of the two sides
uniform vec2 in_sides;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: time
uniform float in_time;

// Gets the Texture Unit from the main function
uniform sampler2D in_texture0;

// Box signed distance function
float sdBox(in vec2 p, in vec2 b) {
  vec2 d = abs(p)-b;
  return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main() {
  float frequency = 500.0;
  float boundary_thickness = 0.005;
  float rate = -20.0;

  vec2 p = vert_texCoord-in_center;
  vec2 xy = vec2(p.x,p.y*in_aspectRatio);
  float xygrad = xy.x + xy.y;
  float d = sdBox(xy, in_sides);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  out_fragColor = vec4(col,1.0);

  // vec4 tex0_color = texture(in_texture0, vert_texCoord);
  
  // out_fragColor = vec4(tex0_color.r, tex0_color.g, tex0_color.b, tex0_color.w);
}