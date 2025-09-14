#version 330 core

// Vertex positions(coordinates) from main program
layout (location = 0) in vec3 in_position;

// Near point calculated in vertex shader
out vec3 vert_nearPoint;
// Far point calculated in vertex shader
out vec3 vert_farPoint;
// View matrix in vertex shader
out mat4 vert_view;
// Projection matrix in vertex shader
out mat4 vert_projection;

// Matrices needed for 3D viewing with perspective from main program
uniform mat4 in_view;
uniform mat4 in_projection;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
  mat4 viewInv = inverse(view);
  mat4 projectionInv = inverse(projection);
  vec4 unprojectedPoint = viewInv * projectionInv * vec4(x, y, z, 1.0);
  return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
  gl_Position = vec4(in_position, 1.0); // using directly the clipped coordinates
  vert_nearPoint = UnprojectPoint(in_position.x, in_position.y, 0.0, in_view, in_projection).xyz; // unprojecting on the near plane
  vert_farPoint = UnprojectPoint(in_position.x, in_position.y, 1.0, in_view, in_projection).xyz; // unprojecting on the far plane
  vert_view = in_view;
  vert_projection = in_projection;
}
