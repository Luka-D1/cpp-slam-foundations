# 07 map、set 与 unordered_map：传感器注册表与面试问答

创建日期：2026-10-28

状态：教学文件已建立，四个实验待完成。

程序：`sensor_registry_demo`

学习方式：**看课/听讲解 → 记面试答案 → 敲代码验证 → 用自己的话总结**。

## 一、情景与学习顺序

第六课处理“按位置保存的一段采样”。本课处理“知道传感器名称，直接找到配置”。

统一使用下面的配置，输入顺序故意为 ODO、IMU、GNSS：

| 名称（key） | 编号 id | 频率 frequency_hz | 启用 enabled |
|---|---:|---:|---|
| ODO | 3 | 20.0 | false |
| IMU | 1 | 200.0 | true |
| GNSS | 2 | 10.0 | true |

代码已提供 SensorInfo、SensorRecord、示例数据和打印工具，四个实验各用自己的局部数据。
你负责填容器操作。本课属于基础训练，不代表已经完成 SLAM 工程贡献。

本笔记以面试问答为主体。每做一个实验，先读对应问题，不要求一次背完。
先能用 20～40 秒回答，再用代码和输出解释自己的答案。

| 实验 | 内容 | 先读的面试问题 |
|---|---|---|
| TODO 1 | map 注册、查找、重复键、更新、删除 | 1～11 |
| TODO 2 | set 名称去重、排序与边界查找 | 3、6、12～15 |
| TODO 3 | unordered_map 查找、桶与重哈希 | 4、5、16～21 |
| TODO 4 | 查询副作用、更新接口与安全删除 | 8～11、22～27 |

建议分两到三次学习，每次约 60～100 分钟；一个实验卡住就先完成该实验。
翁恺第 41 节“STL 简述”可选复习；这里的容器细节由讲解、问答和实验共同补齐，
不要求在编码前看完一套新课程。

## 二、四个 TODO 的基本要求

代码位置：`exercises/07_sensor_registry/main.cpp`。保持已有的 demo 函数风格。

### TODO 1：map——按名称注册与查找

在 demoOrderedRegistry 中：

1. 用 insert 注册三条 source 数据，打印遍历结果。
2. 重复插入 IMU → SensorInfo{99, 999.0, false}，记录返回值；原配置应保持不变。
3. find IMU，判断是否找到，再把频率改为 250.0。
4. find LIDAR，确认未找到且 size 不变。
5. erase ODO 两次，观察返回的删除数量。

核对：初次遍历 GNSS、IMU、ODO；重复插入未覆盖；最后剩 GNSS 和 IMU，
IMU 的 id 仍为 1，频率 250.0，启用状态 true；两次 erase 返回 1、0。

总结：有序是按什么排序？怎样查询才不会意外插入？

### TODO 2：set——去重并做有序查找

在 demoUniqueNames 中：

1. 把 IMU、GNSS、IMU、ODO、GNSS 依次插入 set，打印每次插入结果。
2. find/count 查询 IMU 与 LIDAR。
3. 查看 lower_bound("LIDAR")、upper_bound("IMU")、lower_bound("ZZZ")。
4. 每次访问查找结果前先与 end 比较；最后删除 GNSS。

核对：去重后 3 个名称，顺序 GNSS、IMU、ODO；IMU 的 count 为 1，LIDAR 为 0。
前两个边界查询都找到 ODO，最后一个返回 end；删除 GNSS 后剩 IMU、ODO。

总结：set 为什么不保存额外的 SensorInfo？为什么不能直接改元素？

### TODO 3：unordered_map——查找与桶

在 demoHashedRegistry 中：

1. 填入同样三条配置，查询 IMU 和不存在的 LIDAR。
2. 打印 size、bucket_count、load_factor、max_load_factor。
3. reserve(32)，再次打印；size 应仍为 3，桶数不要求恰好是 32。
4. 保存当前桶数，再 rehash(old_bucket_count + 1)，确保桶数增长。
5. 重哈希前让旧迭代器离开局部作用域，之后重新 find IMU。

进阶验证：在重哈希前保存指向 IMU 的 SensorInfo 的指针，
重哈希后读取它，解释为何此处有效，而第六课 vector 扩容后的旧元素指针失效。
整个过程中不删除该元素；获取指针前必须确认 find 成功。

核对：数据内容和数量不变；遍历顺序与实际桶数只记录观察，不当作固定答案。

总结：平均常数查找为什么不等于每次都常数？reserve 与 rehash 的参数分别表示什么？

### TODO 4：接口选择——避免“查询”偷偷修改注册表

demoAccessAndChoice 初始只有 IMU、GNSS 两项：

1. 对比 find("LIDAR") 与 registry["LIDAR"] 对 size 的影响。
2. 打印误插入的默认配置，然后删除 LIDAR。
3. insert_or_assign 把 IMU 更新为 SensorInfo{1, 400.0, true}。
4. try_emplace 同键 IMU → SensorInfo{99, 999.0, false}，检查未覆盖。
5. try_emplace 新键 ODO → SensorInfo{3, 20.0, false}。
6. 用迭代器循环删除 enabled=false 的项，使用 erase 返回的位置继续。

核对：find 后仍为 2 项；operator[] 后为 3 项，LIDAR 的字段为 0、0.0、false。
清理后最终剩 GNSS、IMU；IMU 频率 400.0，ODO 已删除。
insert_or_assign 更新已有键时返回的 second 是 false，但更新是成功的。

总结三种场景：需要按名称有序输出、只需要高频按键查找、只需要名称去重，各选谁？

## 三、面试高频问答：先掌握短答案

以下是可用于面试的参考回答。请在实际学习后补充自己的例子，不把它们当作已完成实验记录。

### 1. map、set、unordered_map 分别解决什么问题？

**参考回答：** map 按键关联值并保持键有序；set 只保存不重复的键并保持有序；
unordered_map 关联键和值，使用哈希组织元素，不保证键的遍历顺序。

| 容器 | 一个元素表示什么 | 是否唯一 | 顺序 |
|---|---|---|---|
| map<string, SensorInfo> | 名称 → 配置 | 键唯一，值可以重复 | 按键比较规则 |
| set<string> | 一个名称 | 元素就是键，键唯一 | 按键比较规则 |
| unordered_map<string, SensorInfo> | 名称 → 配置 | 键唯一，值可以重复 | 未规定 |

### 2. 为什么有 vector，还需要 map？

**参考回答：** vector 适合按位置访问、连续遍历和保存有顺序的采样序列；
按名称在线性 vector 中逐项查找通常是 O(N)。map/unordered_map 直接表达按键查找。
不过数据少、遍历多时，vector 的连续内存可能更有利，不能只靠大 O 断言谁更快。

联系本项目：一段 IMU 测量保存在 vector；传感器名称到配置的注册表适合关联容器。

### 3. map/set 的“有序”是插入顺序吗？unordered_map 是随机顺序吗？

**参考回答：** map/set 的有序由比较器决定，默认按键升序。
本例字符串是区分大小写的字典序，输入顺序不改变排序结果。
unordered_map 的遍历顺序未规定，不能依赖，也不应称为“每次都会随机打乱”。

默认比较是 std::less<Key>；降序可选择 std::greater<Key>，排序依据不是 mapped value。
“有序”描述逻辑遍历顺序，不表示元素存放在连续内存里。

### 4. 三种容器的时间复杂度怎么回答？

N 表示元素数。先把操作说清楚：

| 操作 | map/set | unordered_map |
|---|---|---|
| find、count | O(log N) | 平均 O(1)，最坏 O(N) |
| 普通无提示的单元素 insert | O(log N) | 平均 O(1)，最坏 O(N) |
| erase(key)，本课唯一键容器 | O(log N) | 平均 O(1)，最坏 O(N) |
| 遍历全部元素 | O(N) | O(N) |

这是按比较/哈希等基本操作计的复杂度；字符串比较、计算长字符串哈希本身也有成本。
不要把全部接口都套成同一个复杂度。map/set 的 erase(iterator) 为均摊 O(1)，
但先 find 再 erase 的完整过程仍含 O(log N) 查找成本。

### 5. map/set 底层一定是红黑树吗？

**参考回答：** 常见标准库实现采用红黑树一类的平衡搜索树，因此能够保持有序并提供对数查找。
C++ 标准约束接口、顺序、复杂度和有效性，不强制必须使用红黑树。
unordered_map 提供桶和哈希语义，也不能把某一种桶内部布局背成唯一实现。

追问时能说明：普通不平衡二叉搜索树可能退化为长链；平衡结构用于控制高度。
本课不要求手写红黑树或旋转代码。

### 6. “键不能重复”依据 operator== 吗？

**参考回答：** map/set 用比较器定义键等价：comp(a,b) 和 comp(b,a) 都为 false，
就认为是等价键，不必等同于 operator==。比较器必须满足严格弱序，不能随意把 < 改成 <=。

unordered_map 由 KeyEqual 判断键等价；默认通常使用 std::equal_to<Key>。
本课 string 的默认比较与相等行为符合日常字符串理解，但原理要区分。

### 7. map 的元素是什么类型？first、second 分别是什么？

**参考回答：** map<Key,T> 的 value_type 是 std::pair<const Key,T>，
first 是不可直接修改的键，second 是配置值。unordered_map 同样如此。
迭代器可通过 it->second 修改配置，不能通过 it->first 修改键。

本课打印辅助函数用 entry.first 和 entry.second，分别输出名称和 SensorInfo。

### 8. find、count、at 和 operator[] 怎么选？

| 接口 | 缺键时 | 是否插入 | 本课用途 |
|---|---|---|---|
| find(key) | 返回 end | 否 | 查找并处理配置 |
| count(key) | 返回 0 | 否 | 判断存在；唯一键容器只能为 0 或 1 |
| at(key) | 抛出 out_of_range | 否 | 确知键存在时读取/修改 |
| operator[](key) | 创建值初始化的 mapped value | 是 | 明确需要“没有就创建”的场景 |

**参考回答：** 查询失败属于正常情况时用 find，读取前判断 end；
不能用 operator[] 伪装无副作用查询。这里不要求额外实现异常处理实验，
使用 at 时先确认键存在。

### 9. 为什么 map/unordered_map 的 operator[] 会意外插入？

**参考回答：** operator[] 要返回一个可赋值的 mapped value 引用。
键不存在时，会先建立对应项，再返回其值的引用。
本课 SensorInfo 有明确默认值，所以误插入会得到 id=0、frequency_hz=0、enabled=false。

不要把“返回配置全是零”当成“查询失败”；此时注册表已经改变。
对不支持相应默认构造的 mapped type，缺键插入形式还有类型要求，不能假设任何类型都可随便 []。

### 10. 为什么 const map 不能用 operator[]，却能用 find/at？

**参考回答：** operator[] 可能插入，所以没有对应的 const 查询版本。
find 有 const 重载，at 也有 const 重载；只读注册表可以使用它们。
本仓库是 C++17，判断存在用 find/count；contains 是 C++20 接口，不作为本课代码要求。

### 11. insert 遇到重复键会覆盖吗？返回值怎么看？

**参考回答：** 本课唯一键容器的普通 insert 不覆盖已有元素，
返回 pair<iterator,bool>：first 指向新插入或已存在的元素，second 表示是否真的插入。
重复键时 second=false，原配置保留。set 的 insert 也可用同样方式观察去重。

注意返回的 pair.first 是迭代器；map 元素内部的 pair.first 才是键。
不要把这两层 first 混为一谈。有提示位置等其他重载返回值不同，本课先用普通重载。

### 12. set 为什么没有 second？能用下标读取第几个元素吗？

**参考回答：** set 只保存键，没有另一个 mapped value，因此没有配置值 second，
也没有 vector 那样的下标访问接口。按位置编号不是 set 的主要操作方式。
可用迭代器遍历；本课不使用 it + 1 这样的随机访问语法。

map/set 迭代器支持双向移动；unordered_map 迭代器至少支持向前遍历，
不要把所有迭代器都当成数组指针。

### 13. set 的元素、map 的键为什么不能直接修改？

**参考回答：** 有序容器依赖键决定结构位置，无序容器依赖键的哈希与等价关系。
随意原地改键会破坏查找规则，因此键不能经普通迭代器直接修改。
本课需要改名时，先保存必要的数据，删除旧键再插入新键，处理好新键已存在的情况。

C++17 还提供节点提取相关接口，本课只需知道存在，不要求再实现一个实验。

### 14. lower_bound 和 upper_bound 有什么区别？

**参考回答：** 在默认升序的 map/set 中，lower_bound(k) 返回第一个“不小于 k”的位置，
upper_bound(k) 返回第一个“大于 k”的位置。找不到符合条件的元素就返回 end。
用自定义比较器时，应按比较规则解释“之前/之后”，不能机械套数字 >=、>。

成员 lower_bound/upper_bound 是 O(log N) 的有序查找。
unordered_map 不提供这种按全局键顺序查边界的接口。

### 15. 去重时要保留重复项怎么办？

**参考回答：** 如果业务需要等价键重复，可考虑 multiset/multimap；
对应哈希版本有 unordered_multiset/unordered_multimap。
它们会改变 count、查找范围和删除数量等语义，不能继续假设 count 只有 0/1。

本课使用 set，是因为“同一名称只需登记一次”；不额外编写 multi 容器实验。

### 16. unordered_map 为什么查得快？哈希值相同就不能同时保存吗？

**参考回答：** 哈希把查找范围定位到桶，再按键等价判断找到目标，
分布合适时平均查找成本较低。不同键可能哈希相同，这叫冲突，不表示键相同，
仍然可以同时保存。冲突严重时查找可能退化到线性。

必须满足“等价键得到相同哈希值”；反过来“相同哈希值推出等价键”不成立。
默认 string 哈希已可使用，本课不要求自己实现哈希器。

### 17. bucket_count、load_factor、max_load_factor 各是什么？

**参考回答：** bucket_count 是桶数量，size 是元素数，
load_factor 表示平均每桶元素数；max_load_factor 是用来指导桶规划的最大负载参数。
load_factor 可理解为元素数除以桶数的平均占用程度。
它们用来描述哈希表组织状态，不是 vector 的 size/capacity。

桶可以为空，也可能有多个元素；负载因子小也不能保证完全没有冲突。
具体桶数和桶内布局依实现而定，不背本机输出数值。

### 18. unordered_map 的 reserve(n) 与 rehash(n) 有何区别？

**参考回答：** reserve 的 n 表示按容纳 n 个元素所需的资源规划桶，
可理解为先把 n / max_load_factor 向上取整，再请求重哈希；
rehash 的 n 表示请求至少相应数量的桶，同时满足当前元素的负载要求。
二者都不创建新的键值元素，也不承诺返回精确桶数。

它们可能重排已有元素，具有开销；reserve 不是“无成本优化”。
设置负载策略时应在预留前明确需求，本课先使用默认策略观察。

### 19. map/set 插入和删除会让迭代器失效吗？

**参考回答：** 普通插入不会使已有元素的迭代器、引用、指针失效；
erase 只使被删元素的访问失效，其他元素仍有效。
这与 vector 删除后方元素也受影响的规则不同。

该结论针对这里的插入/erase，不能推广到 clear、容器析构或赋值等所有操作。

### 20. unordered_map 重哈希时，指针和迭代器一起失效吗？

**参考回答：** rehash 使迭代器失效，但不使仍存在元素的指针、引用失效。
插入若触发重哈希也要考虑这条规则；erase 则会使被删元素的访问失效。
本课 reserve/rehash 之后统一重新 find，使用新迭代器继续操作。

实验三可以保留 SensorInfo 的指针验证差别，但不能继续使用重哈希前的旧迭代器。
unordered_map 的“重排桶”与 vector 的“搬迁连续元素空间”不是同一种失效规则。

### 21. unordered_map::reserve/rehash 的复杂度也是 O(1) 吗？

**参考回答：** 不是。对已有 N 个元素进行 rehash，标准给出平均线性、最坏二次复杂度；
reserve 通过重哈希规划桶，因此也不能当作常数成本。
预先估计规模可以减少反复重哈希，但是否更快仍需结合数据和测量。

这里不做性能结论或微基准测试，先观察桶数变化与迭代器规则。

## 四、面试追问：结合实验四再理解

### 22. insert、emplace、try_emplace、insert_or_assign 怎么区分？

| 接口 | 键已存在时 | 关键区别 |
|---|---|---|
| insert | 不覆盖 | 插入一个键值元素 |
| emplace | 不覆盖 | 用参数构造元素，不能保证失败时完全没构造 |
| try_emplace（C++17） | 不覆盖 | 分开处理键和 mapped 构造，键存在时不构造新 mapped 对象 |
| insert_or_assign（C++17） | 更新值 | 没有则插入，有则赋值 |

**参考回答：** 我会先按“重复键是否覆盖”选择接口，再考虑构造开销。
try_emplace 不代表调用它之前的参数表达式不求值：
把一个已经完整构造好的 SensorInfo 传入，那个对象当然已经构造过。
本课不展开完美转发和模板重载细节。

### 23. insert_or_assign 的 second=false 是更新失败吗？

**参考回答：** 不是。普通返回 pair 的重载里，second 表示“是否插入新元素”，
false 可以表示已成功给已有键赋值。用 first 找到结果，并查看 mapped value 确认更新。
不要把所有 API 的 bool 返回都统一当成“业务成功/失败”。

本课 IMU 已存在，更新频率为 400.0 时应得到 false，并能读到新频率。

### 24. 遍历 map 或 unordered_map 时怎样安全删除？

**参考回答：** 删除当前项时用 it = registry.erase(it)，否则才 ++it。
循环每次与当前 end 比较，erase 若删除末尾会返回 end，不能再解引用。
这种按迭代器删除不会引发这里的哈希重排，未删元素保留。

不要在范围 for 中直接删除它当前遍历的元素；
也不要套用“erase 之后无条件 ++it”而跳过元素。

### 25. 对本项目，什么时候选 map，什么时候选 unordered_map，什么时候选 set？

**参考回答：** 需要按名称稳定有序输出、范围/边界查找时选 map；
主要按名称精确查找且能接受无序和哈希最坏情况时考虑 unordered_map；
只维护唯一名称集合、需要有序去重时选 set。

如果只去重而不需要顺序，还可以考虑 unordered_set。
少量传感器下性能差距未必重要，表达业务需求和避免误用更重要。

### 26. 红黑树与哈希表，哪个更省内存、更快？

**参考回答：** 不能只按容器名字下结论。树有节点和链接等开销，
哈希表有桶及元素管理开销，负载策略、元素大小、分配器和标准库实现都会影响结果。
查找、遍历、排序需求以及实际规模也会影响速度，应按场景选择并测量。

可以说明常见代价，但不要虚构“快几倍”或固定每节点字节数。

### 27. 面试官让你解释一个“查询却修改了数据”的 bug，会怎么说？

**参考回答：** 我会先查是不是对缺失键使用了 map/unordered_map 的 operator[]。
它会创建默认 mapped value，可能把不存在的传感器变成默认编号/频率为零的配置。
改用 find 判断是否存在，只有明确注册或更新时才用插入/赋值接口，并加入缺键测试。

对应实验四：LIDAR 原本不存在，find 不改 size，operator[] 却把 size 从 2 变成 3。

## 五、如何使用这些八股与保存自己的记录

建议先熟练：1、3、4、7～11、13、16、18～20、24、25。
比较器等价、复杂度细节与构造接口追问在做完对应实验后补齐。
每个答案都应能关联到一处代码，而不只是记住容器名字。

| 实验 | 我的真实输出/检查结果 | 面试口述中还不清楚的点 |
|---|---|---|
| 1 map | 待填写 | 待填写 |
| 2 set | 待填写 | 待填写 |
| 3 unordered_map | 待填写 | 待填写 |
| 4 接口选择 | 待填写 | 待填写 |

文中核对结果是预期，不替代实际运行记录；保留自己的原始笔记，纠错时另加说明。

## 六、编译、运行与收尾

以下全部命令都在 WSL Ubuntu 的 `/home/luka/projects/cpp-slam-foundations` 中执行。

```bash
cd /home/luka/projects/cpp-slam-foundations
```

作用：进入主开发仓库。

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

作用：首次加入第 07 课后重新配置。

```bash
cmake --build build --target sensor_registry_demo
```

作用：编译本课。

```bash
./build/exercises/07_sensor_registry/sensor_registry_demo
```

作用：运行当前已实现的实验并记录结果。

```bash
clang-format --dry-run --Werror exercises/07_sensor_registry/main.cpp
```

作用：只检查格式。

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --errors-for-leak-kinds=all --track-origins=yes --error-exitcode=1 \
  ./build/exercises/07_sensor_registry/sensor_registry_demo
```

作用：检查完成后的程序内存错误和泄漏；逻辑正确还需核对重复键、缺键和删除结果。

```bash
git diff --check
git diff
git status --short
```

作用：检查差异格式、已跟踪修改与全部文件状态。
新文件需单独阅读；暂存后检查 git diff --cached，再按实际成果提交。

完成标准：四个实验正确运行；能口述三种容器的选择、复杂度和失效规则；
不依赖 unordered_map 顺序，不误用 [] 查询；笔记有真实结果，收尾检查完成。

## 七、技术核对资料

下列材料用于核对答案，不要求先全部阅读。本项目按 C++17 编译，
工作草案中的较新接口不自动纳入本课。

- [Microsoft：map](https://learn.microsoft.com/en-us/cpp/standard-library/map-class?view=msvc-170)
- [Microsoft：set](https://learn.microsoft.com/en-us/cpp/standard-library/set-class?view=msvc-170)
- [Microsoft：unordered_map](https://learn.microsoft.com/en-us/cpp/standard-library/unordered-map-class?view=msvc-170)
- [C++ 工作草案：关联容器](https://eel.is/c++draft/associative.reqmts)
- [C++ 工作草案：无序关联容器](https://eel.is/c++draft/unord.req)
- [C++ 工作草案：map 元素访问](https://eel.is/c++draft/map.access)
- [C++ 工作草案：map 插入与更新](https://eel.is/c++draft/map.modifiers)
