#ifndef _GLOO_IMGUI_WINDOW_H_
#define _GLOO_IMGUI_WINDOW_H_

#include "gloo/Texture.h"
#include "gloo/Shader.h"
#include "gloo/Program.h"
#include "gloo/Mesh.h"
#include "gloo/Framebuffer.h"
#include "gloo/Renderbuffer.h"

#include "AppData.h"
// ====================================================================================================================





// ====================================================================================================================
// screen vertices ----------------------------------------------------------------------------------------------------
#define SCREEN_VERTICES { gloo::Vertex{glm::vec3(-1.0f,  1.0f, 0.0f),    glm::vec3(0),    glm::vec3(0),    glm::vec2(0.0f, 0.0f)}, \
                          gloo::Vertex{glm::vec3( 1.0f,  1.0f, 0.0f),    glm::vec3(0),    glm::vec3(0),    glm::vec2(1.0f, 0.0f)}, \
                          gloo::Vertex{glm::vec3( 1.0f, -1.0f, 0.0f),    glm::vec3(0),    glm::vec3(0),    glm::vec2(1.0f, 1.0f)}, \
                          gloo::Vertex{glm::vec3(-1.0f, -1.0f, 0.0f),    glm::vec3(0),    glm::vec3(0),    glm::vec2(0.0f, 1.0f)}}
// screen indices -----------------------------------------------------------------------------------------------------
#define SCREEN_INDICES { 0, 1, 2, \
                         2, 3, 0 }
// ====================================================================================================================





// ====================================================================================================================
namespace ImGui {
// ====================================================================================================================





  // ==================================================================================================================
  void ResizeContraint(ImGuiSizeCallbackData* _data);
  // ==================================================================================================================





  // ==================================================================================================================
  class BaseWindow { // abstract
  private:
    static uint COUNT; // global
    const uint m_windowIndex;
    bool* m_pShow;
    bool m_isWindowInitialized;
  protected:
    float windowHeight;
  public:
    BaseWindow(bool& _show) : m_windowIndex(COUNT), m_pShow(&_show), m_isWindowInitialized(false) {
      INCREMENT_COUNT();
    };
    ~BaseWindow() = default;
    void Show(AppData& _appData);
    virtual void Initialize(AppData& _appData) = 0;
    virtual void Draw(AppData& _appData) = 0;
    virtual void Deinitialize(AppData& _appData) = 0;
    const uint getWindowIndex() const {
      return m_windowIndex;
    }
    virtual const std::string getWindowTitle() const = 0;
    static const void RESET_COUNT() {
      COUNT = 0;
    }
    static const void INCREMENT_COUNT() {
      COUNT++;
    }
  };
  // ==================================================================================================================





  // ==================================================================================================================
  class ImageWindow : public BaseWindow {
    gloo::Program imageShaderProgram;
    gloo::Mesh screenMesh;
    ImVec2 viewSize, viewWindowSize;
    // common parameters
    glm::vec3 in_color1, in_color2;
    glm::vec2 in_mouse;
    // box parameters
    glm::vec2 in_box_center, in_box_sides;
    // render variables
    gloo::Texture renderTex;
    gloo::Framebuffer framebuffer;
    gloo::Renderbuffer renderbuffer;
  public:
    ImageWindow(bool& _show);
    ~ImageWindow();
    const std::string getWindowTitle() const override {
      return "Box Window##"+std::to_string(getWindowIndex());
    }
    void Initialize(AppData& _appData) override;
    void Draw(AppData& _appData) override;
    void Deinitialize(AppData& _appData) override;
  };
  // ==================================================================================================================


  // ==================================================================================================================
}
// ====================================================================================================================





// ====================================================================================================================
#endif // _GLOO_IMGUI_WINDOW_H_
