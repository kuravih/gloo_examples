#include "ImguiMouse.h"
#include "iostream"

ImGui::Mouse::Mouse() {}

bool ImGui::Mouse::Inputs(ImVec2& _viewSize) {
  ImGuiWindow* window = GetCurrentWindow();
  ImVec2 origin = window->DC.CursorPos;
  InvisibleButton("arcball", _viewSize);

  if (IsItemClicked()) {
    clicked = true;
    changed = false;
    start = GetMousePos();
  }

  if (clicked && IsMouseReleased(0)) {
    clicked = false;
  }

  if (clicked && IsMouseDragging(0) && IsItemHovered()) {
    mouse = GetMousePos();
    position.x = mouse.x - origin.x;
    position.y = _viewSize.y - (mouse.y - origin.y);
    changed = true;
    start = mouse;
  }

  SetCursorScreenPos(origin);

  return changed;
}
