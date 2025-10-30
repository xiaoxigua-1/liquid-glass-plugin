#pragma once

#include "LiquidGlassDecoration.hpp"
#include <hyprutils/math/Box.hpp>
#include <hyprutils/math/Region.hpp>
#include <src/render/Framebuffer.hpp>
#include <src/render/pass/PassElement.hpp>

class LiquidGlassPassElemnet : public IPassElement {
public:
  struct LiquidGlassData {
    LiquidGlassDecoration *deco = nullptr;
    float a = 1.F;
  };

  LiquidGlassPassElemnet(const LiquidGlassData &_data);
  virtual ~LiquidGlassPassElemnet() = default;

  virtual void draw(const CRegion &damage);
  virtual bool needsLiveBlur();
  virtual bool needsPrecomputeBlur();
  virtual std::optional<CBox> boundingBox();

  virtual const char *passName() { return "LiquidGlassPassElemnet"; }

private:
  LiquidGlassData m_data;
};
