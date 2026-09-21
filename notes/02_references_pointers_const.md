# 02 引用、指针、const 与对象生命周期

## 本次目标

完成后应能解释并通过代码验证：

- `T`、`T&`、`const T&` 和 `T*` 的区别；
- 为什么 `Sensor& sensor = imu;` 不会复制对象；
- 函数参数什么时候使用值、引用、const 引用或指针；
- `nullptr` 的含义以及解引用前为什么要检查；
- 局部对象在何时创建、何时销毁；
- 什么是悬空指针或悬空引用，以及为什么不能使用它们。

建议总时间为 90～150 分钟，可以分成两天完成，不要求一次结束。

## 1. 值传递

用自己的话回答：

- 函数收到的是原对象还是副本？
- 在函数中修改参数，调用者的变量是否变化？

代码实验：实现 `incrementByValue(int value)`，打印函数内外的数值和地址。

## 2. 引用传递

用自己的话回答：

- `int& reference = value;` 是否创建了第二个整数？
- 为什么修改引用会影响原变量？
- 引用初始化后能否重新绑定到另一个对象？

代码实验：实现 `incrementByReference(int& value)`，与值传递的结果比较。

## 3. const 引用

用自己的话回答：

- `const ImuSample&` 为什么适合只读传递较大的对象？
- 它与按值传递相比避免了什么？
- 函数内部能否通过它修改对象？

代码实验：实现 `printSample(const ImuSample& sample)`。

## 4. 指针与 nullptr

用自己的话回答：

- `ImuSample*` 保存的是对象本身还是对象地址？
- `&sample` 和 `*pointer` 分别表示什么？
- 引用和指针在“可以为空”方面有什么区别？

代码实验：实现 `resetSample(ImuSample* sample)`。先检查 `sample == nullptr`，再修改三个轴的数据。

## 5. const 与指针

区分并各写一句解释：

- `const ImuSample* pointer`：
- `ImuSample* const pointer`：
- `const ImuSample* const pointer`：

## 6. 对象生命周期

记录并解释：

