# 06 string、array、vector 与迭代器

创建日期：2026-10-23

更新日期：2026-10-28

状态：四个实验已有实现，本次已编译运行并核对主要输出；已补充八股问答与复习索引。
原有验收清单保留用于学习者自查，不代表所有工程检查都已完成。

程序：`imu_sequence_demo`

学习路径：**看课 → 做笔记 → 敲代码 → 总结**。

## 复习导航

复习时直接点击知识点；第四至七节保留实验步骤，第十一节集中回答常见面试问题。

| 想复习什么 | 直接跳转 |
|---|---|
| string / array / vector 选型、连续存储 | [问答 1～2](#review-containers) |
| array 初始化、固定长度与 const | [问答 3](#review-array-init) |
| string 拼接、find / npos、中文长度、c_str | [问答 4～5](#review-string) |
| 下标、at、front / back、空容器 | [问答 6](#review-access) |
| size / capacity、reserve / resize、clear / shrink_to_fit | [问答 7～9](#review-capacity) |
| 范围 for、auto / auto& / const auto&、复制与引用 | [问答 10～11](#review-range-for) |
| begin / end、迭代器与指针、const_iterator | [问答 12～13](#review-iterators) |
| erase 返回值、相邻删除、范围 for 中修改容器 | [问答 14～15](#review-erase) |
| 扩容、失效规则、索引与元素身份 | [问答 16～17](#review-invalidation) |
| 时间复杂度、push_back / emplace_back | [问答 18～19](#review-complexity) |
| 均值函数、浮点计算、std::abs | [问答 20](#review-mean) |
| 三分钟自测与实际运行记录 | [第十二节](#review-check) / [第十三节](#review-run) |

## 一、这节课到底要做什么

你收到一小段 IMU 测量，需要给传感器起名字，保存三轴加速度，
算出这段数据的平均值，再去掉明显异常的两条采样。

这就是本课的主线。每个新知识都有一个具体用途：

| 代码实验 | 要解决的问题 | 本段知识 |
|---|---|---|
| TODO 1 | 名称怎么保存？一次测量的三个轴怎么保存？ | string、array、下标访问 |
| TODO 2 | 六条采样怎么放在一起？平均值是多少？ | vector、size/capacity、范围 for |
| TODO 3 | 两条坏数据挨着，怎样都删掉？ | 迭代器、begin/end、erase |
| TODO 4 | 存储空间换了，原来取到的位置还能用吗？ | 扩容、引用/指针/迭代器失效 |

先做 TODO 1，再做下一个。四个实验对应路线图第 06 课的全部核心范围，
不要求一次读懂后面所有规则。

第 05 课已经观察“复制/移动一个对象”；本课开始使用容器组织和处理多个对象，
不再添加拷贝构造、移动构造或析构日志。

## 二、时间与材料怎么安排

建议分两次，每次约 90～120 分钟，也可以每次只完成一个实验：

- 第一次：TODO 1（约 35～45 分钟）＋ TODO 2（约 50～70 分钟）。
- 第二次：TODO 3（约 40～60 分钟）＋ TODO 4（约 30～45 分钟）＋ 简短总结。

每段都先讲清问题，再写两三条笔记，然后敲代码并运行。
调试超时就停在当前实验，下一次接着做。

翁恺原课程第 41 节“STL 简述”可选看作背景；它并非本课四个实验的完整教程，
路线图把 STL 算法主要安排在第 08 课，本课不需要等看完大量新视频再动手。
找不到同样的选集编号时，按标题找；也可以先由 GPT 讲解当前实验的必需语法。

## 三、代码里已经提供了什么

文件：

- `exercises/06_imu_sequence/main.cpp`
- `exercises/06_imu_sequence/CMakeLists.txt`
- 本文：学习说明、填写笔记和记录实验。

一个 ImuSample 表示“一次采样”，有两个字段：

- `timestamp_s`：时间，单位秒；
- `acceleration_mps2`：三个 double，依次是 ax、ay、az，单位 m/s²。

它写成 struct，只是让这两个字段可以直接访问，仍是一个 C++ 类类型。
本课不用另写构造函数；字段已经设置默认值，默认采样是时间 0、三个轴全为 0。

辅助函数已给出：

| 函数 | 作用 |
|---|---|
| makeTrainingSamples | 给每个实验提供同样的六条人工数据 |
| printAxes / printSequence | 打印数据，便于观察 |
| meanAcceleration | 只读接收采样序列，通过引用输出三轴均值，空序列返回 false |
| removeOutliers | 用迭代器删除超阈值采样，接收 erase 返回的位置继续遍历 |

四个 demo 函数各自使用局部数据，前一个实验的改动不会影响后一个。
初始骨架要求亲手补齐求均值和删除等逻辑；当前代码已包含这些实现。
下文 TODO 是原实验编号，可用于重新练习，不表示对应实现仍为空。

## 四、TODO 1：保存名称与三轴数据

**本段问题：名称是字符，三个轴是数字，各用什么保存？**

### 先看懂

- string：一串字符，例如 `"imu"`。本实验名称只用 ASCII 字符。
- array<double, 3>：固定存放三个 double，适合每次测量都有三个轴。
- vector 留到下段，它适合数量会变化的一整段采样。

固定长度不等于只读：array 不能追加第四个元素，但能改变第一个元素的值。
array/vector 都属于顺序容器，按位置保存元素。string 是字符串类，
可以用相似的方式访问字符。deque、list、forward_list 也是顺序容器，本课只认识名字。

### 再敲代码

在 demoNameAndAxes 中依次做：

1. 给 sensor_name 追加 `"_01"`，打印名称、size()、empty()。
2. 拼出另一个字符串 `"sensor=imu_01"`。
3. 用 find("imu") 查找；用是否等于 string::npos 判断没找到。
4. 读取 axes.at(0)，把它改为 0.15，再打印三轴和 axes.size()。
5. 解释 axes[0] 与 axes.at(0) 的区别：下标从 0 开始，at 会检查是否越界。

注意实际范围：三个轴的合法下标是 0、1、2。
名称非空时才能读取首字符；string 的 size 统计 char 数，本例的 ASCII 名称正好是字符个数，
不能据此认为 UTF-8 中文也“一字等于一个 char”。

**核对结果：** 名称 imu_01，长度 6，非空；array 长度仍是 3，ax 变为 0.15，另两轴不变。

我的笔记：

- 为什么三轴用 array：
因为array是固定数组，用于存储已知数量的数据。所以用 std::array<double, 3> acceleration{}
其中{}代表默认初始化值为0

补充：这里准确说是用空花括号进行列表初始化，三个 double 都初始化为 0。
局部变量若只写 `std::array<double, 3> acceleration;`，其元素不会自动清零，不能直接读取。

- 为什么改值以后 size 不变：
- 我对 string 拼接/查找的理解：
- 实际输出与遇到的问题：

## 五、TODO 2：保存六条采样，再计算均值

**本段问题：采样数量可以变化，怎样把它们依次存起来？**

先记两个量：

- size：当前实际有多少条采样。
- capacity：当前已有空间最多能放多少条，而不需要重新分配空间。

例如“已有 6 条、还能不扩容放到 8 条”，size 是 6，capacity 是 8。
这是例子，程序中的 capacity 必须实际打印，不规定每次都按两倍增长。

### A. 建立序列

在 demoVectorAndMean 中：

1. 打印空 samples 的 size/capacity。
2. reserve(8)，再打印一次；确认 size 仍为 0、capacity 至少为 8。
3. 用范围 for 读取 source，把每条采样 push_back 到 samples。
4. samples 非空后，用 at(0)、front()、back() 读取第一条和最后一条。

范围 for 可以先照着这个语法读：

```cpp
for (const ImuSample& sample : source) {
    // sample 是当前读到的那条采样，在这里保存它。
}
```

const 引用表示只读且不复制每条采样；push_back 会另外把该采样保存进 samples。
只修改现有元素可以用 ImuSample&；按值循环修改的是副本。
范围 for 期间不要通过插入、删除来改变正在遍历的 vector 的结构。

### B. 分清 reserve 与 resize

另外复制一份 `probe = samples`，在 probe 上逐个操作并打印：

| 操作 | 预计 size | 要看懂的事情 |
|---|---:|---|
| 刚复制完 | 6 | 数据独立，capacity 不要求与 samples 相同 |
| resize(2) | 2 | 后四个元素被移除 |
| resize(4) | 4 | 新增两条默认采样，时间和加速度全为 0 |
| pop_back() | 3 | 删除最后一条；调用前确认非空 |
| clear() | 0 | 清空全部元素，不意味着释放所有预留空间 |

reserve 只预留空间；resize 会改变实际元素数。不能 reserve 后直接用 [0] 写入不存在的元素。
对同一个 probe，这里的缩小和 clear 都不会减小 capacity。
samples 保留完整的六条数据，用来计算均值。

### C. 计算三轴均值

完成 meanAcceleration：

1. mean 三个分量从 0 开始。
2. 用范围 for 累加每条采样的 ax、ay、az。
3. 三个总和各除以 samples.size()。
4. 计算成功返回 true。

函数签名中的 samples 是只读输入，mean 引用是输出。
空序列已经返回 false；此时调用方显示“没有数据”，不能做除以零的计算。
将非空部分实现后，再用空 vector 调用一次，确认不会尝试读首元素或做除法。

**核对结果：** 六条采样的均值约为 [-1.583333, -0.033333, 9.8]。
程序保留三位小数显示 [-1.583, -0.033, 9.800]。数值检查可使用约 1e-6 的误差，
不要要求浮点计算得到的结果与十进制字面量绝对相等。

我的笔记：

- reserve 和 resize 的区别：
- const ImuSample& 与按值循环的区别：
- 六条数据的实际均值：
- 空序列返回了什么：
- 实际输出与遇到的问题：

## 六、TODO 3：用迭代器删除两条相邻异常

**本段问题：vector 中间有坏数据，删掉后下一步应该检查谁？**

本课的练习规则：`std::abs(ax) > 10.0` 判为异常。
这只是人工数据的教学规则，不代表真实 IMU 异常检测方法。
makeTrainingSamples 中 t=0.02、0.03 的 ax 分别是 50、-60，恰好相邻。

### 先认识四个写法

| 写法 | 读作 |
|---|---|
| samples.begin() | 第一个元素的位置 |
| samples.end() | 最后一个元素之后的位置，不能读取它指向的元素 |
| *it / it->timestamp_s | 读取当前位置的采样 / 访问它的字段 |
| ++it | 走到下一个位置 |

迭代器可以理解为“沿容器找到元素的位置工具”，不是所有迭代器都是裸指针。

在此贴入迭代器与指针的具体差别：

| 对比项                  | 指针                                 | 迭代器                                                |
| ----------------------- | ------------------------------------ | ----------------------------------------------------- |
| 主要用途                | 保存对象地址，通过地址访问对象       | 按容器提供的规则定位、访问和遍历元素                  |
| 当前例子的类型          | `ImuSample*`                         | `std::vector<ImuSample>::iterator`                    |
| 如何取得首元素位置      | 非空时 `&samples.at(0)`              | `samples.begin()`                                     |
| 如何取得当前元素        | `*p`                                 | `*it`                                                 |
| 如何访问成员            | `p->timestamp_s`                     | `it->timestamp_s`                                     |
| 如何前进                | 在有效数组范围内，`++p` 前进一个元素 | `++it` 按该容器的遍历规则前进                         |
| 是否都支持 `+ 2`        | 数组指针在合法范围内支持             | 取决于迭代器种类；`vector` 支持，`list` 不支持        |
| 没有指向对象／遍历结束  | 可以用 `nullptr` 表示空指针          | 用容器的 `end()` 判断遍历结束，不能统一当成 `nullptr` |
| `vector` 重新分配空间后 | 指向旧元素的指针失效                 | 指向旧元素的迭代器失效                                |

空序列的 begin() 等于 end()，也没有可读取的第一个元素。
`std::vector<ImuSample>::iterator` 是这个 vector 的可修改迭代器类型。
这里先读懂具体类型，不要求顺便学完整的模板章节；只读场景也可用 const_iterator/cbegin/cend。

### 再敲代码

1. 先在 demoIteratorAndErase 中用显式迭代器遍历，打印时间和 ax。
2. 再到 removeOutliers 实现删除。
3. 当前采样异常时，用 `it = samples.erase(it)` 接收返回的位置。
4. 当前采样正常时，才执行 ++it。
5. 循环中重新与 samples.end() 比较，不缓存删除前的 end。

为什么？删掉当前元素后，后面的元素会向前补位。
erase 返回下一个仍可检查的位置；此时再无条件 ++it，就可能漏掉相邻异常。
如果删的是最后一个元素，返回的是新的 end()，此时结束遍历，不能再解引用。
erase 之后，原位置以及后面的旧迭代器、引用不再用于继续定位，改用返回结果。

删除完成后，沿用 TODO 2 的 meanAcceleration 重新计算均值。

**核对结果：** 剩 4 条，时间依次为 0.00、0.01、0.04、0.05；
新均值为 [0.125, -0.05, 9.8]。

再检查三种边界情况，仍放在本实验里：

- 空序列：删除后仍为空，meanAcceleration 返回 false。
- 全部正常：比如直接使用上面已经清理好的序列，再删一次，仍为 4 条。
- 全部异常：只放原始第 3、4 条采样，删除后为空。

我的笔记：

- begin 与 end 的区别：
- 为什么删除分支不能再无条件 ++it：
- 删除前后数量与均值：
- 三种边界情况的结果：
- 实际输出与遇到的问题：

## 七、TODO 4：扩容以后重新获取元素

**本段问题：vector 为了容纳更多数据换了存储空间，旧位置还能代表原元素吗？**

第 05 课看到过扩容时的拷贝/移动日志。本次不再研究构造日志，
只观察“如何继续安全访问元素”。

在 demoReallocation 中：

1. 保存有效索引 saved_index=0，打印首条采样的时间、数据和地址。
2. 在一个内部大括号中取得首条的引用、指针、迭代器，扩容前读取它们。
3. 离开该大括号，结束这几个局部变量的使用。
4. 保存 old_capacity，再调用 reserve(old_capacity + 1)，确保容量确实需要增长。
5. 打印新的 capacity，再按 saved_index 用 at 重新访问首条数据。
6. 对照前后输出：size 还是 6，原有数据没有改变，但原来的访问位置需要重新获取。

观察扩容前打印的地址文本即可，不使用失效的指针、引用或迭代器比较或读取数据。
索引在这个“只 reserve、不插入删除”的实验中仍能定位同一条采样；
若中间插入/删除导致元素移位，不能把索引当成永久身份编号。

### 本课需要记住的失效规则

| 操作 | 对已取得的引用/迭代器有什么影响 |
|---|---|
| reserve 确实扩容，或插入导致扩容 | 所有元素的旧引用、指针、迭代器和旧 end 都失效 |
| reserve 没有扩容 | 保持有效 |
| push_back 没有扩容 | 原有元素仍可访问，但旧 end 失效 |
| erase 删除元素 | 删除位置及其后的引用、迭代器失效，使用 erase 的返回值继续 |
| resize 增大且扩容 | 全部旧访问位置失效，按需要重新获取 |
| resize 增大但不扩容 | 原有元素仍可访问，但旧 end 失效 |
| resize 缩小、pop_back | 保留的元素仍可访问；被移除的元素和旧 end 不再使用 |
| clear | 所有原元素的访问位置和旧 end 都失效 |

size 变了不等于一定扩容；没有扩容也不等于所有迭代器都仍然有效。
不要用“程序没崩溃”证明旧迭代器可用，判断依据是相应操作的规则。

我的笔记：

- 实际扩容前后的 size/capacity：
- 为什么保存索引后重新获取：
- 不扩容的 push_back 有什么特殊点：
- 为什么 erase 即使没扩容，也要换用返回的迭代器：
- 实际输出与遇到的问题：

## 八、编译和运行

以下所有命令都在 WSL Ubuntu 的 `/home/luka/projects/cpp-slam-foundations` 中执行。

```bash
cd /home/luka/projects/cpp-slam-foundations
```

作用：进入主仓库。

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

作用：首次加入第 06 课后读取新构建配置。

```bash
cmake --build build --target imu_sequence_demo
```

作用：编译第 06 课。

```bash
./build/exercises/06_imu_sequence/imu_sequence_demo
```

作用：运行四段实验。完成一个实验就编译、运行一次，只核对刚完成的那段。
文中“核对结果”是对人工数据的预期，不是替你记录的实际运行结果。

## 九、收尾与验收

仍在 `/home/luka/projects/cpp-slam-foundations` 中执行：

```bash
clang-format --dry-run --Werror exercises/06_imu_sequence/main.cpp
```

作用：检查本课格式。

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --errors-for-leak-kinds=all --track-origins=yes --error-exitcode=1 \
  ./build/exercises/06_imu_sequence/imu_sequence_demo
```

作用：检查完成后的程序有无内存错误和泄漏。
内存检查通过仍需结合数值与边界检查确认逻辑正确。

```bash
git diff --check
git diff
git status --short
```

作用依次为检查差异格式、阅读已跟踪文件修改、列出全部待提交文件。
新文件需要打开阅读；用户决定暂存后，再用 git diff --cached 检查暂存内容。

完成标准：

- [ ] TODO 1：名称操作正确，三轴 array 长度固定但值可修改。
- [ ] TODO 2：分清 reserve/resize，保存六条数据，均值正确，空序列处理正确。
- [ ] TODO 3：相邻异常全部删除，正常/空/全异常三种边界正确。
- [ ] TODO 4：确实触发扩容，重新获取元素，没有使用失效访问位置。
- [ ] 能用自己的话解释范围 for、显式迭代器和失效规则。
- [ ] 自己记录输出、错误原因和总结，编译与内存检查完成。
- [ ] 检查 diff/status 后形成一次有意义的学习提交，再按需要 push。

每个实验只写两三句话总结：我做了什么、输出是什么、说明了什么。
不用为了填写所有术语而延迟动手。

## 十、给 GPT 的辅导说明与按需参考

换对话时可上传本文并说明当前 TODO。接手的 GPT 请：

- 围绕当前一个实验讲解，一次只推进一个检查点。
- 使用本文同一组数据和字段，不另造复杂类或重写第五课内容。
- 先解释语法和预期，再让学习者填写核心代码，不一次给四段完整答案。
- 用户贴出代码后解释具体错误；保留其已有笔记和真实输出。
- 不假定能读取用户 WSL 文件，不虚构编译或实验结果。
- 算法、lambda 留到第 08 课；本课使用循环理解迭代器删除。

以下供核对，不要求学生先读完：

- [Microsoft：array](https://learn.microsoft.com/en-us/cpp/standard-library/array-class-stl?view=msvc-170)
- [Microsoft：vector](https://learn.microsoft.com/en-us/cpp/standard-library/vector-class?view=msvc-170)
- [C++ 工作草案：vector 容量](https://eel.is/c++draft/vector.capacity)
- [C++ 工作草案：vector 插入和删除](https://eel.is/c++draft/vector.modifiers)

本仓库使用 C++17；工作草案中的较新接口不作为本课要求。

<a id="review-containers"></a>

## 十一、八股问答速查（C++17）

### 1. string、array、vector 怎么选？与原生数组有什么区别？

| 类型 | 长度与用途 | 本课对应 | 需要记住 |
|---|---|---|---|
| `std::string` | 可变长度的字符序列 | `sensor_name` | 自己管理字符存储，支持拼接、查找 |
| `std::array<T, N>` | 编译期确定 N 个元素 | 三轴加速度 | N 是类型的一部分，不能 push_back |
| `std::vector<T>` | 运行时可改变元素数量 | 一段 IMU 采样 | 支持追加、删除，注意扩容和失效 |
| `T[N]` | 固定长度的原生数组 | 本课未采用 | 不能整体赋值，在许多表达式中会退化为指针 |

`std::array<double, 3>` 和 `std::array<double, 4>` 是不同类型。
array 可以整体拷贝、赋值，也提供 size、at 和迭代器；传参时不会自动退化成首元素指针。
传 `const std::array<double, 3>&` 可避免复制，同时保留长度信息。

### 2. 连续存储是什么意思？array 一定在栈上，vector 一定在堆上吗？

本课 array、vector 的元素连续排列，string 的字符也连续存储。
连续存储支持按下标 O(1) 访问，遍历通常具有较好的缓存局部性；它不保证元素地址永远不变。
`vector<bool>` 是特殊化，不能把普通 vector 的元素引用、存储结论直接套过去。

不能只按容器名字判断“栈还是堆”：array 的元素属于 array 对象本身，位置取决于该对象在哪里；
局部 vector 对象与它管理的元素缓冲区是两件事，默认分配器下元素使用动态分配的存储。
vector 析构时会销毁元素并释放自己管理的存储，不需要手写 delete。

<a id="review-array-init"></a>

### 3. array 长度固定是不是不能修改？花括号初始化有什么用？

固定的是元素数量，能否改值取决于 const。

```cpp
std::array<double, 3> a{};          // 三个元素都是 0
std::array<double, 3> b{0.1};       // 第一个为 0.1，其余为 0
std::array<double, 3> c;            // 局部变量：元素未初始化，不能直接读取
a.at(0) = 0.15;                    // 合法，size 仍为 3
const std::array<double, 3> fixed{}; // 不能通过 fixed 修改元素
```

本课 ImuSample 的两个成员有 `{}` 默认成员初始化器，因此新增默认采样的时间和三轴为 0。
不能推导成“所有类的默认构造都会把所有字段清零”。

参考：[C++17 草案：array](https://timsong-cpp.github.io/cppwp/n4659/array.overview)。

<a id="review-string"></a>

### 4. string 的 +、+=、find 和 npos 分别怎么用？

`+` 产生拼接结果；`+=` 在当前字符串后追加。`find` 返回第一次匹配的起始下标，未找到返回 `std::string::npos`。

```cpp
std::string name{"imu"};
name += "_01";
std::string label = "sensor=" + name;
auto pos = name.find("imu");
if (pos != std::string::npos) {
    // 找到了；本例 pos 为 0。
}
```

不要写 `if (name.find("imu"))`：在开头找到返回 0，反而会被当成 false；未找到的 npos 又不是 0。
不要用 `pos >= 0` 判断找到，因为返回的 size_type 是无符号类型。
两个字符串字面量不能直接写 `"sensor=" + "imu"`；先让一个操作数成为 string，或写相邻字面量。

### 5. string::size 是字符数吗？c_str 返回的指针由谁管理？

size/length 返回 char 元素数量，不包含末尾的空字符 `\0`。
本课 ASCII 名称 `imu_01` 的 size 是 6；UTF-8 中文通常占多个字节，不能直接把 size 当成人眼看到的字数。
string 可以存储中间带 `\0` 的内容，C 风格字符串函数却通常把第一个 `\0` 当结束。

`c_str()` 提供以空字符结尾的字符序列指针，string 仍拥有这份存储，调用者不能 delete 它。
字符串销毁或发生会使该指针失效的修改后，旧指针不能继续使用；需要时在修改后重新取得。
不要假定所有 string 的失效规则都与 vector 完全相同。

<a id="review-access"></a>

### 6. [] 和 at 有什么区别？空容器能调用 front/back 吗？

对本课 array/vector，合法元素下标必须满足 `0 <= i < size()`。
`operator[]` 不提供标准要求的越界异常检查，越界访问是未定义行为；`at(i)` 越界抛出 `std::out_of_range`。
未定义行为不等于“一定崩溃”，也可能暂时表现正常。

vector/string 的 front、back、pop_back 都要求非空；先检查 empty。
`erase(end())` 也不合法，因为单元素 erase 需要一个能解引用的元素位置。

C++17 string 有一个特殊边界：读取 `s[s.size()]` 得到空字符，但不能把这个位置写成普通字符；
`s.at(s.size())` 仍抛出异常。它不属于字符串正文，更不能把该规则套给 vector。
参考：[C++17 草案：string 元素访问](https://timsong-cpp.github.io/cppwp/n4659/string.access)。

<a id="review-capacity"></a>

### 7. size 和 capacity 有什么区别？reserve 以后能直接写下标吗？

size 是已经存在的元素数；capacity 是不重新分配存储时最多可容纳的元素数，始终有 `size <= capacity`。
访问边界看 size，不看 capacity。

```cpp
std::vector<ImuSample> samples;
samples.reserve(8);       // size 为 0，capacity 至少为 8
// samples[0] = {};      // 错误：第 0 个元素还不存在
samples.push_back({});   // 现在 size 为 1，可以访问 samples.at(0)
```

reserve 不创建新元素。`reserve(n)` 只有 n 大于当前 capacity 才重新分配；较小的 n 不会缩容。

### 8. reserve 和 resize 有什么区别？resize 增大的元素是什么？

| 操作 | size | 元素内容 | capacity |
|---|---|---|---|
| `reserve(n)` | 不变 | 不增加、删除元素 | 必要时增大到至少 n |
| `resize(n)`，n 较小 | 变成 n | 销毁尾部元素 | 不减小 |
| `resize(n)`，n 较大 | 变成 n | 默认插入新增元素 | 不足时扩容 |
| `resize(n, value)`，n 较大 | 变成 n | 新增元素是 value 的副本 | 不足时扩容 |

本课默认分配器下，`vector<double>` 新增默认元素为 0，`vector<ImuSample>` 新增默认采样的成员也为 0。
已有元素不会因为 resize 增大而被重置。缩小再增大创建的是新元素，不能恢复之前被删除的数据。

### 9. clear 会释放容量吗？shrink_to_fit 一定能缩到 size 吗？

clear 销毁所有元素，使 size 为 0，但保留 capacity，适合清空后复用缓冲区。
容量还在不表示旧元素仍然存活，不能继续用清空前取得的引用或迭代器访问。

`shrink_to_fit()` 是请求减少多余容量，标准不保证一定执行，也不保证调用后 capacity 等于 size。
如果发生重新分配，原来的引用、指针、迭代器和 end 都失效。
容器析构会释放其拥有的存储，但这不等于操作系统显示的进程占用会立即下降。

容量规则参考：[C++17 草案：vector capacity](https://timsong-cpp.github.io/cppwp/n4659/vector.capacity)。

<a id="review-range-for"></a>

### 10. 范围 for 中 auto、auto&、const auto& 有什么区别？

以下针对本课 `vector<ImuSample>`：

| 写法 | 是否复制每条采样 | 修改 sample 是否影响容器元素 |
|---|---|---|
| `for (auto sample : samples)` | 是 | 不影响，只修改副本 |
| `for (auto& sample : samples)` | 否 | 影响原元素 |
| `for (const auto& sample : samples)` | 否 | 不能通过 sample 修改原元素 |

只读遍历用 const 引用；修改现有采样用引用；需要独立副本才按值。
同样，`auto first = samples.front()` 得到副本，`auto& first = samples.front()` 才引用原元素。
auto 负责类型推导，不会自动替你选择“避免拷贝”。

### 11. const 引用遍历再 push_back，会不会没有发生复制？

仍然会复制进目标容器。`const ImuSample& sample` 只避免创建循环变量时的复制；
`samples.push_back(sample)` 用这个左值在目标容器中拷贝构造一条新采样。
source 与 samples 中的采样独立，修改目标的三轴不会修改来源。

`std::vector<ImuSample> probe = samples;` 也复制元素，capacity 不必复制成相同值。
本课 ImuSample 内部只有 double 和 array，因此复制后数据独立；若元素是裸指针，复制的是地址，
不能把这条结论推广成“vector 会深拷贝指针所指对象”。

<a id="review-iterators"></a>

### 12. begin/end 是什么？迭代器为什么不等于指针？

容器使用左闭右开区间 `[begin(), end())`：begin 指向首元素，end 表示尾后位置，不能解引用。
空容器满足 begin 等于 end；非空 vector 可用 `end() - 1` 定位末元素。

迭代器是统一访问和遍历元素的抽象，可能由指针实现，也可能是包装类型。
vector 的迭代器支持随机访问，可以 `it + n`、`it[n]`、计算同一容器有效迭代器间的距离；
list 的迭代器不能 `it + n`，只能逐步前进或后退。
不要对不同容器的迭代器做距离运算，也不要递增 end。

本课 `*it` 是当前 ImuSample，`it->timestamp_s` 等价于 `(*it).timestamp_s`。

### 13. const_iterator 与 const iterator 有什么区别？

一个限制通过迭代器改元素，一个限制迭代器变量本身移动，不能混为一谈。

```cpp
// 假设 samples 是非空、非 const 的 vector<ImuSample>。
auto read_it = samples.cbegin();     // const_iterator：可以 ++read_it
// read_it->timestamp_s = 1.0;       // 错误：不能通过它修改元素
const auto fixed_it = samples.begin(); // const iterator：不能 ++fixed_it
fixed_it->timestamp_s = 1.0;         // 可以：所指元素本身并非 const
```

const 容器的 begin 返回只读迭代器；非 const 容器也能主动用 cbegin/cend 获得只读访问。
const 不会防止其他代码修改容器并导致迭代器失效。

<a id="review-erase"></a>

### 14. 为什么删除时必须接收 erase 的返回值？为什么不能总是 ++it？

erase 返回删除位置之后的元素在新序列中的迭代器；没有后续元素时返回新的 end。
删除位置及其后的旧迭代器、引用失效，不能再对原 it 做递增来继续遍历。

```cpp
for (auto it = samples.begin(); it != samples.end();) {
    if (std::abs(it->acceleration_mps2.at(0)) > threshold) {
        it = samples.erase(it);
    } else {
        ++it;
    }
}
```

删除 50 对应采样后，-60 对应采样补到当前待检查的位置。
若接住返回值后又无条件 ++it，就跳过了 -60；若删除末元素后递增返回的 end，还会违反迭代器规则。
循环条件每次重新取 end，不能缓存删除前的尾后迭代器。

### 15. 为什么不能在范围 for 中随意 push_back 或 erase？只改元素值可以吗？

C++17 范围 for 会先取得遍历使用的 begin 和 end，再反复递增迭代器。
push_back 即使不扩容也会使旧 end 失效；erase 会使当前位置及其后的迭代器失效。
所以“先 reserve 足够空间，再在范围 for 中追加”仍不能保证该循环安全。

通过 `auto& sample` 修改时间或三轴等现有字段，不改变 vector 的结构，可以正常进行。
需要删除时用上一问的显式迭代器循环；需要把输入追加到另一个容器时，本课 source → samples 的写法可以使用。

删除与插入规则参考：[C++17 草案：vector modifiers](https://timsong-cpp.github.io/cppwp/n4659/vector.modifiers)。

<a id="review-invalidation"></a>

### 16. vector 什么时候扩容？怎么记失效规则？

当插入后的 size 超过原 capacity 时，需要重新分配更大的连续存储，并把原元素构造到新存储中。
旧元素可能被移动或复制，取决于元素类型及异常保证；联系[第 05 课](05_copy_move_semantics.md)复习。
标准不规定容量一定按 2 倍或 1.5 倍增长，不能把某次输出当成统一规则。

判断分两步：先看是否重新分配，再看操作是否使元素移位或改变尾后位置。

| 操作 | 旧元素引用 / 迭代器 | 旧 end |
|---|---|---|
| 发生重新分配 | 全部失效；旧元素指针也失效 | 失效 |
| reserve 未重新分配 | 保持有效 | 保持有效 |
| push_back 未重新分配 | 原元素保持有效 | 失效 |
| insert 未重新分配 | 插入点之前有效；插入点及之后失效 | 失效 |
| erase 实际删除元素 | 删除点之前有效；删除点及之后失效 | 失效 |
| resize 增大且未重新分配 | 原元素保持有效 | 失效 |
| resize 缩小 / pop_back | 保留元素有效；被删除元素失效 | 失效 |
| clear | 原元素全部失效 | 失效 |

resize 到相同 size 不增删元素。表格讨论操作成功后的结果。
erase 后不要用旧指针追踪原采样：后续元素会移位，相同地址上的值可能已经属于另一条采样。
“还能打印出来”“地址没变”都不足以证明原访问位置仍能安全表示原元素。

### 17. 保存索引就永远安全吗？为什么引用离开作用域后元素还在？

只 reserve 时，元素次序不变，索引 0 仍能定位第一条采样；先保存索引，扩容后通过 at 重新取得引用即可。
但中间插入、删除或排序后，同一索引可能对应另一条采样；若需要长期追踪，应设计稳定的标识。
使用索引前仍要保证索引小于当前 size。

本课局部引用、裸指针和迭代器只是访问 vector 元素的工具，不拥有元素。
内部大括号结束只让这些局部变量结束使用，元素仍由 samples 管理。
元素被删除或容器析构时，外部引用不会因此延长它的生命周期。

<a id="review-complexity"></a>

### 18. vector 常见操作的时间复杂度是多少？为什么追加是均摊 O(1)？

以下把单个 ImuSample 的构造、赋值、销毁视为 O(1)，n 为已有元素数。

| 操作 | 时间复杂度 | 原因 |
|---|---|---|
| `size` / `capacity` / `empty` | O(1) | 不需要遍历元素 |
| `[]` / `at` / `front` / `back` | O(1) | 连续存储、按位置定位 |
| 遍历求均值 | O(n) | 每条采样检查一次 |
| `push_back` | 均摊 O(1)，单次最坏 O(n) | 个别追加需要迁移原元素 |
| `pop_back` | O(1) | 只删除末元素 |
| 中间插入 / 删除单个元素 | O(n) | 后面的元素可能需要移位 |
| `clear` | O(n) | 销毁所有元素 |
| 拷贝整个 vector | O(n) | 复制每个元素 |

均摊 O(1) 描述一串追加操作的总成本平均分摊，不是每次追加都固定耗时，也不是概率上的平均。
预知采样数量时一次 reserve 可以减少迁移；不要每次追加前都 reserve(size + 1)，这可能破坏增长策略，
导致频繁分配和总计 O(n²) 的迁移。

本课逐个 erase 的筛选最坏也是 O(n²)，因为可能反复移动后续元素。
本课先学安全删除，第 08 课再用 C++17 的 erase-remove 惯用法处理批量筛选；`std::erase_if` 是 C++20 接口。

### 19. push_back 和 emplace_back 有什么区别？后者一定更快吗？

push_back 接收已有元素对象，按来源复制或移动；emplace_back 转发构造参数，在容器位置构造元素。
已有对象传进去时，emplace_back 也可能复制或移动；两者都可能扩容，不会因为用了 emplace 就避免失效。

本课 ImuSample 是带默认成员初始化器的聚合类型，没有接收“时间、三轴”的构造函数。
C++17 下不能直接照搬 `samples.emplace_back(0.06, axes)`；可以写：

```cpp
samples.push_back(ImuSample{0.06, {0.1, 0.2, 9.8}});
```

本课使用 push_back 表达清楚即可，不为换一个 API 而添加构造函数。

<a id="review-mean"></a>

### 20. meanAcceleration 的 const &、输出引用和 bool 各有什么用？

```cpp
bool meanAcceleration(const std::vector<ImuSample>& samples,
                      std::array<double, 3>& mean);
```

samples 是只读输入，传 const 引用避免复制整段采样；mean 是可写输出引用，函数写入调用者的三轴数组；
bool 表示是否成功计算。空序列返回 false，而不能把“没有均值”误认为“均值恰好为零”。
当前实现先将 mean 清零，再检查 empty，所以失败时输出也是零数组；调用者应优先检查返回值。

三个累计量是 double，因此除以 samples.size() 时执行浮点除法，不是整数截断除法。
不要把这个结论推广给两个整数相除；浮点结果核对用容差，本课固定范围的人工数据可用约 1e-6。
`std::fixed << std::setprecision(3)` 只控制输出显示三位小数，不改变数组中保存的计算值。

异常筛选应写 `<cmath>` 中的 `std::abs(double)`，避免依赖未限定名称 abs 的可见重载。
当前人工数据是有限数，判定用严格的 `>`，所以刚好等于 ±threshold 的值不会被删除；
NaN、无穷大及阈值合法性属于进一步的数据校验，不是这条教学规则已经解决的问题。

<a id="review-check"></a>

## 十二、三分钟复习自测

先遮住右列回答，答错再按导航回到对应问答。

| 问题 | 一句话答案 |
|---|---|
| reserve(8) 后 size 为 0，能写 samples[0] 吗？ | 不能，预留存储不等于创建元素。 |
| resize(4) 与 reserve(4) 最关键的区别？ | 前者将实际元素数改成 4，后者不改变元素数。 |
| array 长度固定意味着元素不能改吗？ | 不意味着；是否只读看 const。 |
| `for (auto x : samples)` 修改 x 会改原采样吗？ | 不会，x 是副本。 |
| find 返回 0 是没找到吗？ | 是在开头找到；未找到返回 npos。 |
| 不扩容的 push_back 是否完全不使迭代器失效？ | 不是，旧 end 仍失效。 |
| erase 后为何不能无条件 ++it？ | 返回位置已有下一条待检查采样，递增会漏检或越过 end。 |
| clear 后 capacity 还在，旧引用能用吗？ | 不能，原元素已经销毁。 |
| 连续多次 erase 一定是 O(n) 吗？ | 不是，反复移位最坏 O(n²)。 |
| 保存索引能抵抗所有容器修改吗？ | 不能，插入、删除、排序可能改变索引对应的元素。 |

<a id="review-run"></a>

## 十三、本次文档同步的运行核对（2026-10-28）

本次使用当前 main.cpp，以 `g++ -std=c++17 -Wall -Wextra -Wpedantic` 编译到临时目录并运行，
编译成功、无编译警告、程序正常退出。以下是实际输出的摘要；容量具体数值仅代表本次实现。

| 检查项 | 本次结果 |
|---|---|
| 名称与三轴 | imu_01，长度 6，非空；三轴 [0.150, -0.200, 9.800]，长度 3 |
| reserve(8) | size=0，capacity=8；存入六条后 size=6，capacity=8 |
| probe 的 resize / pop_back / clear | size 依次为 6 → 2 → 4 → 3 → 0，capacity 一直为 6；新增采样全零 |
| 六条均值 | [-1.583, -0.033, 9.800] |
| 删除相邻异常 | 6 → 4，保留时间 0.00、0.01、0.04、0.05；均值 [0.125, -0.050, 9.800] |
| 空 / 全正常 / 全异常 | 数量分别 0 → 0、4 → 4、2 → 0；均值成功标记分别 false、true、false |
| reserve 后重新访问 | 首元素地址发生变化，时间与三轴数据保持原值 |
| 实验 4 末尾额外追加 | 新末元素时间为 2.000；最终 size=7，capacity=7 |

当前代码有几处复习时应识别的细节：

- meanAcceleration 中“**三轴累加结果**”打印在除法之后，实际显示的是均值。
- 实验 4 的 `initial size/capacity` 打印在额外 push_back 之后，实际是最终状态；
  文中“reserve 后 size 仍为 6”指追加之前，不能与最终的 7 混淆。
- removeOutliers 当前写的是未限定名称的 abs；本次人工数据输出正确，复习时应采用问答 20 的 `std::abs` 写法。
- 本次未执行 Valgrind 或格式验收，不据此勾选全部完成标准。
