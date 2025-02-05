#include <glfw_initialization.h>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <precomp.h>

namespace veng {
    GlfwInitilization::GlfwInitilization() { 
        if (glfwInit() != GLFW_TRUE) 
            std::exit(EXIT_FAILURE);
        }

    GlfwInitilization::~GlfwInitilization() {
        glfwTerminate();
    }
}