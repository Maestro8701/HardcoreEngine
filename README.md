# 📖 HCEngine (HCE) – ECS Particle System

Минималистичный игровой движок с архитектурой ECS (Entity Component System), OpenGL и продвинутой системой частиц.

## 🛠️ Спецификации
* **OpenGL Version:** 3.3 Core
* **Язык программирования:** C++
* **Лицензия:** MIT License 


## 📦 Зависимости
| Библиотека | Версия | Платформа | Описание | Ссылка |
|------------|--------|-----------|----------|--------|
| **GLFW** | 3.4 | WIN64 | Управление окнами/вводом | [https://www.glfw.org/](https://www.glfw.org/) |
| **GLEW** | 2.3.1 | WIN64 | Расширения OpenGL | [http://glew.sourceforge.net/](http://glew.sourceforge.net/) |
| **GLM** | 1.0.3 | - | Математика для 3D | [https://glm.g-truc.net/](https://glm.g-truc.net/) |

⚠️ **Важно:** Используйте указанные версии. Скачайте бинарные архивы (GLFW/GLEW) и распакуйте в `libs/`.
---

## 🚀 Быстрый старт

### 📥 Установка зависимостей (Windows x64)

> ⚠️ **Важно:** Используйте строго указанные версии библиотек: **GLFW 3.4 (WIN64)**, **GLEW 2.3.1** и **GLM 1.0.3**. 

Скачайте бинарные архивы с официальных сайтов и распакуйте их в папку `libs/`. Пример структуры проекта должен выглядеть следующим образом:

```text
hce-engine/
├── libs/
│   ├── glew-2.3.1/
│   ├── glfw-3.4.bin.WIN64/
│   └── glm-1.0.3/
```

### 🔨 Сборка проекта

#### 1. Генерация проекта через CMake (Windows)
Откройте терминал в корневой папке проекта и выполните команды:
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
```

💡 **Примечание:** Для других сред разработки замените параметр генератора `-G` на нужный:
* `"Visual Studio 16 2019"` — для VS 2019.
* `"MinGW Makefiles"` — если вы используете MinGW / GCC.

#### 2. Компиляция
```bash
cmake --build . --config Release
```

#### 3. Запуск приложения
```bash
.\Release\hce-engine.exe
```

---

## 🎮 Управление

| Кнопка | Действие |
| :---: | :--- |
| **W A S D** | Движение камеры |
| **/** | Зум (Масштабирование) |
| **ESC** | Выход из приложения |

---

## 🏗️ Архитектура проекта

### 📁 Структура папок

```text
hce-engine/
├── assets/                  # Ресурсы
│   └── shaders/             # GLSL-шейдеры (particle.vert, particle.frag)
├── docs/                    # Документация проекта
├── libs/                    # Внешние библиотеки (GLFW, GLEW, GLM)
├── src/                     # Исходный код ядра движка
│   ├── ecs/                 # Базовая ECS-архитектура
│   │   ├── ecs.h            # Основной класс менеджмента ECS
│   │   ├── component.h      # Базовый интерфейс/структура компонентов
│   │   ├── entity.h         # Класс сущности (Entity)
│   │   ├── system.h         # Базовый класс систем (System)
│   │   └── component_holder.h # Хранилище и пулы компонентов
│   ├── components/          # Реализация конкретных компонентов
│   │   ├── position.h
│   │   ├── velocity.h
│   │   └── color.h
│   ├── systems/             # Системы обработки логики
│   │   └── movement_system.h
│   └── renderer/            # Графическая подсистема OpenGL
│       ├── renderer.h
│       └── shader.h
├── CMakeLists.txt           # Главный конфигурационный файл сборки
└── README.md                # Этот файл документации
```

---

## 🧩 ECS-Архитектура

### Компоненты

| Компонент | Описание | Пример использования |
| :--- | :--- | :--- |
| **PositionComponent** | Координаты частицы в пространстве | `{x, y, z}` |
| **VelocityComponent** | Параметры движения и вращения | `{radius, angular_speed}` |
| **ColorComponent** | Цвет и масштабный фактор | `{glm::vec3(1.0f, 0.0f, 0.0f), size_factor}` |

### Системы

| Система | Описание | Пример вызова в коде |
| :--- | :--- | :--- |
| **MovementSystem** | Расчитывает и обновляет позиции частиц | `movement_system.update(delta_time)` |
| **Renderer** | Отрисовывает подготовленные частицы через OpenGL | `renderer.render()` |

### 💻 Пример создания и инициализации частиц

```cpp
ECS ecs;

// Создание одиночной базовой сущности
uint32_t entity_id = ecs.create_entity();
ecs.add_component(entity_id, ColorComponent{ glm::vec3(1.0f, 0.0f, 0.0f), 1.0f });

// Массовая генерация частиц по круговой траектории
for (int i = 0; i (id)` | Возвращает указатель на компонент сущности | `auto* pos = ecs.get_component<PositionComponent>(id);` |
| `update(delta_time)` | Обновляет состояние всех зарегистрированных систем | `ecs.update(delta_time);` |

### Класс `Renderer`

| Метод | Описание | Пример |
| :--- | :--- | :--- |
| `set_view_projection(view, proj)` | Устанавливает матрицы вида и проекции для шейдеров | `renderer.set_view_projection(view, projection);` |
| `render()` | Производит непосредственную отрисовку всех частиц | `renderer.render();` |
| `update_rotation(delta_time)` | Обновляет внутренний угол поворота сцены/камеры | `renderer.update_rotation(delta_time);` |

### Класс `MovementSystem`

| Метод | Описание | Пример |
| :--- | :--- | :--- |
| `set_global_speed(speed)` | Устанавливает глобальный множитель скорости для частиц | `movement_system.set_global_speed(2.0f);` |
| `update(delta_time)` | Запускает перерасчет физических позиций частиц | `movement_system.update(delta_time);` |

---

## 🎨 Шейдеры

### Вершинный шейдер (`assets/shaders/particle.vert`)
```glsl
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

    gl_PointSize = pointSize; // ← ЭТА СТРОКА ОБЯЗАТЕЛЬНА!
}

```

### Фрагментный шейдер (`assets/shaders/particle.frag`)
```glsl
#version 330 core

in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
```

---

## 📌 Планы развития

| Функционал | Статус | Приоритет |
| :--- | :---: | :---: |
| **Система частиц** | ✅ Готово | ⭐⭐⭐ |
| **OpenGL-рендер** | ✅ Готово | ⭐⭐⭐ |
| **Динамическая загрузка ресурсов** | ❌ Нет | ⭐⭐ |
| **Физика (Jolt/Bullet)** | ❌ Нет | ⭐ |
| **UI (Dear ImGui)** | ❌ Нет | ⭐⭐ |
| **Поддержка Vulkan** | ❌ Нет | ⭐⭐⭐ |
| **GPU Simulation** | ❌ Нет | ⭐⭐⭐ |
| **AI Behavior** | ❌ Нет | ⭐⭐ |
| **Сеть** | ❌ Нет | ⭐⭐ |



---

## 📊 Версии библиотек

| Библиотека | Версия | Описание |
| :--- | :---: | :--- |
| **GLFW** | 3.4 (WIN64) | Для управления окнами, контекстом и пользовательским вводом |
| **GLEW** | 2.3.1 | Для инициализации и загрузки расширений OpenGL |
| **GLM** | 1.0.3 | Для работы с трехмерной математикой (векторы, матрицы) |

---

## 🤝 Вклад в проект

Будем рады вашей помощи в улучшении движка! Чтобы внести свой вклад:
1. Сделайте **форк** репозитория.
2. Создайте новую тематическую ветку:
   ```bash
   git checkout -b feature/new-feature
   ```
3. Внесите изменения и сделайте осмысленный коммит:
   ```bash
   git commit -m "Add new feature"
   ```
4. Отправьте ветку в свой форк и создайте **Pull Request** в основной репозиторий.

---

## 📧 Контакты

* **GitHub:** [@Maestro8701](https://github.com/Maestro8701)
* **Email:** [maestro8701l@gmail.com](mailto:maestro8701l@gmail.com)

---

## 📜 Лицензия

Проект распространяется под лицензией **MIT License**. Разрешается свободное использование, модификация и распространение кода с обязательным указанием авторства.

```
