#version 330 core

// output: colors in RGBA
out vec4 out_fragColor;

// input: texture coordinates from the Vertex Shader
in vec2 vert_texCoord;

// input: center
uniform vec2 in_center;

// input: dimensions
uniform float in_width;
uniform float in_height;
uniform float in_skewness;

// input: colors
uniform vec3 in_color1;
uniform vec3 in_color2;

// input: mouse cursor position
uniform vec2 in_mouse;

// input: time
uniform float in_time;

#define METHOD 1

// Parallelogram 2D signed distance function
float sdParallelogram_1(in vec2 p, in float wi, in float he, in float sk) {
  vec2  e  = vec2(sk,he);
  float e2 = sk*sk + he*he;

  p = (p.y<0.0)?-p:p;
  // horizontal edge
  vec2  w = p - e; w.x -= clamp(w.x,-wi,wi);
  vec2  d = vec2(dot(w,w), -w.y);
  // vertical edge
  float s = p.x*e.y - p.y*e.x;
  p = (s<0.0)?-p:p;
  vec2  v = p - vec2(wi,0); v -= e*clamp(dot(v,e)/e2,-1.0,1.0);
  d = min( d, vec2(dot(v,v), wi*he-abs(s)));
  return sqrt(d.x)*sign(-d.y);
}

// Parallelogram 2D signed distance function
float sdParallelogram_2(in vec2 p, in float wi, in float he, in float sk) {
  vec2  e  = vec2(sk,he);
  float e2 = sk*sk + he*he;

  float da = abs(p.x*e.y-p.y*e.x)-wi*he;
  float db = abs(p.y)-e.y;
  if( max(da,db)<0.0 ) {
    return max( da*inversesqrt(e2), db );
  } else {
    float f = clamp(p.y/e.y,-1.0,1.0);
    float g = clamp(p.x-e.x*f, -wi, wi);
    float h = clamp(((p.x-g)*e.x+p.y*e.y)/e2,-1.0,1.0);
    return length(p-vec2(g+e.x*h,e.y*h));
  }
}

// Parallelogram 2D signed distance function
float sdParallelogram_3(in vec2 p, in float wi, in float he, in float sk) {
  // above
  float db = abs(p.y)-he;
  if( db>0.0 && abs(p.x-sk*sign(p.y))<wi )
    return db;
    
  // inside
  float e2 = sk*sk + he*he;
  float h  = p.x*he - p.y*sk;
  float da = (abs(h)-wi*he)*inversesqrt(e2);
  if( da<0.0 && db<0.0 )
    return max( da, db );

  // sides
  vec2 q = (h<0.0)?-p:p; q.x -= wi;
  float v = abs(q.x*sk+q.y*he);
  if( v<e2 )
    return da;
  
  // exterior
  return sqrt( dot(q,q)+e2-2.0*v );
}

float sdParallelogram(in vec2 p, in float wi, in float he, in float sk) {
  #if METHOD==1
  return sdParallelogram_1(p,wi,he,sk);
  #endif
  #if METHOD==2
  return sdParallelogram_2(p,wi,he,sk);
  #endif
  #if METHOD==3
  return sdParallelogram_3(p,wi,he,sk);
  #endif
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

  float d = sdParallelogram(p, in_width, in_height, in_skewness);

  // coloring
  vec3 col = (d>0.0) ? in_color1 : in_color2;
  col *= 1.0 - exp(-6.0*abs(d));
  col *= 0.8 + 0.2*cos(frequency*d + rate*in_time);
  col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,boundary_thickness,abs(d)) );

  d = sdParallelogram(m, in_width, in_height, in_skewness);
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, abs(length(p-m)-abs(d))-line_thickness));
  col = mix(col, color_yellow, 1.0-smoothstep(0.0, line_thickness, length(p-m)-center_radius));

  out_fragColor = vec4(col,1.0);
}