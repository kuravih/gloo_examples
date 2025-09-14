#version 330 core

// Near point calculated in vertex shader
in vec3 vert_nearPoint;
// Far point calculated in vertex shader
in vec3 vert_farPoint;
// View matrix from the vertex shader
in mat4 vert_view;
// Projection matrix from the vertex shader
in mat4 vert_projection;

// output: colors in RGBA
out vec4 out_fragColor;

float near = 0.01;
float far = 0.5;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
  vec2 coord = fragPos3D.xz * scale;
  vec2 derivative = fwidth(coord);
  vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
  float line = min(grid.x, grid.y);
  float minimumz = min(derivative.y, 1);
  float minimumx = min(derivative.x, 1);
  vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

  // z axis
  if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
    color.z = 1.0;

  // x axis
  if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
    color.x = 1.0;

  return color;
}

float computeDepth(vec3 pos) {
  vec4 clip_space_pos = vert_projection * vert_view * vec4(pos.xyz, 1.0);
  return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos) {
  vec4 clip_space_pos = vert_projection * vert_view * vec4(pos.xyz, 1.0);
  float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
  float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
  return linearDepth / far; // normalize
}

void main() {
  float t = -vert_nearPoint.y / (vert_farPoint.y - vert_nearPoint.y);
  vec3 fragPos3D = vert_nearPoint + t * (vert_farPoint - vert_nearPoint);

  // gl_FragDepth = computeDepth(gl_Position); // orginal
  gl_FragDepth = ((gl_DepthRange.diff * computeDepth(fragPos3D)) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

  float linearDepth = computeLinearDepth(fragPos3D);
  float fading = max(0, (0.5 - linearDepth));

  out_fragColor = (grid(fragPos3D, 10, true) + grid(fragPos3D, 1, true))* float(t > 0); // adding multiple resolution for the grid
  out_fragColor.a *= fading;
}