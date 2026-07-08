#include "movement_system.h"
#include "component.h"

MovementSystem::MovementSystem(ECS& ecs) : ecs_(ecs) {}

void MovementSystem::set_global_speed(float speed) {
    global_speed_ = std::max(0.1f, speed);
}

void MovementSystem::update(float delta_time) {
    time_accumulator += delta_time * global_speed_;

    auto entities = ecs_.get_all_entities();
    for (uint32_t entity_id : entities) {
        auto* pos = ecs_.get_component<PositionComponent>(entity_id);
        auto* vel = ecs_.get_component<VelocityComponent>(entity_id);
        auto* color = ecs_.get_component<ColorComponent>(entity_id);

        if (pos && vel) {
            float angle = time_accumulator * vel->angular_speed;
            float radius = std::clamp(vel->radius, min_radius_, max_radius_);

            radius += 0.1f * sin(time_accumulator * 0.3f);

            float new_x = radius * cos(angle);
            float new_y = radius * sin(angle); 
            float new_z = 0.0f; 

            float distance_from_center = sqrt(new_x * new_x + new_z * new_z);
            if (distance_from_center < center_distance_) {
                float angle_to_center = atan2(-new_z, -new_x);
                new_x = center_distance_ * cos(angle_to_center);
                new_z = center_distance_ * sin(angle_to_center);
            }

            pos->x = new_x;
            pos->y = new_y;
            pos->z = new_z;

            if (color) {
                float size_factor = 0.5f + 0.5f * sin(time_accumulator * 0.2f + entity_id);
                color->size_factor = size_factor;
            }
        }
    }
}
