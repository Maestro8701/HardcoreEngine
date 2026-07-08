#pragma once
#include "system.h"
#include "entity.h"
#include "component_holder.h"
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <unordered_set>
#include <memory>
#include <cstdint>

class ECS {
public:
    template <typename T, typename... Args>
    std::enable_if_t<std::is_base_of_v<System, T>, T*> add_system(Args&&... args) {
        T* system = new T(std::forward<Args>(args)...);
        systems_.push_back(system);
        return system;
    }

    template <typename T, typename... Args>
    std::enable_if_t<!std::is_base_of_v<System, T>, T*> add_system(Args&&... args) = delete;

    template <typename T>
    void register_component(uint32_t entity_id, T component) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        get_component_holder<T>().add(entity_id, std::move(component));
    }

    // В public-секции ECS
    template <typename T>
    void add_component(uint32_t entity_id, T component) {
        register_component<T>(entity_id, std::move(component));
    }


    template <typename T>
    T* get_component(uint32_t entity_id) {
        auto it = holders_.find(typeid(T));
        if (it == holders_.end()) return nullptr;
        return static_cast<ComponentHolder<T>*>(it->second.get())->get_typedef(entity_id);
    }


    uint32_t create_entity() {
        static uint32_t next_id = 0;
        return next_id++;
    }

    std::vector<uint32_t> get_all_entities() const {
        std::unordered_set<uint32_t> unique_entities;
        for (const auto& holder_pair : holders_) {
            const auto& entity_ids = holder_pair.second->get_entity_ids();
            unique_entities.insert(entity_ids.begin(), entity_ids.end());
        }
        return std::vector<uint32_t>(unique_entities.begin(), unique_entities.end());
    }

    void update(float delta_time) {
        for (auto* system : systems_) {
            system->update(delta_time);
        }
    }

    const std::vector<System*>& get_systems() const { return systems_; }

private:
    template <typename T>
    ComponentHolder<T>& get_component_holder() {
        auto it = holders_.find(typeid(T));
        if (it == holders_.end()) {
            auto holder = std::make_unique<ComponentHolder<T>>();
            it = holders_.emplace(typeid(T), std::move(holder)).first;
        }
        return *static_cast<ComponentHolder<T>*>(it->second.get());
    }

    std::unordered_map<std::type_index, std::unique_ptr<ComponentHolderBase>> holders_;
    std::vector<System*> systems_;
};
