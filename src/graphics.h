#pragma once

#include <vulkan/vulkan.h>
#include <glfw_window.h>
#include <vector>
#include <optional>

namespace veng {

class Graphics final {
 public:
  Graphics(gsl::not_null<Window*> window);
  ~Graphics();

 private:

  struct QueueFamilyIndicies {
    std::optional<std::uint32_t> graphics_family = std::nullopt;
    std::optional<std::uint32_t> presentation_family = std::nullopt;

    bool IsValid() const { return graphics_family.has_value() && presentation_family.has_value();}
  };

  struct SwapChainProperties {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;

    bool IsValid() const { return !formats.empty() && !present_modes.empty();}
  };

  void InitializeVulkan();
  void CreateInstance();
  void SetupDebugMessenger();
  void PickPhysicalDevice();
  void CreateLogicalDeviceAndQueues();
  void CreateSurface();
  void CreateSwapChain();

  std::vector<gsl::czstring> GetRequiredInstanceExtensions();

  static gsl::span<gsl::czstring> GetSuggestedInstanceExtensions();
  static std::vector<VkExtensionProperties> GetSupportedInstanceExtensions();
  static bool AreAllExtensionsSupported(gsl::span<gsl::czstring> extensions);

  static std::vector<VkLayerProperties> GetSupportedValidationlayers();
  static bool AreAllLayersSupported(gsl::span<gsl::czstring> extensions);

  QueueFamilyIndicies FindQueueFamilies(VkPhysicalDevice device);
  SwapChainProperties GetSwapChainProperties(VkPhysicalDevice device);
  bool IsDeviceSuitable(VkPhysicalDevice device);
  std::vector<VkPhysicalDevice> GetAvailableDevices();
  bool AreAllDeviceExtensionSupported(VkPhysicalDevice device);
  std::vector<VkExtensionProperties> GetDeviceAvailableExtensions(VkPhysicalDevice device);

  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(gsl::span<VkSurfaceFormatKHR> formats);
  VkPresentModeKHR ChooseSwapPresentMode(gsl::span<VkPresentModeKHR> modes);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);


  std::array<gsl::czstring, 1> required_device_extensions_ = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  VkInstance instance_ = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debug_messenger_;

  VkPhysicalDevice physcial_device_ = VK_NULL_HANDLE;
  VkDevice logical_device_ = VK_NULL_HANDLE;
  VkQueue graphics_queue_ = VK_NULL_HANDLE;
  VkQueue present_queue_ = VK_NULL_HANDLE;

  VkSurfaceKHR surface_ = VK_NULL_HANDLE;

  gsl::not_null<Window*> window_;
  bool validation_enabled_ = true;
};

}  // namespace veng