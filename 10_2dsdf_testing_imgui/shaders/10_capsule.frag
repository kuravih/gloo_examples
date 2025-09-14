#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: points
uniform vec2 in_point1;
uniform vec2 in_point2;

// input: end cap radii
uniform float in_radius1;
uniform float in_radius2;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

float cro(in vec2 a, in vec2 b ) {
  return a.x*b.y - a.y*b.x;
}

// Uneven Capsule signed distance function
float sdUnevenCapsule(in vec2 p, in vec2 pa, in vec2 pb, in float ra, in float rb) {
  p  -= pa;
  pb -= pa;
  float h = dot(pb,pb);
  vec2  q = vec2( dot(p,vec2(pb.y,-pb.x)), dot(p,pb) )/h;

  q.x = abs(q.x);
  
  float b = ra-rb;
  vec2  c = vec2(sqrt(h-b*b),b);
  
  float k = cro(c,q);
  float m = dot(c,q);
  float n = dot(q,q);
    
  if( k < 0.0 )
    return sqrt(h*(n)) - ra;
  else if( k > c.x )
    return sqrt(h*(n+1.0-2.0*q.y)) - rb;
  return m - ra;
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

  float d = sdUnevenCapsule(p, in_point1, in_point2, in_radius1, in_radius2);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdUnevenCapsule(m, in_point1, in_point2, in_radius1, in_radius2);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}