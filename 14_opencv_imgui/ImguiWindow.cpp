#include "ImguiWindow.h"
#include "kato/function.hpp"
// ====================================================================================================================





// ====================================================================================================================
void ImGui::ResizeContraint(ImGuiSizeCallbackData* _data) {
  float _height = (float)(int)(intptr_t)_data->UserData;
  _data->DesiredSize.y = _height;
}
// ====================================================================================================================





// ==== BaseWindow show =============================================================================================== 
void ImGui::BaseWindow::Show(AppData& _appData) {
  if (*m_pShow && !m_isWindowInitialized) {
    Initialize(_appData);
    m_isWindowInitialized = true;
  }
  if (m_isWindowInitialized) {
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 0), ImVec2(FLT_MAX, FLT_MAX), ImGui::ResizeContraint, (void*)(intptr_t)windowHeight);
    if (ImGui::Begin(getWindowTitle().c_str(), m_pShow)) {
      Draw(_appData);
    }
    ImGui::End();
  }
  if (m_isWindowInitialized && !*m_pShow) {
    Deinitialize(_appData);
    m_isWindowInitialized = false;
  }
}
// ==== BaseWindow ====================================================================================================





// ==== ImageWindow ===================================================================================================
ImGui::ImageWindow::ImageWindow(bool& _show) : BaseWindow(_show), imageShaderProgram(gloo::Shader(gloo::getFileContents("./shaders/plane.vert"), gloo::Shader::Type::Vertex), gloo::Shader(gloo::getFileContents("./shaders/box.frag"), gloo::Shader::Type::Fragment)), screenMesh(std::vector<gloo::Vertex>(SCREEN_VERTICES), std::vector<GLuint>(SCREEN_INDICES)), viewSize(640,480), viewWindowSize(viewSize.x+14,viewSize.y+14), in_color1(0.90,0.60,0.30), in_color2(0.65,0.85,1.00), in_box_center(0.5,0.5), in_box_sides(0.25,0.25), renderTex(viewSize.x, viewSize.y, gloo::Texture::Type::UnsignedByte, gloo::Texture::InternalFormat::RGBA, gloo::Texture::Format::RGBA, gloo::Texture::Slot::slot00, gloo::Texture::Target::Texture2D) {
  screenMesh.LinkPositionToLocation(0);
  screenMesh.LinkTextureUVToLocation(1);
  gloo::Framebuffer::UNBIND();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ImGui::ImageWindow::Initialize(AppData& _appData) {
  imageShaderProgram.Uniform("in_aspectRatio", renderTex.aspectRatio);
  imageShaderProgram.Uniform("in_sides", in_box_sides);
  imageShaderProgram.Uniform("in_center", in_box_center);
  imageShaderProgram.Uniform("in_time", (float)ImGui::GetTime());
  imageShaderProgram.Uniform("in_color1", in_color1);
  imageShaderProgram.Uniform("in_color2", in_color2);
  framebuffer.Bind();
  renderbuffer.RenderToTexture(renderTex);
  gloo::Framebuffer::UNBIND();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ImGui::ImageWindow::Draw(AppData& _appData) {
  static float zoom=1.0, zoomMin=1.0, zoomMax=8.0;
  float box_sides[2] = {in_box_sides.x, in_box_sides.y};
  float box_center[2] = {in_box_center.x, in_box_center.y};
  ImVec4 color1 = {in_color1.x, in_color1.y, in_color1.z, 1.0};
  ImVec4 color2 = {in_color2.x, in_color2.y, in_color2.z, 1.0};

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  framebuffer.Bind();
  glViewport(0,0,renderTex.width,renderTex.height);
  imageShaderProgram.Uniform("in_sides", in_box_sides);
  imageShaderProgram.Uniform("in_center", in_box_center);
  imageShaderProgram.Uniform("in_time", (float)ImGui::GetTime());
  imageShaderProgram.Uniform("in_color1", in_color1);
  imageShaderProgram.Uniform("in_color2", in_color2);
  screenMesh.Draw();
  gloo::Framebuffer::UNBIND();
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // ---- sides slider2 -----------------------------------------------------------------------------------------------
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Sides     ");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(300);
  if (ImGui::SliderFloat2("##box_sides", box_sides, 0.0f, 1.0f)) {
    in_box_sides.x = box_sides[0];
    in_box_sides.y = box_sides[1];
  }
  // ---- sides slider2 -----------------------------------------------------------------------------------------------

  // ---- center slider2 ----------------------------------------------------------------------------------------------
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Center    ");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(300);
  if (ImGui::SliderFloat2("##box_center", box_center, 0.0f, 1.0f)) {
    in_box_center.x = box_center[0];
    in_box_center.y = box_center[1];
  }
  // ---- center slider2 ---------------------------------------------------------------------------------------------

  // ---- zoom slider -------------------------------------------------------------------------------------------------
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Zoom      ");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(300);
  ImGui::SliderScalar("##box_zoom", ImGuiDataType_Float, &zoom, &zoomMin, &zoomMax, "%.2f X");
  // ---- zoom slider -------------------------------------------------------------------------------------------------

  // ---- 2 color pickers  --------------------------------------------------------------------------------------------
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Colors    ");
  ImGui::SameLine();
  if (ImGui::ColorEdit4("##box_color1", (float*)&color1, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar)) {
    in_color1.x = color1.x;
    in_color1.y = color1.y;
    in_color1.z = color1.z;
  }
  ImGui::SameLine();
  if (ImGui::ColorEdit4("##box_color2", (float*)&color2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar)) {
    in_color2.x = color2.x;
    in_color2.y = color2.y;
    in_color2.z = color2.z;
  }
  // ---- 2 color pickers  --------------------------------------------------------------------------------------------

  viewWindowSize.x = ImGui::GetContentRegionAvail().x;
  viewSize.x = viewWindowSize.x - 14;
  viewSize.y = (int)(viewSize.x*renderTex.aspectRatio);
  viewWindowSize.y = viewSize.y + 14;

  viewSize.x = zoom*viewSize.x;
  viewSize.y = zoom*viewSize.y;

  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6,6));
  ImGui::BeginChild("box_tex_view", viewWindowSize, true, ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar);
  renderTex.Update();
  ImGui::Image((ImTextureID)(GLuint)renderTex, viewSize);
  ImGui::EndChild();
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();

  windowHeight = ImGui::GetCursorPosY()+8;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ImGui::ImageWindow::Deinitialize(AppData& _appData) {
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ImGui::ImageWindow::~ImageWindow() {
  // imageShaderProgram.Delete();
  // screenMesh.Delete();
  // renderTex.Delete();
}
// ==== ImageWindow ===================================================================================================