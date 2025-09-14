#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: points along the box
uniform vec2 in_point1;
uniform vec2 in_point2;

// input: width of the box
uniform float in_thickness;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

// Oriented box signed distance function
float sdOrientedBox(in vec2 p, in vec2 a, in vec2 b, float th) {
  float l = length(b-a);
  vec2 d = (b-a)/l;
  vec2 q = (p-(a+b)*0.5);
  q = mat2(d.x,-d.y,d.y,d.x)*q;
  q = abs(q)-vec2(l,th)*0.5;
  return length(max(q,0.0)) + min(max(q.x,q.y),0.0);    
}


void main() {

  float frequency = 500.0;
  float line_thickness = 0.002;
  float boundary_thickness = 0.005;
  float center_radius = 0.005;
  float rate = -20.0;
  vec3 color_yellow = vec3(1.0,1.0,0.0);

  vec2 p = vert_texCoord;
  vec2 m = in_mouse;

  float d = sdOrientedBox(p, in_point1, in_point2, in_thickness);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdOrientedBox(m, in_point1, in_point2, in_thickness);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}