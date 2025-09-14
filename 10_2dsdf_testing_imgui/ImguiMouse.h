#ifndef _GLOO_IMGUI_MOUSE_H_
#define _GLOO_IMGUI_MOUSE_H_

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <glm/glm.hpp>

namespace ImGui {

  class Mouse {
  public:
    ImVec2 start, mouse, position;
    bool clicked, changed;
    Mouse();
    IMGUI_API bool Inputs(ImVec2& _viewSize);
  };

}

#endif // _GLOO_IMGUI_MOUSE_H_