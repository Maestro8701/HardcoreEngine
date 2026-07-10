cmake_minimum_required(VERSION 3.15)
project(HardcoreEngine)

# Настройки C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти OpenGL, GLEW, GLFW3
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Настройки для Windows (если нужно)
if(WIN32)
    add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)
endif()

# Собрать executable
add_executable(HardcoreEngine
    src/main.cpp
    src/ecs/ecs.h
    src/ecs/entity.h
    src/ecs/system.h
    src/ecs/component.h
    src/ecs/component_holder.h
    src/ecs/movement_system.h
    src/ecs/movement_system.cpp
    src/renderer/renderer.h
    src/renderer/renderer.cpp
    src/utils/shader_utils.h
    src/utils/shader_utils.cpp
)

# Подключить зависимости
target_link_libraries(HardcoreEngine
    PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
)

# Подключить папки для инклудов
target_include_directories(HardcoreEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
)

# Настройки для сборки под Windows
if(WIN32)
    set_target_properties(HardcoreEngine PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Опции для отладки
option(BUILD_WITH_DEBUG_INFO "Включить отладочные символы" ON)
if(BUILD_WITH_DEBUG_INFO)
    target_compile_options(HardcoreEngine PRIVATE -g)
endif()
