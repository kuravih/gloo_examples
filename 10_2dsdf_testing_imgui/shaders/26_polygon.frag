#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

// float sdPolygon(in vec2[N] v, in vec2 p) {
//   float d = dot(p-v[0],p-v[0]);
//   float s = 1.0;
//   for( int i=0, j=N-1; i<N; j=i, i++ ) {
//     vec2 e = v[j] - v[i];
//     vec2 w = p - v[i];
//     vec2 b = w - e*clamp( dot(w,e)/dot(e,e), 0.0, 1.0 );
//     d = min( d, dot(b,b) );
//     bvec3 c = bvec3(p.y>=v[i].y,p.y<v[j].y,e.x*w.y>e.y*w.x);
//     if( all(c) || all(not(c)) )
//       s*=-1.0;  
//   }
//   return s*sqrt(d);
// }

void main() {

  float frequency = 500.0;
  float line_thickness = 0.002;
  float boundary_thickness = 0.005;
  float center_radius = 0.005;
  float rate = -20.0;
  vec3 color_yellow = vec3(1.0,1.0,0.0);

  vec2 p = vert_texCoord - in_center;
  vec2 m = in_mouse - in_center;

  // float d = sdPolygon(p);

  // // coloring
  // vec3 col = (d>0.0) ? in_color1 : in_color2;
  // col *= 1.0 - exp(-6.0*abs(d));
  // col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  // col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  // d = sdPolygon(p);
  // col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  // col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  // out_fragColor = vec4(col,1.0);
  out_fragColor = vec4(1.0,1.0,1.0,1.0);
}