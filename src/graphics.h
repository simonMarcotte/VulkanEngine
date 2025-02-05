#pragma once

#include <vulkan/vulkan.h>
#include <glfw_window.h>
#include <vector>

namespace veng {

class Graphics final {
 public:
  Graphics(gsl::not_null<Window*> window);
  ~Graphics();

 private:
  void InitializeVulkan();
  void CreateInstance();
  static gsl::span<gsl::czstring> GetSuggestedInstanceExtensions();
  static std::vector<VkExtensionProperties> GetSupportedInstanceExtensions();
  static bool AreAllExtensionsSupported(gsl::span<gsl::czstring> extensions);

  VkInstance instance_;
  gsl::not_null<Window*> window_;
};

}  // namespace veng