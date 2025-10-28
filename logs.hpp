#pragma once

#include <string>

class Logs {
public:
  static void info(std::string message, bool notify = false);
  static void error(std::string message, bool notify = false);
  static void warning(std::string message, bool notify = false);
};
