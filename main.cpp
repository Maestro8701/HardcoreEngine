#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ecs.h"
#include "movement_system.h"
#include "renderer.h"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 camera_pos(5.0f, 0.0f, 0.0f); 
float camera_yaw = 0.0f;
float camera_pitch = 0.0f;
float camera_speed = 0.05f;
float zoom_level = 1.0f;

void processInput(GLFWwindow* window, float delta_time) {
    const float sensitivity = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_pos.z -= camera_speed * delta_time * 100.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_pos.z += camera_speed * delta_time * 100.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_pos.x -= camera_speed * delta_time * 100.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_pos.x += camera_speed * delta_time * 100.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        zoom_level = std::min(10.0f, zoom_level + 0.1f); 
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        zoom_level = std::max(0.1f, zoom_level - 0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ECS Particle System", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    ECS ecs;
    uint32_t entity_id = ecs.create_entity();
    ecs.add_component<ColorComponent>(entity_id, ColorComponent{ glm::vec3(1.0f, 0.0f, 0.0f) });

    for (int i = 0; i < 50; ++i) { 
        uint32_t id = ecs.create_entity();
        float radius = 2.0f + static_cast<float>(i % 5) * 0.8f; 
        ecs.add_component<VelocityComponent>(id, VelocityComponent{ radius, 1.5f + static_cast<float>(i % 2) });
        float angle = static_cast<float>(i) * 0.2f;
        ecs.add_component<PositionComponent>(id, { radius * cos(angle), 0.0f, radius * sin(angle) });
        ecs.add_component<ColorComponent>(id, ColorComponent{
            glm::vec3(
                static_cast<float>(i) / 50.0f,
                0.5f + static_cast<float>(i % 3) * 0.2f,
                0.8f - static_cast<float>(i % 2) * 0.3f
            )
            });
    }

    MovementSystem movement_system(ecs);

    Renderer renderer(ecs, window);

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        camera_pos,
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    float last_time = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float current_time = static_cast<float>(glfwGetTime());
        float delta_time = current_time - last_time;
        last_time = current_time;

        processInput(window, delta_time);

        movement_system.update(delta_time);

        view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        float fov = 45.0f / zoom_level;
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        renderer.set_camera_position(camera_pos);
        renderer.set_zoom_level(zoom_level);
        renderer.update_rotation(delta_time);
        renderer.update();
        renderer.set_view_projection(view, projection);

        renderer.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
