#pragma once
#include <cstdint>
#include <atomic>

class Entity {
public:
    static uint32_t generate_id() {
        static std::atomic<uint32_t> next_id{ 1 };
        return next_id.fetch_add(1);
    }

    explicit Entity(uint32_t id = 0) : id_(id) {}
    uint32_t id() const { return id_; }

private:
    uint32_t id_;
};
