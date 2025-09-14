#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: dimensions
uniform vec2 in_dims;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

float dot2( vec2 v ) { return dot(v,v); }


float sdTunnel(in vec2 p, in vec2 wh) {
  p.x = abs(p.x); p.y = -p.y;
  vec2 q = p - wh;

  float d1 = dot2(vec2(max(q.x,0.0),q.y));
  q.x = (p.y>0.0) ? q.x : length(p)-wh.x;
  float d2 = dot2(vec2(q.x,max(q.y,0.0)));
  float d = sqrt( min(d1,d2) );
  
  return (max(q.x,q.y)<0.0) ? -d : d;
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

  float d = sdTunnel(p, in_dims);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdTunnel(p, in_dims);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}