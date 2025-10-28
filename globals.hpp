#pragma once

#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/render/Shader.hpp>
#include <vector>

inline const PLUGIN_DESCRIPTION_INFO PLUGIN_INFO = {"Liquid-glass-plugin", "",
                                                    "xiaoxigua-1", "0.1"};
inline HANDLE PHANDLE = nullptr;
inline std::vector<IHyprWindowDecoration *> g_registeredDecorationPtrs;
inline SShader SHADER;
