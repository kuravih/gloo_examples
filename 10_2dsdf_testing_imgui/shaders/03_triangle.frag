#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: the 3 points of the triangle
uniform vec2 in_point1;
uniform vec2 in_point2;
uniform vec2 in_point3;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

// Triangle signed distance function
float sdTriangle( in vec2 p, in vec2 p0, in vec2 p1, in vec2 p2 ) {
  vec2 e0 = p1 - p0;
  vec2 e1 = p2 - p1;
  vec2 e2 = p0 - p2;

  vec2 v0 = p - p0;
  vec2 v1 = p - p1;
  vec2 v2 = p - p2;

  vec2 pq0 = v0 - e0*clamp( dot(v0,e0)/dot(e0,e0), 0.0, 1.0 );
  vec2 pq1 = v1 - e1*clamp( dot(v1,e1)/dot(e1,e1), 0.0, 1.0 );
  vec2 pq2 = v2 - e2*clamp( dot(v2,e2)/dot(e2,e2), 0.0, 1.0 );
    
  float s = e0.x*e2.y - e0.y*e2.x;
  vec2 d = min( min( vec2( dot( pq0, pq0 ), s*(v0.x*e0.y-v0.y*e0.x) ), vec2( dot( pq1, pq1 ), s*(v1.x*e1.y-v1.y*e1.x) )), vec2( dot( pq2, pq2 ), s*(v2.x*e2.y-v2.y*e2.x) ));

  return -sqrt(d.x)*sign(d.y);
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

  float d = sdTriangle(p, in_point1, in_point2, in_point3);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdTriangle(m, in_point1, in_point2, in_point3);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}