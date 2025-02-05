#include <precomp.h>
#include <graphics.h>
#include <GLFW/glfw3.h>

namespace veng {

Graphics::Graphics(gsl::not_null<Window*> window) : window_(window) {
  InitializeVulkan();
}

Graphics::~Graphics() {
}

void Graphics::InitializeVulkan() {
  CreateInstance();
}

void Graphics::CreateInstance() {
  gsl::span<gsl::czstring> suggested_extensions = Graphics::GetSuggestedInstanceExtensions();

  if (!AreAllExtensionsSupported(suggested_extensions)){
    std::exit(EXIT_FAILURE);
  }

  

  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = nullptr;
  app_info.pApplicationName = "Course";
  app_info.pEngineName = "VEng";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instance_creation_info = {};
  instance_creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_creation_info.pNext = nullptr;
  instance_creation_info.pApplicationInfo = &app_info;
  instance_creation_info.enabledExtensionCount = suggested_extensions.size();
  instance_creation_info.ppEnabledExtensionNames = suggested_extensions.data();
  instance_creation_info.enabledLayerCount = 0;

  VkResult result = vkCreateInstance(&instance_creation_info, nullptr, &instance_);

  if (result != VK_SUCCESS) {
    std::exit(EXIT_FAILURE);
  }
}

gsl::span<gsl::czstring> Graphics::GetSuggestedInstanceExtensions() {
  std::uint32_t glfwExtensionCount = 0;
  gsl::czstring* glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  return {glfwExtensions, glfwExtensionCount};
}

std::vector<VkExtensionProperties> Graphics::GetSupportedInstanceExtensions() {
  
  std::uint32_t count;
  vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

  if (count == 0){
    return {};
  }

  std::vector<VkExtensionProperties> properties(count);
  vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());
  return properties;
}

bool Graphics::AreAllExtensionsSupported(gsl::span<gsl::czstring> extensions) {
  std::vector<VkExtensionProperties> supported_extensions = GetSupportedInstanceExtensions();

  auto is_extension_supported = [&supported_extensions](gsl::czstring name) {
    return std::any_of(supported_extensions.begin(), supported_extensions.end(), 
      [name](const VkExtensionProperties& property) {
        return veng::streq(property.extensionName, name);
      });
  };

  return std::all_of(extensions.begin(), extensions.end(), is_extension_supported);
}

}  // namespace veng