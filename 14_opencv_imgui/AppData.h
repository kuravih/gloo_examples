#ifndef _GLOO_APPDATA_H_
#define _GLOO_APPDATA_H_

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <opencv2/opencv.hpp>

#include <string>

#define IMAGE_WIDTH 480
#define IMAGE_HEIGHT 640

struct AppData {
  struct UI {
    cv::Mat imageA, imageB;
    std::string information;
    bool showImageAWindow, showImageBWindow;
  } ui;
};


#endif // _GLOO_APPDATA_H_