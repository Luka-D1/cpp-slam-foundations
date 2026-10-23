# 06 string、array、vector 与迭代器

创建日期：2026-10-23

状态：教学文件已建立，四个代码实验待完成。

程序：`imu_sequence_demo`

学习路径：**看课 → 做笔记 → 敲代码 → 总结**。

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
| meanAcceleration | 接口与空序列处理已给出，累加和求平均由你写 |
| removeOutliers | 接口已给出，迭代器删除由你写 |

四个 demo 函数各自使用局部数据，前一个实验的改动不会影响后一个。
开始时已有代码可以编译；求均值和删除等核心逻辑需要你亲手补齐。

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
对同一个 probe，这里的缩小、清空不要求它缩减 capacity。
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
