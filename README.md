📖 HCEngine (HCE) – ECS Particle System
Минималистичный игровой движок с ECS, OpenGL и системой частиц

OpenGL Version
C++
License

🚀 Быстрый старт
📥 Установка зависимостей (Windows x64)
⚠️ Важно: Используйте GLFW 3.4 (WIN64), GLEW 2.3.1 и GLM 1.0.3.
Скачайте бинарные архивы с официальных сайтов и распакуйте в папку libs/:

GLFW 3.4 (WIN64)
GLEW 2.3.1
GLM 1.0.3
Пример структуры проекта:
hce-engine/
├── libs/
│   ├── glew-2.3.1/
│   ├── glfw-3.4.bin.WIN64/
│   └── glm-1.0.3/
🔨 Сборка проекта
1. Генерация проекта CMake (Windows)
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
💡 Примечание: Для других версий Visual Studio замените Visual Studio 17 2022 на:

Visual Studio 16 2019
MinGW Makefiles (если используете MinGW)
2. Сборка
cmake --build . --config Release
3. Запуск
.\Release\hce-engine.exe
🎮 Управление
Кнопка	Действие
WASD	Движение камеры
+ / -	Зум
ESC	Выход
🏗️ Архитектура проекта
📁 Структура папок
hce-engine/
├── assets/          # Ресурсы (шейдеры, текстуры)
│   └── shaders/     # GLSL-шейдеры (particle.vert, particle.frag)
├── docs/            # Документация
├── libs/            # Внешние библиотеки (GLFW, GLEW, GLM)
├── src/             # Исходный код
│   ├── ecs/         # ECS-архитектура
│   │   ├── ecs.h     # Основной класс ECS
│   │   ├── component.h # Базовые компоненты
│   │   ├── entity.h   # Класс Entity
│   │   ├── system.h   # Базовый System
│   │   └── component_holder.h # Хранилище компонентов
│   ├── components/   # Компоненты
│   │   ├── position.h
│   │   ├── velocity.h
│   │   └── color.h
│   ├── systems/      # Системы обработки
│   │   └── movement_system.h
│   └── renderer/     # OpenGL-рендер
│       ├── renderer.h
│       └── shader.h
├── CMakeLists.txt   # Конфигурация сборки
└── README.md        # Этот файл
Показать все
🧩 ECS-Архитектура
Компоненты
Компонент	Описание	Пример использования
PositionComponent	Координаты частицы	{x, y, z}
VelocityComponent	Параметры движения	{radius, angular_speed}
ColorComponent	Цвет и размер	{glm::vec3(1.0f, 0.0f, 0.0f), size_factor}
Системы
Система	Описание	Код
MovementSystem	Обновляет позиции частиц	movement_system.update(delta_time)
Renderer	Отрисовывает частицы с OpenGL	renderer.render()
Пример создания частиц
ECS ecs;
uint32_t entity_id = ecs.create_entity();
ecs.add_component<ColorComponent>(entity_id, ColorComponent{ glm::vec3(1.0f, 0.0f, 0.0f), 1.0f });

for (int i = 0; i < 50; ++i) {
    uint32_t id = ecs.create_entity();
    ecs.add_component<PositionComponent>(id, { 2.0f * cos(i), 0.0f, 2.0f * sin(i) });
    ecs.add_component<VelocityComponent>(id, VelocityComponent{ 2.0f, 1.5f });
    ecs.add_component<ColorComponent>(id, { glm::vec3(i/50.0f, 0.5f, 0.8f), 1.0f });
}
📜 API-Справка
Класс ECS
Метод	Описание	Пример
create_entity()	Создаёт новую сущность	uint32_t id = ecs.create_entity()
add_component<T>(id, component)	Добавляет компонент	ecs.add_component<PositionComponent>(id, {0.0f, 0.0f, 0.0f})
get_component<T>(id)	Получает компонент	auto* pos = ecs.get_component<PositionComponent>(id)
update(delta_time)	Обновляет все системы	ecs.update(delta_time)
Класс Renderer
Метод	Описание	Пример
set_view_projection(view, projection)	Устанавливает матрицы вида и проекции	renderer.set_view_projection(view, projection)
render()	Отрисовывает все частицы	renderer.render()
update_rotation(delta_time)	Обновляет угол поворота	renderer.update_rotation(delta_time)
Класс MovementSystem
Метод	Описание	Пример
set_global_speed(speed)	Устанавливает глобальную скорость	movement_system.set_global_speed(2.0f)
update(delta_time)	Обновляет позиции частиц	movement_system.update(delta_time)
🎨 Шейдеры
Вершинный шейдер (assets/shaders/particle.vert)
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

void main() {
    vec3 rotated_pos = aPos;
    rotated_pos.xz = mat2(cos(rotation_angle), -sin(rotation_angle),
                          sin(rotation_angle), cos(rotation_angle)) * rotated_pos.xz;
    gl_Position = projection * view * vec4(rotated_pos, 1.0);
    ourColor = aColor;
    float size = global_size * aSize;
    gl_PointSize = size;
}
Показать все
Фрагментный шейдер (assets/shaders/particle.frag)
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(ourColor, 1.0);
}
📌 Планы развития
Функционал	Статус	Приоритет
Система частиц	✅ Готово	⭐⭐⭐
OpenGL-рендер	✅ Готово	⭐⭐⭐
Динамическая загрузка ресурсов	❌ Нет	⭐⭐
Физика (Jolt/Bullet)	❌ Нет	⭐
UI (Dear ImGui)	❌ Нет	⭐⭐
Поддержка Vulkan	❌ Нет	⭐⭐⭐
📜 Лицензия
MIT License
Разрешается свободное использование, модификация и распространение кода с указанием авторства.

🤝 Вклад в проект
Сделайте форк репозитория.
Создайте новую ветку (git checkout -b feature/new-feature).
Внесите изменения и закоммитьте (git commit -m "Add new feature").
Отправьте Pull Request.
📧 Контакты
GitHub: @your_username
Email: your_email@example.com
📊 Версии библиотек
Библиотека	Версия	Описание
GLFW	3.4 (WIN64)	Для управления окнами и вводом
GLEW	2.3.1	Для загрузки OpenGL-расширений
GLM	1.0.3	Для работы с математикой (векторы, матрицы)
