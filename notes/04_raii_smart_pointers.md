# 04 动态内存、RAII 与智能指针

日期：2026-10-07  
状态：进行中  
代码产物：`sensor_ownership_demo`

## 一、本课目标

完成本课后，我应该能够：

- 区分局部自动对象与动态分配对象；
- 用“谁负责释放资源”解释所有权；
- 说明裸 `new/delete` 的主要风险；
- 理解 RAII；
- 使用 `std::unique_ptr` 表达独占所有权；
- 使用 `std::move` 转移所有权；
- 使用 `std::make_unique` 创建对象；
- 使用 `vector<unique_ptr<Sensor>>` 管理不同传感器；
- 初步区分 `shared_ptr` 与 `weak_ptr`；
- 使用 Valgrind 检查内存问题。

本课是 C++ 语法和工程练习，不是独立的简历项目。

## 二、与前三课的联系

第 01 课提供：

- 抽象基类 `Sensor`；
- `ImuSensor` 和 `GnssSensor`；
- 虚函数、多态与虚析构函数。

第 02 课提供：

- 地址、指针与 `nullptr`；
- const 引用；
- 指针只能在对象生命周期内使用。

第 03 课提供：

- 构造函数与析构函数；
- 局部作用域；
- 对象生命周期。

本课要回答：

> 当传感器对象需要在运行时创建并保存到容器中时，谁负责最终销毁它？

## 三、课程范围

本课重点：

- 栈与堆的基本区别；
- 所有权；
- RAII；
- `unique_ptr`、`make_unique`；
- `std::move` 的所有权转移；
- `vector<unique_ptr<Sensor>>`；
- `shared_ptr`、`weak_ptr` 的基本用途；
- Valgrind。

本课暂时不深入：

- 自己实现智能指针；
- `new[]/delete[]`；
- 自定义内存分配器；
- placement new；
- 复杂循环引用；
- 拷贝、移动构造函数的完整规则；
- Rule of Zero/Five。

拷贝和移动语义将在第 05 课系统学习。本课只要求理解：

> `std::move` 可以把 `unique_ptr` 的所有权交给另一个对象。

## 四、对应材料与预计时间

翁恺课程：

- 第 11 节：`new & delete`，约 21 分钟。

观看目的是理解动态对象为何必须正确释放，不是鼓励最终代码使用裸 `new/delete`。

建议分三次：

1. 动态内存、所有权与 RAII：70～90 分钟；
2. `unique_ptr` 与容器：90～120 分钟；
3. `shared_ptr`、`weak_ptr` 与 Valgrind：70～90 分钟。

## 五、核心概念

### 1. 自动对象与动态对象

回答：

- 普通局部对象何时构造、何时析构？
- 动态对象为什么容易泄漏？
- “栈对象”和“堆对象”分别想表达什么？
- 对象、指针和动态存储空间是不是同一回事？

我的理解：

> 待填写。

### 2. 所有权

所有权回答：

> 谁负责保证对象最终被正确销毁？

思考：

- 只有一个明确所有者时使用什么？
- 多个模块必须共同延长生命周期时使用什么？
- 只想观察、不想延长生命周期时使用什么？

我的理解：

> 待填写。

### 3. RAII

RAII：Resource Acquisition Is Initialization。

回答：

- 资源如何绑定到对象生命周期？
- 为什么构造函数适合获得资源？
- 为什么析构函数适合释放资源？
- 智能指针离开作用域时会发生什么？

我的理解：

> 待填写。

### 4. unique_ptr

`unique_ptr<T>` 表示独占一个 `T` 对象。

实验验证：

- `unique_ptr` 能否复制？
- 能否移动？
- 移动后原指针是什么状态？
- 离开作用域后，被管理对象是否自动析构？
- 为什么优先使用 `make_unique`？

我的结论：

> 待填写。

### 5. shared_ptr

`shared_ptr<T>` 表示多个所有者共同管理对象。

回答：

- 复制 `shared_ptr` 后引用计数如何变化？
- `use_count()` 表示什么？
- 最后一个所有者销毁时发生什么？
- 为什么不应该默认到处使用 `shared_ptr`？

我的理解：

> 待填写。

### 6. weak_ptr

`weak_ptr<T>` 观察由 `shared_ptr` 管理的对象，但不拥有它。

实验验证：

- 它是否增加 `use_count()`？
- `lock()` 成功时返回什么？
- 对象销毁后 `expired()` 是什么？
- 为什么访问前需要 `lock()`？

我的理解：

> 待填写。

## 六、编码任务

代码：

```text
exercises/04_sensor_ownership/main.cpp
```

文件已经提供 `Sensor`、`ImuSensor`、`GnssSensor` 和 `printSensorData()`。它们来自第 01 课，不是本课主要难点。

### TODO 1：第一个 unique_ptr

要求：

1. 使用 `std::make_unique` 创建 `ImuSensor`；
2. 由 `std::unique_ptr<Sensor>` 管理；
3. 通过智能指针调用成员函数；
4. 增加局部作用域；
5. 运行前预测析构输出；
6. 不写裸 `new/delete`。

回答：

- 智能指针变量本身是什么？
- 它管理的对象是什么？
- 谁负责调用对象的析构函数？

### TODO 2：转移所有权

要求：

1. 创建一个 `unique_ptr`；
2. 临时尝试复制并观察编译错误，然后撤销；
3. 使用 `std::move` 转交所有权；
4. 分别检查移动后两个指针是否为 `nullptr`；
5. 记录移动前后的状态。

这里只理解所有权转移，不展开移动构造函数的完整规则。

### TODO 3：vector<unique_ptr<Sensor>>

要求：

1. 建立保存独占智能指针的 `vector`；
2. 创建 IMU 和 GNSS 对象；
3. 把所有权移入容器；
4. 不在容器外保留第二个所有者；
5. 不把主实验替换成 shared_ptr 容器。

回答：

- 此时传感器的所有者是谁？
- 容器析构时，其元素会发生什么？

### TODO 4：遍历并保持多态

要求：

1. 遍历容器；
2. 使用前判空；
3. 把智能指针指向的对象作为 `Sensor` 引用传给 `printSensorData()`；
4. 验证不同派生类调用不同的 `read()`；
5. 区分智能指针和动态多态各自负责什么。

### TODO 5：shared_ptr 与 weak_ptr

这一部分独立于主容器实验。

要求：

1. 使用 `make_shared` 创建传感器；
2. 复制一次 `shared_ptr`；
3. 记录每一步 `use_count()`；
4. 创建观察它的 `weak_ptr`；
5. 验证 weak_ptr 不增加引用计数；
6. 使用 `lock()` 临时访问；
7. 所有 shared_ptr 销毁后检查 `expired()`。

### TODO 6：作用域与析构顺序

使用额外大括号划分作用域。

运行前预测：

- 哪些对象会被构造？
- 谁拥有每个对象？
- 什么时候开始析构？
- 派生类和基类析构输出是什么顺序？
- 为什么不需要手写 `delete`？

## 七、实验记录

### 1. unique_ptr 移动

预测：

```text
待填写
```

实际：

```text
待填写
```

解释：

> 待填写。

### 2. vector 所有权

预测：

```text
待填写
```

实际：

```text
待填写
```

解释：

> 待填写。

### 3. shared_ptr 与 weak_ptr

```text
use_count 变化：
expired() 变化：
```

解释：

> 待填写。

## 八、编译与运行

以下命令均在仓库根目录执行：

```bash
cd /home/luka/projects/cpp-slam-foundations
```

首次加入第 04 课后：

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

作用：让 CMake 读取第 04 课目录。

编译：

```bash
cmake --build build
```

作用：编译全部练习并显示错误和警告。

运行：

```bash
./build/exercises/04_sensor_ownership/sensor_ownership_demo
```

作用：运行所有权实验。

格式检查：

```bash
clang-format --dry-run --Werror exercises/04_sensor_ownership/main.cpp
```

作用：检查格式，不修改文件。

## 九、Valgrind 验收

```bash
valgrind --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --error-exitcode=1 \
  ./build/exercises/04_sensor_ownership/sensor_ownership_demo
```

目标包括：

```text
definitely lost: 0 bytes
indirectly lost: 0 bytes
ERROR SUMMARY: 0 errors
```

必须实际运行，不虚构结果。

## 十、错误记录

### 错误 1：复制 unique_ptr

- 编译器信息：
- 原因：
- 修正：

### 错误 2：移动后继续解引用

- 现象：
- 原因：
- 正确检查：

### 错误 3：引用计数理解错误

- 我的原理解：
- 实验结果：
- 修正：

## 十一、验收问题

不查笔记回答：

1. 所有权解决什么问题？
2. RAII 如何利用构造和析构？
3. 为什么优先 `make_unique` 而不是裸 `new`？
4. 为什么 `unique_ptr` 不能复制？
5. 移动后原 `unique_ptr` 通常是什么状态？
6. 为什么 `vector<unique_ptr<Sensor>>` 可以同时支持管理和多态？
7. `shared_ptr` 何时销毁对象？
8. `weak_ptr` 为什么不增加引用计数？
9. `lock()` 有什么作用？
10. 智能指针能否自动解决所有悬空引用问题？
11. 为什么没有共享需求时优先 `unique_ptr`？
12. Valgrind 的 `definitely lost: 0 bytes` 表示什么？

## 十二、完成标准

- TODO 1～6 完成；
- 最终代码没有裸 `new/delete`；
- 容器能管理 IMU 和 GNSS；
- 多态调用正确；
- 能解释每个对象的所有者；
- 能解释析构顺序；
- 能区分三种智能指针；
- CMake 配置与编译成功；
- 程序运行成功；
- Valgrind 无泄漏、无错误；
- 笔记记录预测、实际输出和错误；
- 提交前检查 Git diff 与状态。

## 十三、结束前检查

```bash
git diff
git status --short
```

作用：检查本课修改范围，避免遗漏或混入无关内容。

全部验收后，再创建一次有意义的 Git commit。
