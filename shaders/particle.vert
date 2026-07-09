#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aSize;

out vec3 ourColor;
out float pointSize;

uniform mat4 view;
uniform mat4 projection;
uniform float rotation_angle;
uniform float global_size;

void main()
{
    vec3 rotated_pos = aPos;
    rotated_pos.xz = mat2(
        cos(rotation_angle), -sin(rotation_angle),
        sin(rotation_angle),  cos(rotation_angle)
    ) * rotated_pos.xz;

    gl_Position = projection * view * vec4(rotated_pos, 1.0);
    ourColor = aColor;
    pointSize = global_size * aSize;
}
