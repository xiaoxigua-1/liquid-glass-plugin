#include "logs.hpp"
#include "globals.hpp"
#include <src/debug/Log.hpp>
#include <src/helpers/Color.hpp>

void log(std::string message, CHyprColor color, float timeMs,
         bool notify = false) {
  std::string logMessage = std::format("[{}] {}", PLUGIN_INFO.name, message);

  Debug::log(eLogLevel::NONE, logMessage);
  if (notify)
    HyprlandAPI::addNotification(PHANDLE, logMessage, color, timeMs);
}

void Logs::error(std::string message, bool notify) {
  log(message, CHyprColor{0.863, 0.078, 0.235, 1.0}, 5000, notify);
}

void Logs::info(std::string message, bool notify) {
  log(message, CHyprColor{0.196, 0.804, 0.196, 1.0}, 5000, notify);
}
void Logs::warning(std::string message, bool notify) {
  log(message, CHyprColor{1.0, 0.647, 0.0, 1.0}, 5000, notify);
}
