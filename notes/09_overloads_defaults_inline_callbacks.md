# 09 函数重载、默认参数、内联与回调

第九课涉及到的工程思想：把数据处理规则和数据处理过程分离

创建于 2026-11-09

程序为 `processing_pipeline_demo`。代码骨架已建立，四个实验待完成。

学习路径是 **看课或听讲解 → 记下面试短答案 → 填一个 TODO 并运行 → 用结果总结**。

## 一、从一条观测残差开始

第 08 课用 lambda 告诉 `find_if` 怎么匹配、告诉 `sort` 怎么比较。第 09 课换成自己写处理函数。让调用者选择规则，同一个处理入口就可以执行不同操作。

先用一条模拟观测练习。时间戳为 400 ms，原始残差为 2.50 m，估计偏差为 0.50 m，校正值是 2.00 m。后三个实验用有符号残差 -2.50 m，方便看出门限判断、保留符号和取绝对值的区别。最终将取绝对值和截断幅值接成两步。

建议分两次，每次约 60～90 分钟。第一次做 TODO 1、2，第二次做 TODO 3、4。路线图对应翁恺第 17、18 节，可以按需看。函数指针和 `std::function` 可以先结合实验学。

| 实验 | 先读问答 | 一眼能核对的结果 |
|---|---|---|
| TODO 1 两种重载输入 | 1～4 | 传原始数值和传观测对象都得到 2.00 m |
| TODO 2 默认门限与 `inline` | 5～8 | -2.50 m 在默认 2.0 m 门限下为 true，显式改成 3.0 m 时为 false |
| TODO 3 函数指针 | 9～12 | 同一个入口得到 -2.50 m 或 2.50 m |
| TODO 4 两步回调管线 | 13～16 | -2.50 m 先变成 2.50 m，再按配置截到 1.50 m 或 0.75 m |

## 二、四个 TODO

代码位于 `exercises/09_processing_pipeline/main.cpp`。示例数据、处理入口和两个普通函数已给出。每个实验可以独立编译运行。

### TODO 1 用同一个名字接收两种输入

在 `demoOverloads` 上方定义两个名为 `correctResidual` 的函数。一个接收 `double residual_m, double bias_m`，返回原始残差减偏差。另一个接收 `const Measurement &sample, double bias_m`，用对象的 `residual_m` 调用第一个函数。回到 `demoOverloads`，分别传原始数值和对象，打印校正结果。

核对两次结果都是 2.00 m，原对象的残差仍是 2.50 m。能指出编译器怎样根据第一个实参选出重载，即可进入下一步。

### TODO 2 给门限一个常用默认值

在 `demoDefaultArgumentAndInline` 上方定义 `inline bool exceedsLimit(double residual_m, double limit_m = 2.0)`。判断残差幅值是否**严格大于**门限，可用 `std::abs`。分别调用以下三种写法并打印布尔结果。

| 调用 | 预期 |
|---|---|
| `exceedsLimit(-2.50)` | true，使用默认门限 2.0 m |
| `exceedsLimit(-2.50, 3.0)` | false，使用显式门限 |
| `exceedsLimit(-2.0)` | false，幅值等于默认门限 |

说出哪次调用省略了实参、哪次显式传入。这里通过运行验证的是默认实参与门限逻辑，`inline` 的含义见问答 8。

### TODO 3 让调用者传入一个普通函数

在 `demoFunctionPointer` 上方定义 `printWithFunctionPointer`，接收 `double raw_m` 和 `ResidualFn step`。骨架已用 `using ResidualFn = double (*)(double);` 给指针类型起名，读作“指向接收一个 double、返回 double 的函数的指针”。

先判断 `step` 是否为空，有效时调用 `step(raw_m)` 并打印返回值。入口中用相同的 -2.50 m 分别传入 `keepResidual`、`absoluteResidual` 和 `nullptr`。核对结果分别为 -2.50 m、2.50 m 和空指针提示。想一想这和第 08 课把谓词交给算法哪里相似。

### TODO 4 用同一个管线处理两种门限配置

1. 在 `demoPipeline` 上方定义 `printPipelineResult`，输入为 `double raw_m` 和 `const std::vector<ResidualStep> &steps`。从原始值开始，逐个调用有效步骤，把本步结果送给下一步，并打印每一步结果。遇到空步骤时打印提示并跳过。
2. 在 `demoPipeline` 组装两步。第一步是普通函数 `absoluteResidual`。第二步是按值捕获门限的 lambda，接收当前幅值，返回幅值和门限中较小的一个，可用 `std::min`。
3. 分别用 1.50 m、0.75 m 的门限组装步骤并调用同一个管线函数。核对中间值都是 2.50 m，最终值分别是 1.50 m 和 0.75 m。

这里的“管线”就是按顺序执行一组函数。先确认每步都接收上一步的输出，再考虑如何少写重复代码。

## 三、面试问答

做一个 TODO 时先读相关几题。面试可以先给 20～40 秒的短答，再举本课代码的例子。

### 1. 什么是函数重载？

同一作用域内可以用同一个名字定义多个函数，并通过参数的类型、数量或排列区分它们。本课的两个 `correctResidual` 都做残差校正，第一个参数分别是 `double` 和 `const Measurement &`。语义一致的接口用同名重载更容易理解。

### 2. 重载与第 01 课的重写有什么区别？

重载解析在编译时根据调用处的实参选择函数。重写是派生类给基类的虚函数提供新的实现，通过基类引用或指针进行普通虚调用时，会按对象的实际类型选择实现。`override` 让编译器检查派生类函数确实重写了基类虚函数。

### 3. 编译器怎样选择重载？

先找候选函数，再判断哪些能接受给定实参，最后从可行候选中选唯一最佳匹配。常见情况中，精确匹配优于类型提升，类型提升优于普通标准转换。例如 `pick(int)` 与 `pick(double)` 同时存在时，`pick(1)` 选择前者，`pick(1.0)` 选择后者。存在多个候选且无法确定唯一最佳项时，调用会报二义性。 [C++17 草案的最佳可行函数](https://timsong-cpp.github.io/cppwp/n4659/over.match.best)

### 4. 能否只用返回类型来区分两个重载？

普通函数仅返回类型不同，参数列表相同，不能构成重载。`int read()` 和 `double read()` 不能作为两个同名同参接口共存，写 `double x = read()` 也无法用接收变量来区分它们。

### 5. 默认参数什么时候起作用？一般写在哪里？

调用省略了末尾实参时，编译器按调用处可见的声明补上默认实参。这里的 `exceedsLimit(-2.50)` 等同于显式传入门限 2.0。声明与定义分开时，通常把默认值写在调用者能看到的声明里，定义处保留参数但省略默认值。同一作用域不能重复声明同一个参数的默认值，即使值相同。 [C++17 草案的默认实参](https://timsong-cpp.github.io/cppwp/n4659/dcl.fct.default)

### 6. 默认参数和重载一起使用会怎样产生二义性？

假设已有 `report(double)`，又声明 `report(double, double limit = 2.0)`。`report(2.50)` 可以调用两者，并且第一个实参都是精确匹配。编译器不能因为其中一个候选省略了参数，就自动选择另一个。可以为接口选清楚的参数组合，或用不同函数名表达不同用途。

### 7. 有默认值的双参数函数可以赋给单参数函数指针吗？

函数类型仍然含有两个参数。比如 `exceedsLimit` 带默认值，类型仍为 `bool(double, double)`。把它存入 `bool (*)(double, double)` 后，经这个指针调用时需要传两个实参，因为默认实参并不属于函数类型。若需要一个一元处理步骤，可以用一个只接收一项的 lambda 在内部调用原函数。

### 8. `inline` 会让函数一定在调用处展开吗？

优化器决定是否展开函数调用，函数可以在没有 `inline` 说明符的情况下被优化展开。对工程代码而言，`inline` 还允许有外部链接的函数在多个翻译单元中按单一定义规则提供相同定义，这就是常把小型 `inline` 函数定义写在头文件里的原因。翻译单元可以先理解为一个源文件展开头文件后交给编译器的内容，本课用单文件学习语法。 [C++17 草案的 inline 说明符](https://timsong-cpp.github.io/cppwp/n4659/dcl.inline)

### 9. 什么是回调？它一定异步吗？

把可调用对象交给另一段代码，由那段代码决定何时调用，就是回调。本课的 `printWithFunctionPointer` 在当前调用中执行传入的规则，属于同步回调。第 08 课交给 `find_if` 的谓词也是相同思路。

### 10. 怎样读懂 `double (*)(double)`？

右侧的 `(double)` 是函数的参数列表，左侧的 `double` 是返回类型，中间的 `(*)` 表示指向这种函数的指针。可用 `using` 起一个名字。在需要匹配函数指针的上下文中，普通函数名 `absoluteResidual` 可以转换为函数指针，写成 `&absoluteResidual` 也可以。

### 11. 为什么函数指针要先检查空值？

函数指针可以是 `nullptr`，此时没有可调用的函数。执行 `step(raw_m)` 前先用 `if (step)` 或与 `nullptr` 比较，避免调用空指针。实验 3 的第三次调用专门核对这个分支。

### 12. lambda 都能当作普通函数指针吗？

写成 `[]` 的无捕获 lambda 在签名匹配时可以转换成函数指针。写成 `[limit_m]` 的 lambda 含有自己的捕获状态，需要把那个闭包对象一起保存，可交给 `std::function`。 [C++17 草案的 lambda 闭包类型](https://timsong-cpp.github.io/cppwp/n4659/expr.prim.lambda.closure)

### 13. `std::function<double(double)>` 表达什么？

它是一个可调用包装器。调用方只需知道可以传一个 `double` 并取得 `double` 结果，包装器内部可以持有兼容且可拷贝的可调用对象，例如普通函数指针、函数对象或本课按值捕获的 lambda。把两种规则都包装成 `ResidualStep` 后，可以把它们放在同一个 `vector`。 [C++17 草案的 std::function](https://timsong-cpp.github.io/cppwp/n4659/func.wrap.func)

### 14. 为什么门限要按值捕获？按引用可以吗？

按值捕获会在创建步骤时保存门限的副本，因此分别用 1.50 和 0.75 创建的步骤各自拥有自己的配置。按引用捕获会读取外部原变量的当前值，调用时要确保它仍然存活。需要保存回调供以后执行时，要特别检查引用捕获的生命周期。

### 15. 空的 `std::function` 怎么处理？和空函数指针有什么区别？

默认构造的 `std::function` 可能没有目标，`if (step)` 可以检查。直接调用空包装器会抛出 `std::bad_function_call`，调用空函数指针则是未定义行为。本课统一在调用前检查，函数指针实验打印提示，管线遇到空步骤时跳过。 [C++17 草案的调用规则](https://timsong-cpp.github.io/cppwp/n4659/func.wrap.func.inv)

### 16. 函数指针和 `std::function` 怎么选？管线的步骤顺序为什么重要？

只需接收签名固定的普通函数时，函数指针就能表达需求。要用统一接口存储不同类型的可调用对象，尤其是带配置的 lambda，可以用 `std::function`。它为这种灵活性引入包装开销，具体成本取决于调用对象与实现。管线中每一步都拿到上一步的结果。本课先把有符号残差转为幅值，再按幅值上限截断，顺序体现了处理规则的输入要求。

## 四、运行与收尾

以下命令都在 **WSL Ubuntu 的 `/home/luka/projects/cpp-slam-foundations`** 执行。

| 命令 | 作用 |
|---|---|
| `cd /home/luka/projects/cpp-slam-foundations` | 进入主开发仓库。 |
| `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug` | 首次加入第 09 课后生成 Debug 构建规则。 |
| `cmake --build build --target processing_pipeline_demo` | 只编译第 09 课。每个 TODO 完成后都可运行。 |
| `./build/exercises/09_processing_pipeline/processing_pipeline_demo` | 执行当前代码并核对实际输出。 |
| `clang-format --dry-run --Werror exercises/09_processing_pipeline/main.cpp` | 检查本课代码格式。 |
| `git diff --check` | 检查已跟踪改动中的空白错误。 |
| `git diff` | 查看已跟踪文件的改动。 |
| `git status --short` | 查看文件状态。新增文件可以单独阅读，暂存后再检查差异。 |

| 实验 | 我的实际输出 | 我能独立回答的面试问题 |
|---|---|---|
| 1 重载 |  |  |
| 2 默认实参与 `inline` |  |  |
| 3 函数指针 |  |  |
| 4 回调管线 |  |  |

四个实验跑通后，用自己的输出解释重载与重写的区别、默认实参如何补齐，以及两种回调怎样传入规则。记录本次改动，再按实际成果提交。
