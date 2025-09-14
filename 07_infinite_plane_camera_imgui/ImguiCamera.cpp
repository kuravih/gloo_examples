#include "ImguiCamera.h"
#include "iostream"

ImGui::Camera::Camera(glm::vec3 _position, glm::vec3 _lookat, glm::vec3 _up, float _fovy_deg, float _aspect, float _near, float _far) : gloo::Camera(_position, _lookat, _up, _fovy_deg, _aspect, _near, _far) {}

void ImGui::Camera::Inputs(ImVec2& _viewSize) {
  ImGuiWindow* window = GetCurrentWindow();
  ImVec2 origin = window->DC.CursorPos;
  InvisibleButton("arcball", _viewSize);

  if (IsItemClicked()) {
    clicked = true;
    start = GetMousePos();
  }

  if (clicked && IsMouseReleased(0)) {
    clicked = false;
  }

  if (clicked && IsMouseDragging(0)) {
    mouse = GetMousePos();
    ImVec2 rot = ImMul(sensitivity, mouse-start);
    glm::vec3 newPosition = glm::rotate(position, glm::radians(-rot.y), glm::normalize(glm::cross(position, up)));
    position = glm::rotate(newPosition, glm::radians(-rot.x), up);
    start = mouse;
  }

  Update();

  SetCursorScreenPos(origin);
}