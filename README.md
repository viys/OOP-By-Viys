# 用 C 语言实现面向对象编程（OOP）教程
## ——以“动物说话系统”为例

---

## 目录

1. [面向对象简介](#面向对象简介)
2. [项目结构总览](#项目结构总览)
3. [四大特性详解](#四大特性详解)
   - [封装（Encapsulation）](#封装encapsulation)
   - [继承（Inheritance）](#继承inheritance)
   - [多态（Polymorphism）](#多态polymorphism)
   - [抽象（Abstraction）](#抽象abstraction)
4. [编译与运行](#编译与运行)
5. [运行结果与解释](#运行结果与解释)
6. [扩展练习](#扩展练习)
7. [总结](#总结)

---

## 面向对象简介

C 语言本身没有 `class`、`extends` 或 `virtual` 等关键字，但可以通过结构体、函数指针和统一接口表达面向对象的设计思想：

- **封装**：把数据和操作数据的函数组织在一起，通过接口访问对象；
- **继承**：子类复用父类的属性和接口，并按需扩展或重写行为；
- **多态**：使用相同接口操作不同对象，由对象决定实际执行的方法；
- **抽象**：提取共同能力，让调用者依赖统一接口而不是具体类型。

本项目使用 Animal、Cat 和 Dog 演示这四个特性。重点不是把 C 语言写成 C++，而是理解面向接口、复用代码和隔离变化的方法。

---

## 项目结构总览

```text
OOP-By-Viys/
├─ CMakeLists.txt
└─ project
   ├─ animal
   │  ├─ CMakeLists.txt
   │  ├─ animal.c        // Animal、Cat、Dog 的实现
   │  └─ animal.h        // 属性、接口和构造/析构函数声明
   └─ app
      └─ main.c          // 创建对象并演示四大特性
```

---

## 四大特性详解

### 封装（Encapsulation）

> 把对象的数据与操作方法组织在一起，外部代码通过接口访问数据。

`ANIMAL_CLASS` 同时包含接口表 `api` 和属性 `attr`：

```c
typedef struct {
    ANIMAL_CLASS_IMPLEMENTS api;
    Animal_Attr attr;
} ANIMAL_CLASS;
```

名称由 `animal_get_name()` 统一读取：

```c
static int animal_get_name(void* t, char* name) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;
    memcpy(name, this->attr.name, strlen(this->attr.name) + 1);
    return 0;
}
```

在 `main.c` 中，不直接访问 `attr.name`，而是通过接口获取：

```c
char name[10] = {0};

animal_cat->get_name(animal_cat, name);
printf("animal name is %s\n", name);
```

这个例子体现的是“接口层面的封装”。由于教学需要，结构体定义仍然放在头文件中；实际项目若需要更严格的数据隐藏，可以进一步使用不透明结构体。

------

### 继承（Inheritance）

> Cat 和 Dog 复用 Animal 已有的接口与属性，只重写自己不同的行为。

派生类与父类保持兼容布局：第一个成员都是 `ANIMAL_CLASS_IMPLEMENTS api`，属性都包含 `name` 和 `sound`。构造 Cat 时，先复制 Animal 的接口与属性：

```c
memcpy(&this->api, t, sizeof(ANIMAL_CLASS_IMPLEMENTS));
memcpy(&this->attr, &parent->attr, sizeof(Cat_Attr));
```

然后只重写 Cat 的 `speak`：

```c
this->api.speak = cat_speak;
```

Dog 的构造过程相同：

```c
DOG_CLASS* DOG_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t);
```

因此，Cat 和 Dog 可以继续使用从 Animal 复用的 `init`、`get_name` 接口与属性，同时拥有自己的 `speak` 实现。

需要注意：C 语言没有原生继承。这里依靠结构体布局和接口复制模拟“继承与方法重写”，这些布局约定必须由开发者共同维护。

------

### 多态（Polymorphism）

> 同一个调用入口，根据传入对象执行不同实现。

`animal_sound()` 只依赖统一接口：

```c
static int animal_sound(void* t) {
    ANIMAL_CLASS_IMPLEMENTS* this = (ANIMAL_CLASS_IMPLEMENTS*)t;
    return this->speak(this);
}
```

调用代码完全相同：

```c
animal_sound(cat);
animal_sound(dog);
```

但运行结果不同：

```text
animal cat say: Meow!
animal dog say: Woof!
```

原因是 Cat 和 Dog 的 `speak` 函数指针分别指向 `cat_speak` 和 `dog_speak`。这就是通过函数指针实现的运行时多态。

------

### 抽象（Abstraction）

> 提取不同动物都具备的能力，为调用者提供统一的行为约定。

`ANIMAL_CLASS_IMPLEMENTS` 定义了所有动物对象可以提供的公共接口：

```c
typedef struct {
    int (*init)(void* t, Animal_Attr attr);
    int (*get_name)(void* t, char* name);
    int (*speak)(void* t);
} ANIMAL_CLASS_IMPLEMENTS;
```

调用者只需要持有 `ANIMAL_CLASS_IMPLEMENTS*`：

```c
ANIMAL_CLASS_IMPLEMENTS* animal_cat = NULL;
ANIMAL_CLASS_IMPLEMENTS* animal_dog = NULL;
```

这样，`main.c` 关注的是“初始化动物、读取名称、让动物发声”，不需要了解每个函数内部如何实现。接口描述“对象能做什么”，具体函数负责“对象怎么做”。

------

## 编译与运行

本项目使用 CMake 构建，不依赖平台专用 API，可在 Windows、Linux 和 macOS 上编译。需要安装：

- CMake 3.15 或更高版本；
- 支持 C99 的 C 编译器，例如 GCC、Clang、Apple Clang、MinGW 或 MSVC。

### 步骤 1：生成构建目录

```bash
cmake -S . -B build
```

### 步骤 2：编译项目

```bash
cmake --build build
```

### 步骤 3：运行程序

Windows（MinGW 等单配置生成器）：

```bash
.\build\oopc.exe
```

使用 Visual Studio 等多配置生成器时：

```bash
.\build\Debug\oopc.exe
```

Linux / macOS：

```bash
./build/oopc
```

macOS 也可以使用 Xcode 生成器：

```bash
cmake -S . -B build -G Xcode
cmake --build build --config Release
./build/Release/oopc
```

代码统一使用标准 C99 接口，并使用 `\n` 输出换行。Windows C 运行库会将其转换为 CRLF，Linux 和 macOS 则保持 LF，无需在源码中进行平台判断。

---

## 运行结果与解释

```text
>
animal name is cat
animal name is dog
animal cat say: Meow!
animal dog say: Woof!
animal cat say: Meow!
animal dog say: Woof!
<
```

- `animal name is ...`：通过 `get_name()` 演示封装；
- 第一组 `say`：Cat 和 Dog 复用 Animal 接口并重写 `speak`，演示继承；
- 第二组 `say`：通过统一的 `animal_sound()` 调用不同实现，演示多态；
- 整个调用过程只依赖 `ANIMAL_CLASS_IMPLEMENTS`，演示抽象。

---

## 扩展练习

可以按照 Cat 和 Dog 的原有格式增加一个 Bird：

1. 在 `animal.h` 中定义 `Bird_Attr` 和 `BIRD_CLASS`；
2. 在 `animal.c` 中实现 `bird_speak()`；
3. 在 `BIRD_CLASS_CTOR()` 中复制 Animal 接口并重写 `speak`；
4. 在 `main.c` 中通过 `animal_sound(bird)` 验证多态。

核心代码可以保持与现有构造函数一致：

```c
BIRD_CLASS* BIRD_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t) {
    BIRD_CLASS* this = (BIRD_CLASS*)malloc(sizeof(BIRD_CLASS));
    ANIMAL_CLASS* parent = (ANIMAL_CLASS*)t;

    memcpy(&this->api, t, sizeof(ANIMAL_CLASS_IMPLEMENTS));
    memcpy(&this->attr, &parent->attr, sizeof(Bird_Attr));
    this->api.speak = bird_speak;

    return this;
}
```

练习目标是：增加 Bird 后，`animal_sound()` 不需要增加 `if/else` 类型判断。

---

## 总结

| 特性 | 在代码中的体现 |
| ---- | -------------- |
| 封装 | 通过 `get_name()` 访问名称，由接口管理对象数据 |
| 继承 | Cat/Dog 构造函数复制 Animal 接口，并重写 `speak` |
| 多态 | `animal_sound()` 使用统一接口，实际行为由函数指针决定 |
| 抽象 | `ANIMAL_CLASS_IMPLEMENTS` 定义所有动物的公共能力 |

本项目展示的是一种轻量级 C 面向对象写法。它适合需要统一接口和可替换实现的场景；如果业务流程简单，普通结构体和函数通常更加直接。
