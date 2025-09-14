#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: radii
uniform float in_radius1;
uniform float in_radius2;

// input: distance between the circles
uniform float in_distance;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

float sdCrescent(vec2 p, float d, float ra, float rb) {
  p.y = abs(p.y);

  float a = (ra*ra - rb*rb + d*d)/(2.0*d);
  float b = sqrt(max(ra*ra-a*a,0.0));
  if(d*(p.x*b-p.y*a) > d*d*max(b-p.y,0.0))
    return length(p-vec2(a,b));

  return max( (length(p)-ra),-(length(p-vec2(d,0))-rb));
}


void main() {

  float frequency = 500.0;
  float line_thickness = 0.002;
  float boundary_thickness = 0.005;
  float center_radius = 0.005;
  float rate = -20.0;
  vec3 color_yellow = vec3(1.0,1.0,0.0);

  vec2 p = vert_texCoord - in_center;
  vec2 m = in_mouse - in_center;

  float d = sdCrescent(p, in_distance, in_radius1, in_radius2);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdCrescent(p, in_distance, in_radius1, in_radius2);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}