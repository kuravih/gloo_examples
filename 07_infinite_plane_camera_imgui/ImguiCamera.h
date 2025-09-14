#ifndef _GLOO_IMGUI_CAMERA_H_
#define _GLOO_IMGUI_CAMERA_H_

#include "gloo/Camera.h"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

namespace ImGui {

  class Camera: public gloo::Camera {
  public:
    ImVec2 start, mouse, sensitivity = {0.1f, 0.1f};
    bool clicked;
    Camera(glm::vec3 _position, glm::vec3 _lookat, glm::vec3 _up, float _fovy_deg=45.0f, float _aspect=1.0f, float _near=0.1f, float _far=100.0f);
    IMGUI_API void Inputs(ImVec2& _viewSize);
  };

}

#endif // _GLOO_IMGUI_CAMERA_H_