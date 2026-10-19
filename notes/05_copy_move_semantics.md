# 05 拷贝、移动语义与 Rule of Zero/Five

创建日期：2026-10-14
状态：教学骨架已建立，实验待完成
代码产物：`measurement_packet_demo`
学习路径：**看课 → 做笔记 → 敲代码 → 总结**

## 一、本课目标与前置知识

对照 `00_cpp_learning_roadmap.md` 第 05 项：
实现 `MeasurementPacket` 的拷贝/移动观察实验，记录函数返回和容器扩容时的行为，
最终说明什么时候采用 Rule of Zero，什么时候需要专门设计资源管理操作。

第 02 课的值传递/引用、第 03 课的构造/析构、第 04 课的所有权是前置知识。
已会使用 `std::move(unique_ptr)`，不代表已经理解一般类型的移动语义，本课会把它补全。

完成后应当能够：

- 区分拷贝构造与拷贝赋值、移动构造与移动赋值；
- 区分包对象本身和包内测量数组的存储空间；
- 解释 `const T&`、`T&&` 在本课参数中的含义；
- 说明 `std::move` 本身不搬运资源；
- 实现安全的教学用拷贝、移动与析构日志；
- 区分 C++17 直接返回值构造与 NRVO；
- 观察 `vector` 扩容和 `noexcept` 的关系；
- 给出 Rule of Zero/Five 的合理使用场景。

这仍然是语法与工程训练，不作为独立 SLAM 项目或性能优化成果。

## 二、课程材料与时间

沿用当前翁恺课程选集编号：

- 第 26 节：拷贝构造 I；
- 第 27 节：拷贝构造 II；
- 移动语义、`noexcept`、C++17 拷贝消除、Rule of Zero/Five：由 GPT 结合本课代码补讲。

[翁恺 C++ 视频选集](https://www.bilibili.com/video/BV1dE41167hJ/)

不同平台编号可能不同，以标题匹配。每次看一节并允许暂停、回看；
不要把“看完视频”当成“掌握了概念”。

建议分三次，每次约 90～120 分钟；根据编译与理解情况调整，不必一天做完。

| 次数 | 看课 | 做笔记 | 敲代码 | 总结与本次交付 |
|---|---|---|---|---|
| 1 | 第 26、27 节，分段观看约 40 分钟 | 10 分钟：构造与赋值 | 35～50 分钟：TODO 1、2 | 10 分钟：独立副本、地址和日志 |
| 2 | GPT 讲解移动与 noexcept，20～30 分钟 | 10～15 分钟：源/目标状态 | 45～60 分钟：TODO 3 | 10 分钟：为什么 std::move 不保证移动 |
| 3 | GPT 讲解返回、扩容、Zero/Five，20～30 分钟 | 10 分钟：预测 | 45～60 分钟：TODO 4～6 | 15～20 分钟：验证、复盘和 Git 检查 |

第三次内容偏多，可拆成两次；到点完成一个可运行实验即可暂停。

## 三、代码起点与课程边界

文件位置：

- `exercises/05_copy_move_semantics/main.cpp`
- `exercises/05_copy_move_semantics/CMakeLists.txt`
- 本笔记；配套 `notes/05_chatgpt_study_guide.md` 用于交接。

起点类有两个成员：

- `std::string sensor_name_`：传感器名称；
- `std::vector<double> values_`：若干测量值，标准库负责其动态存储。

普通构造函数和只读/修改/打印接口已经提供。特殊成员函数仅提供注释中的签名，
需亲手逐个实现；没有启用 TODO 时，编译器生成的操作已经可以管理这些成员。

`vector` 本课只用大小、容量、插入和扩容，不展开第 06 课的迭代器课程。
不要求手写裸指针数组、智能指针或内存分配器；不展开完美转发、引用折叠、复杂异常安全。

## 四、概念笔记

### 1. 判断构造还是赋值

判断依据是目标对象是否已经存在，而不是代码里有没有等号。

| 语句（a 已存在） | 操作 | 我的解释 |
|---|---|---|
| `MeasurementPacket b = a;` | 拷贝构造 | 利用已有的对象初始化新对象 |
| `b = a;` | 拷贝赋值 | 把已有对象的值赋给另一个已有对象 |
| `MeasurementPacket c(std::move(a));` | 有匹配移动构造时使用它 | 待填写 |
| `b = std::move(c);` | 有匹配移动赋值时使用它 | 待填写 |

待回答：为什么赋值运算符通常返回 `MeasurementPacket&`？`return *this` 返回谁？

### 2. 值语义、浅拷贝与资源

对本课的 `vector<double>`，拷贝后两个数组各自保存数据。
修改副本不应改变原包。不要因此认为“所有类的默认拷贝都是深拷贝”：
如果成员是裸指针，默认拷贝通常只复制指针值，不复制它指向的数据。
若两份对象又都负责释放该指针，就有重复释放风险。

我的理解：待填写。我的复制独立性实验：待填写。

### 3. 移动与 std::move

`std::move` 将表达式转换为可以参与移动重载选择的右值表达式；
真正的资源转交由被选中的构造/赋值函数完成，它本身不执行转移。
没有可用移动操作时可能调用拷贝，或因具体重载情况而编译失败。

`other` 即使声明为 `T&&`，在函数体中用名字访问时仍是左值表达式。
因此移动成员时需要针对成员使用 `std::move`。

从第 04 课推广时注意：把 `unique_ptr` 移给另一个所有者后，源为空有库保证，
但不能把它推广成“所有移动后的对象都为空”。`string/vector` 移动后通常处于
有效但未指定的状态。可查询大小、析构、重新赋值；取首元素仍必须先确认非空。
对自定义类，应明确自己的移动后约定；本课保持这些标准库成员可安全使用的基本约定。

我的理解、移动前后记录：待填写。

### 4. 特殊成员函数、Rule of Zero/Five

五个相关操作：析构、拷贝构造、拷贝赋值、移动构造、移动赋值。
普通带参构造不属于这“五个”。

- Rule of Zero：资源由 `string/vector/unique_ptr` 等成员管理时，
  通常不需要自己声明这五个函数；是否可拷贝取决于成员本身。
- Rule of Five：一旦为资源语义专门处理某个操作，要一并考虑另外几个。
  可以自己实现、`=default` 或 `=delete`，不是所有类都必须手写五个函数体。
- 只加一个日志析构函数也可能抑制隐式移动；这正是本课需显式补齐移动的原因。

教学用 `MeasurementPacket` 手写它们是为了观察日志；收尾另写
`PlainMeasurementPacket` 验证实际工程中更简洁的默认实现。
依据：[C++ Core Guidelines C.20/C.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-ctor)。

我的选择理由：待填写。

## 五、编码任务

### TODO 1：读懂起点类

先只编译、运行现有骨架。解释：

1. 为什么 `values_(3, 0.1)` 对应三个 0.1？

values_是一个std::vector<double>类型的成员变量。在构造函数的初始化列表中，values_(3,0.1)调用了vector的构造函数；第一个参数表示元素数量，第二个参数表示每个元素的初始值。


2. `this` 和打印出来的 `data` 地址分别指向什么？

this是指向当前对象MeasurementPacket的指针 data指的是values_.data()返回的地址
该地址指向内部测量数组的第一个double元素。

3. 为什么同一个地址出现两行日志不代表构造了两次？
construct | this=0x7fffffffd790 | sensor=IMU | size=3 | data=0x55555556e2c0 | first=0.1
baseline | this=0x7fffffffd790 | sensor=IMU | size=3 | data=0x55555556e2c0 | first=0.1

print()只读取并输出信息，并非重新构造了一个MeasurementPacket对象

4. 当前输出的 TODO 标记为何不是学习任务通过的证据？
这题啥意思 没看懂
Plus：你傻福啊 调用几个成员函数，函数功能是打印TODO标记，就代表学习任务通过了是吗，真是面向对象的编程语言。

产出：记录 baseline 输出，用三句话描述对象、成员和资源。
地址每次运行都可能不同，不硬编码地址。

### TODO 2：拷贝构造、拷贝赋值和析构

在类中实现注释列出的两个拷贝操作和析构函数。

- 构造用初始化列表拷贝成员；赋值更新已有成员并返回自身引用。
- 日志包含 copy construct / copy assign / destroy、目标 `this` 和源 `&other`。
- 不打印整个大数组，不在析构中手动释放 vector 的内存。
- 分别测试定义 `b = a` 和已有对象 `c = a`。
- 改 b 的首值，验证 a 原值未变；使用 `assert` 或明确的检查结果验证。
- 测试 `a = a` 后数值、数量仍正确。

产出：两种操作的日志、独立副本验证、一次自赋值结果。
这个中间阶段只为学习拷贝，先完成它，再补齐 TODO 3。

### TODO 3：移动构造、移动赋值和 noexcept

亲手实现两个移动操作，用 `std::move` 处理 string/vector 成员。

- 新建 b 接收 a 的数据：移动构造；
- 已存在且有数据的 c 接收 b：移动赋值；旧资源交由标准库成员处理；
- 日志区分源和目标；赋值考虑 `this == &other` 并返回自身引用；
- 记录源对象移动后的大小，但不断言它必须为空；
- 给源对象重新赋值，再验证它可以继续使用；
- 测试自移动后仍可析构、重新赋值。

本课这些默认分配器成员的移动支持不抛异常，可在相应操作标注 `noexcept`；
它是承诺，不是“吞掉异常”。不要为了输出效果调用可能分配大内存的日志代码，
也不要打开流的异常抛出设置。标注依据要结合实际成员和函数体。

可选问答：对 const 包调用 `std::move`，为何通常不能匹配这里的 `T&&` 移动函数？

产出：两类移动日志，源的重新赋值验证，`noexcept` 的一句解释。

### TODO 4：函数返回与拷贝消除

实现两个按值返回函数：

- `makeDirectPacket()`：直接返回 `MeasurementPacket("IMU", 3, 0.1)`；
- `makeNamedPacket()`：先定义局部 packet，再 `return packet;`。

在 demoReturn 中分别用返回值初始化新对象。不要返回局部引用，
不要为“优化”改成 `return std::move(packet);`。

比较两种构建（命令见第八节）：

- 正常 C++17 构建；
- 禁用可选拷贝消除的观察构建。

C++17 直接返回同类型纯右值时可直接构造结果，没有可要求打印的中间移动。
命名局部变量的 NRVO 是另一件事，它是允许但不强制的消除。
禁用可选消除不能取消 C++17 强制直接构造行为；不要期待固定日志总数。
参考：[构造与 C++17 保证的拷贝消除](https://learn.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=msvc-170)、
[NRVO 规则](https://eel.is/c++draft/class.copy.elision)。

产出：两种函数、两套构建日志和解释。

### TODO 5：vector 扩容观察

本课使用 `vector<MeasurementPacket>`，容器直接包含包对象。

1. reserve(2)，打印实际 capacity；reserve 不创建包对象。
2. 用 emplace_back 填满实际 capacity。
3. 再插入一个包，记录扩容前后 size/capacity 和 copy/move 日志。
4. 分清“构造新插入元素”与“迁移原有元素”的日志。
5. 比较 push_back(a)、push_back(std::move(a))、emplace_back(构造参数)。

容量增长倍率、各条构造日志的相对顺序依实现而定，不规定一定翻倍，
也不假定 reserve(2) 的结果必为 2。扩容会使原有元素地址/引用/迭代器失效。
实际边界参考：[vector::reserve 规则](https://eel.is/c++draft/vector.capacity)。

移动可能抛异常而拷贝可用时，标准库实现通常会选择拷贝旧元素来保持异常保证。
可选：临时移除移动构造的 noexcept，对比本机输出后恢复；不必真的抛异常。
这不是“没有 noexcept 就绝不移动”的通用规则。
参考：[Core Guidelines C.66](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-move-noexcept)。

产出：一次确定触发的扩容日志与三种插入方式比较。

### TODO 6：回到 Rule of Zero

另建 `PlainMeasurementPacket`，保留 string/vector 数据和必要接口，
不声明那五个特殊成员函数（连日志析构也不声明）。
验证默认拷贝的数据独立性与默认移动后重新赋值可用。

回答：为什么教学类需要操作日志，而这个实际数据类可以更简单？
如果直接封装一个必须 close 的系统句柄，应该把释放和复制/移动策略放在哪里？
只做文字分析，不额外实现文件句柄类。

产出：Plain 类、行为检查与 Rule of Zero/Five 对照总结。

## 六、实验记录与错误整理

每个 TODO 都先写预测，再保存真实输出；下表可以复制扩展。

| 任务 | 我的预测 | 实际输出/错误 | 结论与原因 |
|---|---|---|---|
| 1 普通构造 | 待填写 | 待填写 | 待填写 |
| 2 拷贝 | 待填写 | 待填写 | 待填写 |
| 3 移动 | 待填写 | 待填写 | 待填写 |
| 4 返回值 | 待填写 | 待填写 | 待填写 |
| 5 扩容 | 待填写 | 待填写 | 待填写 |
| 6 Zero/Five | 待填写 | 待填写 | 待填写 |

错误记录：原始报错 → 对应代码 → 原因 → 修改 → 重新编译结果。
尤其记录：只有拷贝日志、忘记 return *this、把构造当赋值、误以为 move 后必为空、
把未扩容当扩容、强行要求返回值产生移动日志。

## 七、常规编译与运行

以下所有命令都在 WSL Ubuntu 的 `/home/luka/projects/cpp-slam-foundations` 中执行。

```bash
cd /home/luka/projects/cpp-slam-foundations
```

作用：进入唯一主开发仓库。

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

作用：读取第 05 课 CMake，生成正常 Debug 构建。

```bash
cmake --build build --target measurement_packet_demo
```

作用：只编译第 05 课，便于聚焦当前错误。

```bash
./build/exercises/05_copy_move_semantics/measurement_packet_demo
```

作用：运行当前已实现的实验；未实现 TODO 的提示不是验收结果。

## 八、返回值对照构建（完成 TODO 3、4 后再用）

仍在 `/home/luka/projects/cpp-slam-foundations` 中执行：

```bash
cmake -S . -B build/lesson05_no_elision -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug -DLESSON05_DISABLE_OPTIONAL_ELISION=ON
```

作用：独立配置 GCC/Clang 的可选消除对照构建，不改变正常构建。
该开关只影响第 05 课目标；[GCC 文档](https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html)
说明 -fno-elide-constructors 不能取消 C++17 必须消除的临时量。

```bash
cmake --build build/lesson05_no_elision --target measurement_packet_demo
```

作用：编译对照版本。

```bash
./build/lesson05_no_elision/exercises/05_copy_move_semantics/measurement_packet_demo
```

作用：与正常版本对照返回值日志，不把单次日志数量当作跨编译器保证。

## 九、收尾检查与验收

以下命令仍在 `/home/luka/projects/cpp-slam-foundations` 执行。

```bash
clang-format --dry-run --Werror exercises/05_copy_move_semantics/main.cpp
```

作用：只检查第 05 课格式。

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --errors-for-leak-kinds=all --track-origins=yes --error-exitcode=1 \
  ./build/exercises/05_copy_move_semantics/measurement_packet_demo
```

作用：检查实现后的实验是否存在内存错误和泄漏。骨架通过不代表最终代码通过。

```bash
git diff --check
git diff
git status --short
```

作用依次为：检查差异空白问题、查看已跟踪文件改动、列出包括新文件在内的状态。
新文件未暂存时不会出现在普通 git diff 中，也需要打开检查；
准备提交后再用 git diff --cached 查看全部暂存内容。

课程完成清单：

- [ ] TODO 1～6 的关键实现由自己完成。
- [ ] 拷贝后修改副本，原包不变；赋值、自赋值结果正确。
- [ ] 移动构造/赋值可观察，源对象可安全重新赋值。
- [ ] 能解释返回日志中的保证与编译器选择。
- [ ] 实际触发一次扩容，正确区分新元素与旧元素。
- [ ] 能解释 noexcept、Rule of Zero/Five。
- [ ] 正常与对照版本编译运行成功；最终 Valgrind 无错误、无泄漏。
- [ ] 笔记有预测、真实输出、错误分析和总结。
- [ ] 检查 diff/status 后创建有意义的学习提交；按需要推送。

## 十、本课总结（自己填写）

1. 最容易混淆的两种操作及区分办法：
2. 哪条真实输出改变了我对 std::move 的理解：
3. 返回局部对象为什么通常不写 return std::move(local)：
4. 数据包在传感器处理流水线中按值返回、复制或转移时各有什么意义：
5. 为什么日志不能证明实际性能一定提升：
6. 下一课学习容器时，我要继续验证的问题：


浅拷贝： 对指针成员只复制地址，两个对象可能共享同一块动态内存。
深拷贝： 为新对象申请独立内存，再复制原对象的数据内容。
const &： 表示通过常量引用接收原对象，避免参数复制，并限制通过该引用修改对象；它本身不负责深拷贝。

写了一个类 要给三个函数：1 default constructor 默认构造函数
2 virtual destructor 虚析构函数 3 copy constructor 拷贝构造函数