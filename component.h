#pragma once
#include <typeindex>
#include <glm/glm.hpp>

class Component {
public:
    virtual ~Component() = default;
    virtual std::type_index type() const = 0;
};

struct PositionComponent : public Component {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    PositionComponent(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f)
        : x(x_), y(y_), z(z_) {
    }

    std::type_index type() const override { return typeid(PositionComponent); }
};

struct ColorComponent : public Component {
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
    float size_factor = 1.0f;  

    ColorComponent(glm::vec3 color_ = glm::vec3(1.0f, 0.0f, 0.0f)) : color(color_) {}
    std::type_index type() const override { return typeid(ColorComponent); }
};



struct VelocityComponent : public Component {
    float vx = 0.0f;
    float vy = 0.0f;
    float vz = 0.0f;
    float radius = 1.0f; 
    float angular_speed = 2.0f; 

    VelocityComponent(float radius_ = 2.0f, float angular_speed_ = 2.0f)
        : radius(radius_), angular_speed(angular_speed_) {
    }

    std::type_index type() const override {
        return typeid(VelocityComponent);
    }
};
