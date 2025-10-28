#include "LiquidGlassDecoration.hpp"
#include "LiquidGlassPassElement.hpp"
#include "globals.hpp"
#include "logs.hpp"
#include <GLES3/gl32.h>
#include <hyprutils/math/Misc.hpp>
#include <hyprutils/math/Region.hpp>
#include <src/desktop/Window.hpp>
#include <src/render/Framebuffer.hpp>
#include <src/render/OpenGL.hpp>
#include <src/render/Renderer.hpp>
#include <src/render/Shader.hpp>

LiquidGlassDecoration::LiquidGlassDecoration(PHLWINDOW pWindow)
    : IHyprWindowDecoration(pWindow), m_window(pWindow) {
  Logs::info("Decoration Init", true);
}

eDecorationLayer LiquidGlassDecoration::getDecorationLayer() {
  return DECORATION_LAYER_UNDER;
}

uint64_t LiquidGlassDecoration::getDecorationFlags() {
  return DECORATION_NON_SOLID;
}

eDecorationType LiquidGlassDecoration::getDecorationType() {
  return eDecorationType::DECORATION_CUSTOM;
}

std::string LiquidGlassDecoration::getDisplayName() { return "LiquidGlass"; }

SDecorationPositioningInfo LiquidGlassDecoration::getPositioningInfo() {
  SDecorationPositioningInfo info;

  info.priority = 10000;
  info.policy = DECORATION_POSITION_ABSOLUTE;
  info.desiredExtents = {{0, 0}, {0, 0}};

  return info;
}

void LiquidGlassDecoration::onPositioningReply(
    const SDecorationPositioningReply &reply) {}

void LiquidGlassDecoration::draw(PHLMONITOR pMonitor, float const &a) {
  LiquidGlassPassElemnet::LiquidGlassData data{this, a};

  g_pHyprRenderer->m_renderPass.add(makeUnique<LiquidGlassPassElemnet>(data));
}

void LiquidGlassDecoration::renderPass(PHLMONITOR pMonitor, const float &a) {
  const auto PWINDOW = m_window.lock();
  const auto PWORKSPACE = m_window->m_workspace;

  const auto WORKSPACEOFFSET = PWORKSPACE && !m_window->m_pinned
                                   ? PWORKSPACE->m_renderOffset->value()
                                   : Vector2D();
  const auto SOURCE = g_pHyprOpenGL->m_renderData.currentFB;

  auto thisbox = PWINDOW->getWindowMainSurfaceBox();

  CBox wlrbox =
      thisbox.translate(WORKSPACEOFFSET)
          .translate(-pMonitor->m_position + m_window->m_floatingOffset)
          .scale(pMonitor->m_scale)
          .round();
  CBox transformBox = wlrbox;

  const auto TR = wlTransformToHyprutils(
      invertTransform(g_pHyprOpenGL->m_renderData.pMonitor->m_transform));
  transformBox.transform(
      TR, g_pHyprOpenGL->m_renderData.pMonitor->m_transformedSize.x,
      g_pHyprOpenGL->m_renderData.pMonitor->m_transformedSize.y);

  sampleBackground(*SOURCE, transformBox);
  liquidGlassShader(m_sampleBF, *SOURCE, wlrbox);
}

void LiquidGlassDecoration::liquidGlassShader(CFramebuffer &sourceBF,
                                              CFramebuffer &targetBF,
                                              CBox box) {
  const auto TR = wlTransformToHyprutils(
      invertTransform(g_pHyprOpenGL->m_renderData.pMonitor->m_transform));

  Mat3x3 matrix = g_pHyprOpenGL->m_renderData.monitorProjection.projectBox(
      box, TR, box.rot);
  Mat3x3 glMatrix =
      g_pHyprOpenGL->m_renderData.projection.copy().multiply(matrix);
  auto tex = sourceBF.getTexture();

  glMatrix.transpose();
  glBindFramebuffer(GL_FRAMEBUFFER, targetBF.getFBID());
  glActiveTexture(GL_TEXTURE0);
  tex->bind();
  g_pHyprOpenGL->useProgram(SHADER.program);

  SHADER.setUniformMatrix3fv(SHADER_PROJ, 1, GL_FALSE, glMatrix.getMatrix());
  SHADER.setUniformInt(SHADER_TEX, 0);

  glBindVertexArray(SHADER.uniformLocations[SHADER_SHADER_VAO]);
  g_pHyprOpenGL->scissor(box);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  g_pHyprOpenGL->scissor(nullptr);
}

void LiquidGlassDecoration::sampleBackground(CFramebuffer &sourceBF, CBox box) {
  if (m_sampleBF.m_size.x != box.width || m_sampleBF.m_size.y != box.height) {
    m_sampleBF.alloc(box.width, box.height, sourceBF.m_drmFormat);
  }

  int x0 = box.x;
  int x1 = box.x + box.width;
  int y0 = box.y;
  int y1 = box.y + box.height;

  glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceBF.getFBID());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_sampleBF.getFBID());

  glBlitFramebuffer(x0, y0, x1, y1, 0, 0, box.width, box.height,
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void LiquidGlassDecoration::updateWindow(PHLWINDOW pWindow) {
  g_pDecorationPositioner->repositionDeco(this);
}

void LiquidGlassDecoration::damageEntire() {
  const auto PWINDOWWORKSPACE = m_window->m_workspace;
  auto surfaceBox = m_window->getWindowMainSurfaceBox();

  if (PWINDOWWORKSPACE && PWINDOWWORKSPACE->m_renderOffset->isBeingAnimated() &&
      !m_window->m_pinned)
    surfaceBox.translate(PWINDOWWORKSPACE->m_renderOffset->value());
  surfaceBox.translate(m_window->m_floatingOffset);

  g_pHyprRenderer->damageBox(surfaceBox);
}
