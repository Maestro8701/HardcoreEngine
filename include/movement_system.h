#pragma once
#include "system.h"
#include "ecs.h"
#include <cmath>
#include <algorithm>

class MovementSystem : public System {
public:
    MovementSystem(ECS& ecs);
    void update(float delta_time) override;
    void set_global_speed(float speed);
    float get_global_speed() const { return global_speed_; }

private:
    ECS& ecs_;
    float time_accumulator = 0.0f;
    float global_speed_ = 1.0f;

    const float max_radius_ = 5.0f;   
    const float min_radius_ = 0.5f;
    const float center_distance_ = 0.2f; 
};
