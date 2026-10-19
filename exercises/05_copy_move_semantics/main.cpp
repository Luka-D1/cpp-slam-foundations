#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// 第 05 课：拷贝、移动语义与 Rule of Zero/Five。
// 配套笔记：notes/05_copy_move_semantics.md。
// 起点使用标准库成员管理资源，特殊成员函数暂由编译器处理。
// 完成 TODO 2/3 后才会出现 copy/move/destroy 日志；骨架可运行不等于完成课程。
class MeasurementPacket {
public:
  MeasurementPacket(const std::string &sensor_name, std::size_t value_count,
                    double initial_value)
      : sensor_name_(sensor_name), values_(value_count, initial_value) {
    print("construct");
  }

  std::size_t size() const { return values_.size(); }
  bool empty() const { return values_.empty(); }

  // 调用前确认非空；at() 在越界时抛异常，不允许靠越界来观察移动状态。
  double first() const { return values_.at(0); }
  void setFirst(double value) { values_.at(0) = value; }

  void print(const char *label) const {
    std::cout << label << " | this=" << this << " | sensor=" << sensor_name_
              << " | size=" << values_.size()
              << " | data=" << static_cast<const void *>(values_.data());
    if (!values_.empty()) {
      std::cout << " | first=" << values_.front();
    }
    std::cout << '\n';
  }

  // TODO 2：逐个添加下面的拷贝操作和析构函数，记录操作名、this、&other。
  MeasurementPacket(const MeasurementPacket &other):
  sensor_name_(other.sensor_name_),values_(other.values_){
    std::cout << "copy construct"
              << " | this = " << this
              << "| source = " << &other << '\n'; 

  }
  // MeasurementPacket(const MeasurementPacket &other);
  // MeasurementPacket &operator=(const MeasurementPacket &other);
  MeasurementPacket &operator = (const MeasurementPacket &other){
  sensor_name_ = other.sensor_name_;
  values_ = other.values_;
  std::cout << "copy assign"
            << " | this = " << this
            << " | source = " << &other << '\n';

  return *this;
  }

  ~MeasurementPacket()
  {
    print("destruct");
  }


  // ~MeasurementPacket();
  // 提示：构造使用初始化列表；赋值修改已有成员并 return *this。
  // 标准库成员自动释放资源，析构函数只打印，不手写 delete。
  // 添加这些函数后，隐式移动可能被抑制；TODO 3 会显式补齐移动操作。

  // TODO 3：实现移动构造和移动赋值，移动各成员并记录源/目标对象。
  // MeasurementPacket(MeasurementPacket &&other) noexcept;
  MeasurementPacket(MeasurementPacket &&other) noexcept
    :sensor_name_(std::move(other.sensor_name_)),values_(std::move(other.values_))
  {std::cout << "move construct"
             <<" | this "<< this
             <<" | source= " << &other <<'\n';
  }
  // MeasurementPacket &operator=(MeasurementPacket &&other) noexcept;
  // other 是有名字的变量，表达式 other 是左值；移动成员需要 std::move。
  // 赋值考虑 this == &other。移动后保留源对象可析构、可重新赋值的状态。

private:
  std::string sensor_name_;
  std::vector<double> values_;
};

void demoCopy() {
  std::cout << "\n[TODO 2] copy construction and copy assignment\n";
  MeasurementPacket a("IMU", 3, 0.1);
  MeasurementPacket b(a);
  MeasurementPacket c("GNSS", 2,0.0 );
  c = a;

  a.print("a");
  b.print("b");
  c.print("c");

  b.setFirst(2.0);
  assert(b.first() == 2.0);
  assert(a.first() == 0.1);

  a = a;
  assert(a.first() == 0.1);
  assert(a.size() == 3);

  std::cout <<"copy checks passed\n";

  // 创建 a("IMU", 3, 0.1)，分别进行 b = a（定义新对象）和 c = a（赋值）。
  // 修改 b 的第一个值，验证 a 未变；记录对象地址与数据地址。
  // 完成拷贝操作后测试 a = a，使用 assert 验证值与数量。
}

void demoMove() {
  std::cout << "\n[TODO 3] move construction and move assignment\n";

  MeasurementPacket a("IMU",3,0.1);

  a.print("before move a");

  MeasurementPacket b(std::move(a));

  a.print("after move a");
  b.print("after move b");

  

  // 分别验证新对象 b(std::move(a)) 与已有对象 c = std::move(b)。
  // 记录移动前后状态。源对象仍存在，不要求它必定为空或保持原值。
  // 给源对象重新赋值后再正常使用；测试自移动不会破坏类的有效状态。
}

// TODO 4：在此实现两个按值返回函数。
// makeDirectPacket(): return MeasurementPacket("IMU", 3, 0.1);
// makeNamedPacket(): 创建局部 packet 后 return packet;
// 不返回局部对象的引用，也不写 return std::move(packet)。

void demoReturn() {
  std::cout << "\n[TODO 4] return by value and copy elision\n";
  // 使用两个工厂函数初始化对象，对比默认构建和禁用可选消除的构建。
  // C++17 直接返回同类型纯右值，与命名局部变量的 NRVO 要分别解释。
}

void demoVectorGrowth() {
  std::cout << "\n[TODO 5] vector reallocation\n";
  // vector<MeasurementPacket> 与第 04 课 vector<unique_ptr<Sensor>> 不同。
  // reserve(2) 后记录真实 capacity，用 emplace_back 填满它。
  // 再 emplace_back 一次，保证触发扩容；区分新元素构造与旧元素搬迁。
  // 不假设容量恰为 2 或按两倍增长，不使用扩容前保存的元素引用/指针。
  // 对比 push_back(a)、push_back(std::move(a)) 和 emplace_back(...)。
}

// TODO 6：另写 PlainMeasurementPacket，只保存 string/vector 成员和必要接口。
// 不声明五个特殊成员函数，用它验证默认拷贝/移动，说明 Rule of Zero。
void demoRuleOfZero() {
  std::cout << "\n[TODO 6] Rule of Zero / Rule of Five\n";
}

int main() {
  std::cout << "lesson 05: measurement packet copy/move scaffold\n";
  {
    // TODO 1：读懂这段普通构造，区分包对象地址与内部数据地址。
    MeasurementPacket packet("IMU", 3, 0.1);
    packet.print("baseline");
    assert(packet.size() == 3);
    assert(packet.first() == 0.1);
  }

  demoCopy();
  demoMove();
  demoReturn();
  demoVectorGrowth();
  demoRuleOfZero();
  return 0;
}
