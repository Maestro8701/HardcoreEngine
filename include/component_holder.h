#pragma once
#include "component.h"
#include <unordered_map>
#include <vector>

class ComponentHolderBase {
public:
    virtual ~ComponentHolderBase() = default;
    virtual Component* get(uint32_t entity_id) const = 0;
    virtual std::vector<uint32_t> get_entity_ids() const = 0;
};

template <typename T>
class ComponentHolder : public ComponentHolderBase {
public:
    void add(uint32_t entity_id, T component) {
        components_[entity_id] = std::move(component);
    }

    Component* get(uint32_t entity_id) const override {
        auto it = components_.find(entity_id);
        if (it == components_.end()) {
            return nullptr;
        }
        return const_cast<T*>(&it->second);  
    }

    T* get_typedef(uint32_t entity_id) const {  
        auto it = components_.find(entity_id);
        if (it == components_.end()) {
            return nullptr;
        }
        return const_cast<T*>(&it->second);
    }

    std::vector<uint32_t> get_entity_ids() const override {
        std::vector<uint32_t> ids;
        for (const auto& pair : components_) {
            ids.push_back(pair.first);
        }
        return ids;
    }

private:
    mutable std::unordered_map<uint32_t, T> components_;
};
