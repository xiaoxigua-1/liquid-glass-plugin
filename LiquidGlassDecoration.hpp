#pragma once

#include <hyprland/src/render/decorations/IHyprWindowDecoration.hpp>
#include <src/render/Framebuffer.hpp>

struct box {
  float x = 0, y = 0, w = 0, h = 0;

  //
  Vector2D middle() { return Vector2D{x + w / 2.0, y + h / 2.0}; }
};

class LiquidGlassDecoration : public IHyprWindowDecoration {
public:
  PHLWINDOWREF m_window;

  LiquidGlassDecoration(PHLWINDOW);
  virtual ~LiquidGlassDecoration() = default;

  virtual SDecorationPositioningInfo getPositioningInfo();

  virtual void onPositioningReply(const SDecorationPositioningReply &reply);

  virtual void draw(PHLMONITOR, float const &a);

  virtual eDecorationType getDecorationType();

  virtual void updateWindow(PHLWINDOW);

  virtual void damageEntire();

  virtual eDecorationLayer getDecorationLayer();

  virtual uint64_t getDecorationFlags();

  virtual std::string getDisplayName();

  void sampleBackground(CFramebuffer &sourceBF, CBox box);
  void renderPass(PHLMONITOR pMonitor, const float &a);

private:
  CFramebuffer m_sampleBF;
};
