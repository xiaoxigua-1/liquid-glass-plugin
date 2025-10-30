#include "LiquidGlassPassElement.hpp"
#include <src/desktop/Window.hpp>
#include <src/render/OpenGL.hpp>

LiquidGlassPassElemnet::LiquidGlassPassElemnet(const LiquidGlassData &data)
    : m_data(data) {}

void LiquidGlassPassElemnet::draw(const CRegion &damage) {
  m_data.deco->renderPass(g_pHyprOpenGL->m_renderData.pMonitor.lock(),
                          m_data.a);
}

std::optional<CBox> LiquidGlassPassElemnet::boundingBox() {
  const auto PWINDOW = m_data.deco->m_window;
  const auto PWINDOWWORKSPACE = PWINDOW->m_workspace;
  auto surfaceBox = PWINDOW->getWindowMainSurfaceBox();

  if (PWINDOWWORKSPACE && PWINDOWWORKSPACE->m_renderOffset->isBeingAnimated() &&
      !PWINDOW->m_pinned)
    surfaceBox.translate(PWINDOWWORKSPACE->m_renderOffset->value());
  surfaceBox.translate(PWINDOW->m_floatingOffset);

  return surfaceBox;
}

bool LiquidGlassPassElemnet::needsLiveBlur() { return false; }

bool LiquidGlassPassElemnet::needsPrecomputeBlur() { return false; }
