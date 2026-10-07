# 03 类、构造函数、初始化列表、析构函数与组合

日期：2026-09-30  
状态：待学习

## 一、本课目标

完成后，我应该能够：

- 区分类的接口与内部状态；
- 解释构造函数何时执行；
- 使用成员初始化列表；
- 解释析构函数与局部对象生命周期；
- 使用组合表达“一个对象拥有另一个对象”；
- 根据输出判断成员对象和外层对象的构造、析构顺序。

本课是 C++ 语法与工程基础练习，不是简历项目。

## 二、预计用时

- 概念与最小示例：20 分钟
- `SensorConfig`：25 分钟
- `ImuSensor` 与组合：30 分钟
- 生命周期实验：20 分钟
- 编译、笔记和 Git 检查：15 分钟

总计约 110 分钟。时间不足时，可以在完成 `SensorConfig` 后暂停。

## 三、核心概念

### 1. 类与对象

待用自己的话补充：

- 类是什么：
类是用来封装变量与函数的工具，很像组成整体项目的一块块基础积木。

Correct：类是一种用户定义的类型，将数据成员和成员函数组织在一起，描述一类对象的状态和行为。

- 对象是什么：
对象是CPP中用来进行操作的变量

Correct：对象是类的具体实例。一个类可以创建多个对象，每个对象拥有自己的非静态成员数据。

- `public` 与 `private` 分别表达什么：
public：类外代码也可以访问
private：只有该类的成员函数和友元函数才可以访问

### 2. 构造函数

待补充：

- 构造函数的名字有什么特点：
构造函数的名字应当与类的名字相同

- 构造函数是否有返回类型：
无返回类型

- 构造函数在什么时候调用：
创建该构造函数所属类的某个对象时，即被调用

- 为什么对象构造完成后就应该处于有效状态：

Correct：构造函数应当建立对象所需的初始状态，使对象满足类规定的约束，后续成员函数才能正常正常使用他。

### 3. 成员初始化列表

待补充：

- 初始化列表写在什么位置：
写在构造函数的位置

- 初始化成员与先默认初始化、再赋值有什么区别：

初始化是建立成员的初始状态；赋值是修改已经存在的成员。

- 为什么 `config_` 应该放在 `ImuSensor` 的初始化列表中：
因为config_的接口 name_与frequency_hz_都是自己私有的，不向外释放权限，所以只能对config_进行初始化列表。

### 4. 析构函数与生命周期

待补充：

- 析构函数的名字有什么特点：
构造函数前加个~

- 局部对象在什么时候析构：
在离开该局部对象所属的{}后进行析构

- 离开作用域后，为什么不能继续使用对象的地址或引用：
对象的生命周期已经结束，指向它的指针和引用变成悬空指针、悬空引用，不能再通过它们正常访问原对象。

### 5. 组合

本课的关系是：

```text
ImuSensor 拥有一个 SensorConfig
```

待补充：

- 为什么这是“has-a”关系：
我的理解是：组合关系是通过类的写法，基于已有的类组合创造出新的类，因此新类对于基础类存在一个依附的关系，也就是"has-a"关系

因为ImuSensor内部包含一个SensorConfig config_成员对象，所以ImuSensor拥有一个SensorConfig，这是一个“has-a”关系。SensorConfig是ImuSensor的组成部分，其生命周期由外层的ImuSensor管理。

- 它和第 01 课的 `ImuSensor is-a Sensor` 有什么不同：
ImuSensor is-a Sensor 表示继承关系：ImuSensor 是一种具体的 Sensor，因此可以在需要 Sensor 的地方使用 ImuSensor。
ImuSensor has-a SensorConfig 表示组合关系：SensorConfig 只是 ImuSensor 拥有的一个组成部分，并不意味着 ImuSensor 是一种 SensorConfig。

## 四、编码任务

代码位置：`exercises/03_classes_lifecycle/main.cpp`

### TODO 1：SensorConfig

1. 定义 `SensorConfig` 类；
2. 添加私有成员 `name_` 和 `frequency_hz_`；
3. 构造函数接收名称和频率；
4. 必须使用初始化列表；
5. 提供只读访问函数；
6. 构造和析构时分别打印信息。

完成后先编译一次，再继续 TODO 2。

### TODO 2：ImuSensor 与组合

1. 内部包含一个 `SensorConfig config_`；
2. 添加 `bool running_`；
3. 构造函数使用初始化列表；
4. `start()` 和 `stop()` 修改状态；
5. `printStatus()` 不修改对象，思考它是否需要 `const`；
6. 构造和析构时分别打印信息。

### TODO 3：作用域实验

在 `main()` 中增加局部作用域，只在其中创建 `ImuSensor`。先写预测输出，再编译运行，不要根据实际结果倒填预测。

### TODO 4：解释顺序

我的预测：

```text
待填写
```

实际输出（与额外作用域相关的部分）：

```text
 === before scope ===
 === inside scope ===
SensorConfig constructed:x frequency_hz:100
ImuSensor constructed x, 100, 0
name:x, frequency: 100, running: 0
name:x, frequency: 100, running: 1
name:x, frequency: 100, running: 0
 === about to leave scope ===
ImuSensor destructed
SensorConfig::~SC()x
 === after scope ===
```


回答：

1. `SensorConfig` 和 `ImuSensor` 谁先构造？为什么？

`SensorConfig` 先构造，然后才执行 `ImuSensor` 的构造函数体。因为 `config_` 是 `ImuSensor` 的成员对象，外层对象完成构造前，必须先完成其成员对象的构造。成员的实际初始化顺序由它们在类中的声明顺序决定，而不是由初始化列表中的书写顺序决定。

2. 两者谁先析构？为什么？

从输出看，先执行 `ImuSensor` 的析构函数体，再析构它的成员对象 `config_`，因此随后才调用 `SensorConfig` 的析构函数。整体遵循与构造相反的顺序：先构造成员、再构造外层对象；先析构外层对象、再按成员声明的逆序析构成员。

3. 析构发生在 `return 0` 时，还是离开额外的大括号时？

发生在离开额外的大括号时。`imu` 是该块作用域内的局部对象，它的生命周期在到达右大括号时结束，不需要等到执行 `return 0`。

4. 若在作用域外保留对象指针，为什么不能继续解引用？

一旦离开作用域，指针所指对象的生命周期已经结束，该指针会成为悬空指针。悬空指针通常仍保留原来的地址，并不会自动变成空指针；继续解引用会产生未定义行为。

## 五、编译与运行

以下命令均在 `/home/luka/projects/cpp-slam-foundations` 中执行。

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

作用：重新读取项目结构并生成 Debug 构建文件。

```bash
cmake --build build
```

作用：编译全部练习，检查语法错误和警告。

```bash
./build/exercises/03_classes_lifecycle/imu_buffer_demo
```

作用：运行第 03 课，观察输出顺序。

```bash
clang-format --dry-run --Werror exercises/03_classes_lifecycle/main.cpp
```

作用：检查格式，不修改文件。

## 六、错误记录

- 编译器信息：
- 我的原代码：
- 原因：
- 修正：

## 七、本课小结

不查笔记回答：

1. 构造函数和普通成员函数的两个区别是什么？

   构造函数与类同名且没有返回类型；它在创建对象时自动调用，用来建立对象的初始有效状态，而普通成员函数通常由代码显式调用，用来完成对象创建后的操作。

2. 初始化列表解决了什么问题？

   初始化列表在进入构造函数体之前直接初始化成员，避免先默认初始化再赋值。对于引用成员、`const` 成员以及没有默认构造函数的成员对象，必须使用初始化列表。

3. 析构函数由谁调用？

   析构函数通常由 C++ 在对象生命周期结束时自动调用。例如，局部对象离开作用域时会自动析构，不需要手动调用析构函数。

4. 成员对象和外层对象的构造、析构顺序是什么？

   构造时先构造成员对象，再执行外层对象的构造函数体；析构时先执行外层对象的析构函数体，再按声明顺序的逆序析构成员对象。

5. 组合与继承分别适合描述什么关系？

   组合描述“has-a”关系，即一个对象拥有另一个对象作为组成部分；继承描述“is-a”关系，即派生类是一种基类，并可在需要基类的地方使用。

我的总结：

> 类把状态和行为组织在一起，构造函数负责让新对象从一开始就处于有效状态，析构函数负责在对象生命周期结束时进行清理。成员初始化列表用于直接初始化成员；当类中包含其他类的对象时，成员对象先构造、后析构。组合适合表达“拥有”，继承适合表达“是一种”。理解作用域和生命周期后，也能判断对象何时失效，并避免解引用悬空指针。

## 八、结束前检查

以下命令均在 `/home/luka/projects/cpp-slam-foundations` 中执行。

```bash
git diff
git status --short
```

作用：检查本课修改和待提交文件。代码可编译运行、笔记填写并解释输出后，再创建本课 commit。




void Person :: sayHello(){ //定义类Person中sayHello函数中的作用


}

Fileds 成员变量
parameters 参数
Local variables 局部变量

private 只有类的成员函数可以访问这些成员变量，或者成员函数
public 任何人都能访问
protected 这个类的子类可以访问

同一个类之间可以互相访问私有变量 private权限限制只在编译时刻 

构造函数与类的名字相同 并且没有返回类型
析构函数没有参数 

组合Composition 软件重用 用已有的对象来制造出新的对象出来
Such as we use "engine" and "tyre" to create a "car"
