#include <GLFW/glfw3.h>
#include <glfw_initialization.h>
#include <glfw_monitor.h>
#include <glfw_window.h>
#include <graphics.h>
#include <precomp.h>

std::int32_t main() {
  const veng::GlfwInitilization _glfw;

  veng::Window window("Vulkan Engine", {800, 600});
  window.TryMoveToMonitor(0);

  veng::Graphics mygraphics(&window);

  while (!window.ShouldClose()) {
    glfwPollEvents();
  }

  return EXIT_SUCCESS;
}