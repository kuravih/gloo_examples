#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: dimensions
uniform float in_length1;
uniform float in_length2;
uniform float in_height;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

float dot2(in vec2 v ) {
  return dot(v,v);
}

// Trapezoid signed distance function
float sdTrapezoid(in vec2 p, in float r1, in float r2, in float he) {
  vec2 k1 = vec2(r2,he);
  vec2 k2 = vec2(r2-r1,2.0*he);
  p.x = abs(p.x);
  vec2 ca = vec2(p.x-min(p.x,(p.y<0.0)?r1:r2), abs(p.y)-he);
  vec2 cb = p - k1 + k2*clamp( dot(k1-p,k2)/dot2(k2), 0.0, 1.0 );
  float s = (cb.x<0.0 && ca.y<0.0) ? -1.0 : 1.0;
  return s*sqrt(min(dot2(ca),dot2(cb)));
}

void main() {
  float frequency = 500.0;
  float line_thickness = 0.002;
  float boundary_thickness = 0.005;
  float center_radius = 0.005;
  float rate = -20.0;
  vec3 color_yellow = vec3(1.0,1.0,0.0);

  vec2 p = vert_texCoord-in_center;
  vec2 m = in_mouse-in_center;

  float d = sdTrapezoid(p, in_length1, in_length2, in_height);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdTrapezoid(m, in_length1, in_length2, in_height);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}