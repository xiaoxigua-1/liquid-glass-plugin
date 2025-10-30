#include "LiquidGlassDecoration.hpp"
#include "globals.hpp"
#include "logs.hpp"
#include "shaders.h"

#include <GLES3/gl32.h>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/render/OpenGL.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <src/render/Shader.hpp>

std::string loadShader(const char *fileName) {
  std::string message;

  if (shaders.contains(fileName)) {
    message = std::format("Load shader {}", fileName);

    Logs::info(message);
    return shaders.at(fileName);
  }

  message = std::format("Couldn't load shader {}", fileName);
  Logs::error(message);
  throw std::runtime_error(message);
}

void initShader() {
  const char *shaderFile = "liquidGlass.frag";
  GLuint prog = g_pHyprOpenGL->createProgram(
      g_pHyprOpenGL->m_shaders->TEXVERTSRC, loadShader(shaderFile), true);

  if (prog == 0) {
    std::string message = std::format("Couldn't compile shader {}", shaderFile);
    Logs::error(message);
    throw std::runtime_error(message);
  }

  SHADER.program = prog;
  SHADER.uniformLocations[SHADER_PROJ] = glGetUniformLocation(prog, "proj");
  SHADER.uniformLocations[SHADER_POS_ATTRIB] = glGetAttribLocation(prog, "pos");
  SHADER.uniformLocations[SHADER_TEX_ATTRIB] =
      glGetAttribLocation(prog, "texcoord");
  SHADER.uniformLocations[SHADER_TEX] = glGetAttribLocation(prog, "tex");

  SHADER.uniformLocations[SHADER_RADIUS] = glGetAttribLocation(prog, "radius");
  SHADER.uniformLocations[SHADER_TOP_LEFT] =
      glGetAttribLocation(prog, "topLeft");
  SHADER.uniformLocations[SHADER_FULL_SIZE] =
      glGetAttribLocation(prog, "fullSize");

  SHADER.createVao();
}

void onNewWindow(void *self, std::any data) {
  const auto PWINDOW = std::any_cast<PHLWINDOW>(data);

  auto pDec = makeUnique<LiquidGlassDecoration>(PWINDOW);
  g_registeredDecorationPtrs.push_back(pDec.get());
  HyprlandAPI::addWindowDecoration(PHANDLE, PWINDOW, std::move(pDec));
}

APICALL EXPORT std::string PLUGIN_API_VERSION() { return HYPRLAND_API_VERSION; }

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
  PHANDLE = handle;

  const std::string HASH = __hyprland_api_get_hash();

  // ALWAYS add this to your plugins. It will prevent random crashes coming from
  // mismatched header versions.
  if (HASH != GIT_COMMIT_HASH) {
    Logs::error("Mismatched headers! Can't proceed.");
    throw std::runtime_error("[MyPlugin] Version mismatch");
  }

  initShader();

  static auto P = HyprlandAPI::registerCallbackDynamic(
      PHANDLE, "openWindow",
      [&](void *self, SCallbackInfo &info, std::any data) {
        onNewWindow(self, data);
      });

  for (auto &w : g_pCompositor->m_windows) {
    if (w->isHidden() || !w->m_isMapped)
      continue;

    auto pDec = makeUnique<LiquidGlassDecoration>(w);
    g_registeredDecorationPtrs.push_back(pDec.get());
    HyprlandAPI::addWindowDecoration(PHANDLE, w, std::move(pDec));
  }

  return PLUGIN_INFO;
}

APICALL EXPORT void PLUGIN_EXIT() {
  for (const auto &pDecPtr : g_registeredDecorationPtrs) {
    HyprlandAPI::removeWindowDecoration(PHANDLE, pDecPtr);
  };

  g_pHyprRenderer->m_renderPass.removeAllOfType("LiquidGlassPassElemnet");
  SHADER.destroy();
}
