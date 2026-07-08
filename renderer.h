#pragma once
#include "ecs.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <iostream>

struct Vertex {
    glm::vec3 position;  
    glm::vec3 color;     
    float size_factor;    
};


class Renderer {
public:
    Renderer(ECS& ecs, GLFWwindow* window) : ecs_(ecs), window_(window) {
        init();
        update();
    }

    void update() {
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

    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);

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
    }

    void set_view_projection(const glm::mat4& view, const glm::mat4& projection) {
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

    void set_zoom_level(float zoom) {
        zoom_level_ = std::max(0.1f, std::min(5.0f, zoom));
    }

    float get_zoom_level() const {
        return zoom_level_;
    }

    void update_rotation(float delta_time) {
        rotation_angle_ += delta_time * 0.5f;
        if (rotation_angle_ > 2.0f * glm::pi<float>()) {
            rotation_angle_ -= 2.0f * glm::pi<float>();
        }
    }

    void set_camera_position(const glm::vec3& pos) {
        camera_pos_ = pos;
    }

private:

    float global_particle_size_ = 50.0f;
    ECS& ecs_;
    GLFWwindow* window_;
    GLuint VAO_ = 0, VBO_ = 0, shader_program = 0;
    std::vector<Vertex> vertices;
    float zoom_level_ = 1.0f;
    float particle_size = 10.0f; 

    float rotation_angle_ = 0.0f;
    glm::vec3 camera_pos_{ 0.0f, 0.0f, 3.0f };

    void init() {
        const char* vertex_shader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aSize;  // Новый атрибут размера

out vec3 ourColor;
out float pointSize;

uniform mat4 view;
uniform mat4 projection;
uniform float rotation_angle;
uniform float global_size;  // Глобальный размер частиц

void main() {
    vec3 rotated_pos = aPos;
    rotated_pos.xz = mat2(cos(rotation_angle), -sin(rotation_angle),
                          sin(rotation_angle), cos(rotation_angle)) * rotated_pos.xz;

    gl_Position = projection * view * vec4(rotated_pos, 1.0);
    ourColor = aColor;

    // Размер частицы с учетом глобального и индивидуального фактора
    float size = global_size * aSize;
    gl_PointSize = size;  // Используем размер точки
}
)";

        const char* fragment_shader = R"(
            #version 330 core
            in vec3 ourColor;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(ourColor, 1.0);
            }
        )";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);
        checkShaderCompilation(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);
        checkShaderCompilation(fs);

        shader_program = glCreateProgram();
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);
        checkProgramLinking(shader_program);

        glDeleteShader(vs);
        glDeleteShader(fs);

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

    void update_buffer() {
        if (vertices.empty()) {
            std::cerr << "Warning: Trying to update buffer with empty vertices!" << std::endl;
            return;
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void checkShaderCompilation(GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void checkProgramLinking(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    void checkOpenGLError(const std::string& context) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL Error in " << context << ": " << err << std::endl;
        }
    }
};
