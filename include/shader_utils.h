#pragma once

#include <string>
#include <GL/glew.h>

std::string load_file(const std::string& path);
GLuint compile_shader(const std::string& vertex_path, const std::string& fragment_path);
void check_shader_compilation(GLuint shader, const std::string& name);
void check_program_linking(GLuint program);
