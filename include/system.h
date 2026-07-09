#pragma once
class System {
public:
    virtual ~System() = default; 
    virtual void update(float) {} 
};
