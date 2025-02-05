#pragma once

namespace veng {

struct GlfwInitilization {
 public:
  GlfwInitilization();
  ~GlfwInitilization();

  GlfwInitilization(const GlfwInitilization&) = delete;
  GlfwInitilization& operator=(const GlfwInitilization&) = delete;
};
}  // namespace veng
