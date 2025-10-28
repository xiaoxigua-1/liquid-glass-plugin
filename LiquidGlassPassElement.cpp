#include "LiquidGlassPassElement.hpp"
#include <src/render/OpenGL.hpp>

LiquidGlassPassElemnet::LiquidGlassPassElemnet(const LiquidGlassData &data)
    : m_data(data) {}

void LiquidGlassPassElemnet::draw(const CRegion &damage) {
  m_data.deco->renderPass(g_pHyprOpenGL->m_renderData.pMonitor.lock(),
                          m_data.a);
}

bool LiquidGlassPassElemnet::needsLiveBlur() { return false; }

bool LiquidGlassPassElemnet::needsPrecomputeBlur() { return false; }
