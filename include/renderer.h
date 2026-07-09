#pragma once

#include "ecs.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "shader_utils.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    float size_factor;
};

class Renderer {
public:
    Renderer(ECS& ecs, GLFWwindow* window);
    void update();
    void render();
    void set_view_projection(const glm::mat4& view, const glm::mat4& projection);
    void set_zoom_level(float zoom);
    float get_zoom_level() const;
    void update_rotation(float delta_time);
    void set_camera_position(const glm::vec3& pos);

private:
    float global_particle_size_ = 50.0f;
    ECS& ecs_;
    GLFWwindow* window_;
    GLuint VAO_ = 0, VBO_ = 0, shader_program = 0;
    std::vector<Vertex> vertices;
    float zoom_level_ = 1.0f;
    float rotation_angle_ = 0.0f;
    glm::vec3 camera_pos_{ 0.0f, 0.0f, 3.0f };

    void init();
    void update_buffer();
    void checkOpenGLError(const std::string& context);
};

Renderer::Renderer(ECS& ecs, GLFWwindow* window)
    : ecs_(ecs), window_(window)
{
    init();
    update();
}

void Renderer::init()
{
    // Load shaders from files
    shader_program = compile_shader("shaders/particle.vert", "shaders/particle.frag");
    if (shader_program == 0) {
        throw std::runtime_error("Failed to compile shaders: check 'shaders/particle.vert' and 'shaders/particle.frag'");
    }

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    GLint pos_attrib = glGetAttribLocation(shader_program, "aPos");
    GLint color_attrib = glGetAttribLocation(shader_program, "aColor");
    GLint size_attrib = glGetAttribLocation(shader_program, "aSize");

    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(color_attrib);
    glVertexAttribPointer(size_attrib, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(size_attrib);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    checkOpenGLError("VAO/VBO setup");
}

void Renderer::update_buffer()
{
    if (vertices.empty()) {
        std::cerr << "Warning: Trying to update buffer with empty vertices!" << std::endl;
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::update()
{
    vertices.clear();
    auto entities = ecs_.get_all_entities();
    for (uint32_t entity_id : entities) {
        auto* pos = ecs_.get_component<PositionComponent>(entity_id);
        auto* color = ecs_.get_component<ColorComponent>(entity_id);
        if (pos && color) {
            vertices.push_back({ {pos->x, pos->y, pos->z}, color->color, color->size_factor });
        }
    }
    update_buffer();
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);

    // Pass global parameters to shader
    GLint global_size_loc = glGetUniformLocation(shader_program, "global_size");
    if (global_size_loc != -1) {
        glUniform1f(global_size_loc, global_particle_size_);
    }

    GLint rotation_loc = glGetUniformLocation(shader_program, "rotation_angle");
    if (rotation_loc != -1) {
        glUniform1f(rotation_loc, rotation_angle_);
    }

    glBindVertexArray(VAO_);
    if (!vertices.empty()) {
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));
    }
    glBindVertexArray(0);
    glDisable(GL_PROGRAM_POINT_SIZE);
    glUseProgram(0);
}

void Renderer::set_view_projection(const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shader_program);
    GLint view_loc = glGetUniformLocation(shader_program, "view");
    GLint proj_loc = glGetUniformLocation(shader_program, "projection");
    if (view_loc == -1 || proj_loc == -1) {
        std::cerr << "ERROR: Uniform 'view' or 'projection' not found in shader!" << std::endl;
        return;
    }
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &projection[0][0]);
    glUseProgram(0);
}

void Renderer::update_rotation(float delta_time)
{
    rotation_angle_ += delta_time * 0.5f;
    if (rotation_angle_ > 2.0f * glm::pi<float>()) {
        rotation_angle_ -= 2.0f * glm::pi<float>();
    }
}

void Renderer::set_zoom_level(float zoom)
{
    zoom_level_ = std::max(0.1f, std::min(5.0f, zoom));
}

float Renderer::get_zoom_level() const
{
    return zoom_level_;
}

void Renderer::set_camera_position(const glm::vec3& pos)
{
    camera_pos_ = pos;
}

void Renderer::checkOpenGLError(const std::string& context)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error in " << context << ": " << err << std::endl;
    }
}
