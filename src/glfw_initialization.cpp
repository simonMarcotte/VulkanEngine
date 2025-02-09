#include <glfw_initialization.h>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <precomp.h>
#include <spdlog/spdlog.h>

namespace veng {

    void glfw_error_callback(std::int32_t error_code, gsl::czstring message){
        spdlog::error("GFLW Validation: {}", message);
    }

    GlfwInitilization::GlfwInitilization() { 
        glfwSetErrorCallback(glfw_error_callback);
        if (glfwInit() != GLFW_TRUE) 
            std::exit(EXIT_FAILURE);
    }

    GlfwInitilization::~GlfwInitilization() {
        glfwTerminate();
    }
}