#include <precomp.h>
#include <graphics.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace veng {

static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationCallBack(
  VkDebugUtilsMessageSeverityFlagBitsEXT severity,
  VkDebugUtilsMessageTypeFlagsEXT type,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data
) {
  if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
    std::cout << "Validation Error: " << callback_data->pMessage << std::endl;
  } else {
    std::cout << "Validation Message: " << callback_data->pMessage << std::endl;
  }
  
  return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT GetCreateMessengerInfo() {
  VkDebugUtilsMessengerCreateInfoEXT creation_info = {};
  creation_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

  creation_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   | 
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

  creation_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

  creation_info.pfnUserCallback = ValidationCallBack;
  creation_info.pUserData = nullptr;

  return creation_info;
}

Graphics::Graphics(gsl::not_null<Window*> window) : window_(window) {

  #if !defined(NDEBUG)
    validation_enabled_ = true;
  #endif

  InitializeVulkan();
}

Graphics::~Graphics() {
}

void Graphics::InitializeVulkan() {
  CreateInstance();
}

void Graphics::CreateInstance() {

  std::array<gsl::czstring, 1> validation_layers = {"VK_LAYER_KHRONOS_validation"};
  if(!AreAllLayersSupported(validation_layers)){
    validation_enabled_ = false;
  }
  
  std::vector<gsl::czstring> required_extensions = GetRequiredInstanceExtensions();
  

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
  instance_creation_info.enabledExtensionCount = required_extensions.size();
  instance_creation_info.ppEnabledExtensionNames = required_extensions.data();
  instance_creation_info.enabledLayerCount = 0;

  VkDebugUtilsMessengerCreateInfoEXT messenger_creation_info = GetCreateMessengerInfo();

  if(validation_enabled_){
    instance_creation_info.pNext = &messenger_creation_info;
    instance_creation_info.enabledLayerCount = validation_layers.size();
    instance_creation_info.ppEnabledLayerNames = validation_layers.data();
  } else {
    instance_creation_info.enabledLayerCount = 0;
    instance_creation_info.ppEnabledLayerNames = nullptr;
  }

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

std::vector<gsl::czstring> Graphics::GetRequiredInstanceExtensions() {
  gsl::span<gsl::czstring> suggested_extensions = Graphics::GetSuggestedInstanceExtensions();
  std::vector<gsl::czstring> required_extensions(suggested_extensions.size());
  std::copy(suggested_extensions.begin(), suggested_extensions.end(), required_extensions.begin());

  if (validation_enabled_){
    required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  
  if (!AreAllExtensionsSupported(required_extensions)){
    std::exit(EXIT_FAILURE);
  }
  
  return required_extensions;
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

bool ExtensionMatchesName(gsl::czstring name, const VkExtensionProperties& properties) {
  return veng::streq(properties.extensionName, name);
}

bool IsExtensionSupported(gsl::span<VkExtensionProperties> extensions, gsl::czstring name) {
   return std::any_of(extensions.begin(), extensions.end(), std::bind_front(ExtensionMatchesName, name));
} 

bool Graphics::AreAllExtensionsSupported(gsl::span<gsl::czstring> extensions) {
  std::vector<VkExtensionProperties> supported_extensions = GetSupportedInstanceExtensions();

  return std::all_of(extensions.begin(), extensions.end(), std::bind_front(IsExtensionSupported, supported_extensions));
}


//---------------VALIDATION LAYERS-------------------------

std::vector<VkLayerProperties> Graphics::GetSupportedValidationlayers() {

  std::uint32_t count;
  vkEnumerateInstanceLayerProperties(&count, nullptr);

  if (count == 0){
    return {};
  }

  std::vector<VkLayerProperties> properties(count);
  vkEnumerateInstanceLayerProperties(&count, properties.data());
  return properties;
}

bool LayerMatchesName(gsl::czstring name, const VkLayerProperties& properties) {
  return veng::streq(properties.layerName, name);
}

bool IsLayerSupported(gsl::span<VkLayerProperties> layers, gsl::czstring name) {
   return std::any_of(layers.begin(), layers.end(), std::bind_front(LayerMatchesName, name));
} 

bool Graphics::AreAllLayersSupported(gsl::span<gsl::czstring> layers) {
  std::vector<VkLayerProperties> supported_layers = GetSupportedValidationlayers();

  return std::all_of(layers.begin(), layers.end(), std::bind_front(IsLayerSupported, supported_layers));
}

}  // namespace veng