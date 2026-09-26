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
对于一个函数 类似于：

```cpp
void incrementByValue(int value) {
    value++;
}

int number = 10;
incrementByValue(number);
```

此处的value是由number复制出来的副本 


- 在函数中修改参数，调用者的变量是否变化？

函数中修改参数 调用者的变量不会改变调用者变量 原因或许是修改的参数只是一个形式参数 不会影响全局变量

代码实验：实现 `incrementByValue(int value)`，打印函数内外的数值和地址。

## 2. 引用传递

用自己的话回答：

- `int& reference = value;` 是否创建了第二个整数？

此处并未创造第二个整数 只是给第一个整数起了一个别名

- 为什么修改引用会影响原变量？

可能因为引用指向的就是原变量的原对象，不同于指向副本

- 引用初始化后能否重新绑定到另一个对象？

一般来讲 引用不能指向两个不同的对象 但是如果经历初始化 或许可以重新绑定

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





## 9.21晚学习内容：
## 9.21 晚学习内容：

int* p;        // p 是指向 int 的指针，存储 int 对象的地址
               // * 在声明中表示该变量是指针类型

int& r = a;    // r 是 a 的引用 / 别名
               // & 在声明中表示 reference（引用）

&a;            // 取得 a 的地址
               // & 在表达式中表示取地址

*p;            // 访问 p 所指向的对象
               // * 在表达式中表示解引用（dereference）

int* f();      // f 函数返回一个指向 int 的指针

int& g();      // g 函数返回一个 int 的引用
               // 即返回某个 int 对象的别名、



## 9.26午后学习内容：

引用不可以为null值 必须依赖在一个已有的变量上 指向不可能是别人 permanent marriage
指针可以指向空值 与已有的对象是独立的 可以改变指向不同的地址
reference 存在限制：
没有指向引用的引用 No references to references
没有指向引用的指针 No pointers yo references such as: int&* p     // which isillegal
    but 指向指针的引用是可以的：void f(int*& p)


