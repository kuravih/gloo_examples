#ifndef _GLOO_APPDATA_H_
#define _GLOO_APPDATA_H_

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <string>

struct AppData {
  struct UI {
    std::string information;
    bool showBoxWindow, showCircleWindow;
  } ui;
};


#endif // _GLOO_APPDATA_H_