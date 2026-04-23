<div align="center">

```
 ██████╗██████╗ ████████╗    ██████╗  █████╗ ██╗   ██╗    ████████╗██████╗  █████╗  ██████╗███████╗██████╗
██╔════╝██╔══██╗╚══██╔══╝    ██╔══██╗██╔══██╗╚██╗ ██╔╝    ╚══██╔══╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗
██║     ██████╔╝   ██║       ██████╔╝███████║ ╚████╔╝        ██║   ██████╔╝███████║██║     █████╗  ██████╔╝
██║     ██╔══██╗   ██║       ██╔══██╗██╔══██║  ╚██╔╝         ██║   ██╔══██╗██╔══██║██║     ██╔══╝  ██╔══██╗
╚██████╗██║  ██║   ██║       ██║  ██║██║  ██║   ██║          ██║   ██║  ██║██║  ██║╚██████╗███████╗██║  ██║
 ╚═════╝╚═╝  ╚═╝   ╚═╝       ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝          ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚══════╝╚═╝  ╚═╝
```

**CPU Ray Tracer в C++ | Chaos Ray Tracing Course**

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat-square&logo=windows&logoColor=white)]()
[![Build](https://img.shields.io/badge/Build-Visual%20Studio%202022-5C2D91?style=flat-square&logo=visualstudio&logoColor=white)]()
[![RapidJSON](https://img.shields.io/badge/JSON-RapidJSON-orange?style=flat-square)]()

</div>

---

## 🔭 Overview

**CRT Ray Tracer** е CPU-базиран ray tracer, написан от нулата в C++ като учебен проект към [Chaos Ray Tracing Course](https://www.chaos.com/chaos-camp-2025?srsltid=AfmBOoqPK7dHQLAMDpOiJICx2t8SDpW7myyjSEWFLLVz6VfhB0KpONz4). Проектът имплементира физически коректен рендъринг — от основната математика на лъчите и триъгълниците до глобално осветление и пространствени структури за ускорение.

Всеки пиксел се изчислява чрез рекурсивно проследяване на лъчи, симулирайки как светлината взаимодейства с повърхности в сцената.

---

## ⚙️ Функции

### 🎨 Рендъринг
| Функция | Описание |
|---|---|
| **Перспективна камера** | Ray generation от camera matrix и позиция |
| **Ray–Triangle Intersection** | Точно засичане на лъч с триъгълник |
| **Осветление & Shading** | Point lights с дифузно осветление |
| **Сенки** | Shadow rays с bias корекция срещу self-intersection |
| **Отражения** | Рекурсивни reflection rays до `MAX_RAY_DEPTH = 5` |
| **Рефракции** | Диелектрични материали с Fresnel formula и IOR |
| **Текстури** | Bitmap текстури, checkerboard, edge texture pattern |
| **Smooth Shading** | Интерполиране на нормали по върхове (Phong shading) |
| **Глобално Осветление (GI)** | Hemisphere sampling за индиректно осветление |
| **Анимации** | Поддръжка за multi-frame рендъринг (12 frames) |

### ⚡ Ускорения
| Оптимизация | Клас | Описание |
|---|---|---|
| **AABB** | `CRTAxisAlignedBB` | Axis-Aligned Bounding Boxes — бързо broad-phase отхвърляне на лъчи |
| **KD-Tree** | `CRTKDTree` | Пространствено разделяне (дълбочина до 12 нива) за O(log n) ray-mesh intersection |
| **Bucket Rendering** | `CRTRenderer` | Разделяне на кадъра на парчета за по-ефективно изчисление |

### 📄 Scene Format
Сцените се зареждат от `.crtscene` файлове (JSON формат) чрез **RapidJSON**. Поддържани полета:

```json
{
  "settings": { "background_color": [...], "image_settings": {...}, "gi_on": true },
  "camera":   { "matrix": [...], "position": [...] },
  "lights":   [{ "intensity": 1.0, "position": [...] }],
  "materials":[{ "type": "...", "albedo": [...], "ior": 1.5, "smooth_shading": true }],
  "textures": [{ "type": "bitmap", "file_path": "..." }],
  "objects":  [{ "material_index": 0, "vertices": [...], "triangles": [...], "uvs": [...] }]
}
```

---

## 🏗️ Структура на проекта

```
chaos-ray-tracing/
│
├── main.cpp                    # Entry point
├── Helper.h / Helper.cpp       # Константи, типове и utility функции
│
├── algorithms/
│   ├── CRTRenderer             # Главен рендъринг loop (bucket rendering, TraceType)
│   ├── CRTShader               # Осветление, сенки, отражения, рефракции, GI
│   ├── CRTRayTriangle          # Ray–triangle intersection
│   └── CRTAlbedo               # Albedo изчисления за материали и текстури
│
├── math/
│   ├── CRTVector               # 3D вектор (dot, cross, normalize, reflect, refract)
│   ├── CRTMatrix               # 3×3 матрица и трансформации
│   ├── CRTRay                  # Ray (origin, direction, depth)
│   └── CRTriangle              # Triangle геометрия и нормали
│
├── image/
│   ├── CRTImageBuffer          # PPM image buffer
│   ├── BitMapBuffer            # Bitmap output
│   ├── CRTColor                # RGB цвят с операции
│   ├── CRTAxisAlignedBB        # AABB имплементация
│   ├── CRTKDTree               # KD-Tree за mesh acceleration
│   └── stbImage                # stb_image за зареждане на текстури
│
├── scene/
│   ├── CRTScene                # Контейнер за цялата сцена
│   ├── CRTSceneParser          # JSON парсер за .crtscene
│   ├── CRTCamera               # Камера и ray generation
│   ├── CRTMesh                 # Mesh геометрия
│   ├── CRTMaterial             # Материали (diffuse, reflective, refractive)
│   ├── CRTexture               # Текстурни типове
│   ├── CRTLight                # Point light
│   └── CRTSettings             # Глобални настройки
│
└── rapidjson/                  # Vendor JSON библиотека
```

---

## 📸 Gallery

> Всички изображения са генерирани изцяло от този ray tracer — без post-processing.

### Cornell Box с рефракция и GI
![Cornell Box](renders/task5.png)
*Затворена стая (Cornell Box) с цветни стени, точково осветление и стъклена сфера в центъра. Демонстрира рефракция и отражения.*

---

### Сфера със сянка
![Sphere Shadow](renders/task4.png)
*Жълта сфера над равнина с меко осветление и коректна сянка — демонстрира shadow rays и дифузно shading.*

---

### Текстури и UV Mapping
![Textures](renders/hw12_task4.png)
*Четири quad-а с различни текстурни типове: solid color, edge texture, checkerboard pattern и bitmap текстура.*

---

### 3D Модел — Дракон
![Dragon](renders/dragon12.png)
*Сложен 3D mesh на дракон с хиляди триъгълници, рендериран с KD-Tree ускорение, осветление и сенки.*

## 🚀 Как да го стартираш

### Изисквания

- Visual Studio 2022 (v143 toolset)
- Windows 10/11 x64
- C++17

### Build

1. Отвори `main.sln` с Visual Studio 2022
2. Избери конфигурация `Release | x64`
3. `Ctrl+Shift+B`

### Run

Редактирай `main.cpp` за да посочиш сцена и изходен файл:

```cpp
CRTRenderer renderer("scene/scene1.crtscene", RenderType::BUCKETS, TraceType::KD);
renderer.renderScene("output1.ppm");
```

Изходният файл се записва като `.ppm` изображение.

| `TraceType` | Описание |
|---|---|
| `KD` | KD-Tree ускорение — препоръчително за сложни mesh-ове |

---

## 🔬 Как работи

```
Camera
  └─► Ray Generation (per pixel)
        └─► Scene Traversal
              ├─ AABB Broad-Phase Test        — O(1) отхвърляне
              └─ KD-Tree Intersection         — O(log n) намиране
                    └─ Triangle Hit Test
                          └─ Shading
                                ├─ Direct Lighting + Shadow Rays
                                ├─ Reflection Ray  (рекурсивно, до дълбочина 5)
                                ├─ Refraction Ray  (Fresnel + Snell's Law)
                                └─ GI Hemisphere Sampling
```

- **AABB** отхвърля лъчи бързо на ниво bounding box преди скъпите триъгълникови тестове.
- **KD-Tree** (дълбочина 12) разделя mesh-а пространствено — намалява сложността от O(n) до O(log n).
- **GI** сэмплва хемисферата около hit point (`DEFAULT_GI_RAYS_COUNT = 4`) за индиректна дифузна светлина.
- **Fresnel** формулата (`0.5 * (1 + dot)^5`) определя баланса отражение/рефракция при диелектрични материали.

---

## 📦 Зависимости

| Библиотека | Употреба |
|---|---|
| [RapidJSON](https://rapidjson.org/) | Парсване на `.crtscene` JSON файлове |
| [stb_image](https://github.com/nothings/stb) | Зареждане на bitmap текстури |

---

## 👤 Автор

**Nikolay Georgiev** — [@NGeorgiev12](https://github.com/NGeorgiev12)

---

<div align="center">
  <sub>Всеки пиксел е изчислен честно. Без GPU. Без трикове. Само физика и математика.</sub>
</div>
