# Архитектура ECS Particle System

## Основные компоненты
- `PositionComponent`: Координаты (x, y, z).
- `ColorComponent`: RGB-цвет + размер.
- `VelocityComponent`: Радиус и угловая скорость.

## Системы
- `MovementSystem`: Обновляет позиции частиц по времени.
- `Renderer`: Отрисовывает частицы через OpenGL.
