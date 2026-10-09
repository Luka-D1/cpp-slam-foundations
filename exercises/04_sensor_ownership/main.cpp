#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

class Sensor {
public:
  virtual ~Sensor() { std::cout << "Sensor destroyed\n"; }

  virtual std::string name() const = 0;
  virtual void read() const = 0;
};

class ImuSensor final : public Sensor {
public:
  ImuSensor() { std::cout << "ImuSensor constructed\n"; }
  ~ImuSensor() override { std::cout << "ImuSensor destroyed\n"; }

  std::string name() const override { return "IMU"; }
  void read() const override { std::cout << "read IMU data\n"; }
};

class GnssSensor final : public Sensor {
public:
  GnssSensor() { std::cout << "GnssSensor constructed\n"; }
  ~GnssSensor() override { std::cout << "GnssSensor destroyed\n"; }

  std::string name() const override { return "GNSS"; }
  void read() const override { std::cout << "read GNSS data\n"; }
};

void printSensorData(const Sensor &sensor) {
  std::cout << "sensor: " << sensor.name() << '\n';
  sensor.read();
}

int main() {
  std::cout << "sensor ownership exercise\n";

  // TODO 1：使用 make_unique 创建一个由 unique_ptr 独占的 ImuSensor。
  // 调用 name() 和 read()，观察作用域结束时的析构输出。
  auto imu = std::make_unique<ImuSensor>();
  std::cout<< imu->name() <<'\n';
  imu->read();

  // TODO 2：把一个 unique_ptr 的所有权移动给另一个 unique_ptr。
  // 预测并验证移动后两个指针的状态。不要复制 unique_ptr，
  // 也不要使用裸 new/delete。
  auto imu2 = std::move(imu);
  std::cout<< "imu is null: "<<(imu == nullptr) <<'\n';
  std::cout<< "imu2 is null: "<<(imu2 == nullptr) <<'\n';

  // TODO 3：创建 vector<unique_ptr<Sensor>>。
  // 分别创建 ImuSensor 和 GnssSensor，并把所有权移入容器。

  // TODO 4：遍历容器。
  // 判空后把每个 Sensor 交给 printSensorData()，验证多态仍然生效。

  // TODO 5：建立最小 shared_ptr/weak_ptr 实验。
  // 观察 use_count()；验证 weak_ptr 不增加计数；
  // 用 lock() 临时访问，并在所有 shared_ptr 销毁后检查 expired()。

  // TODO 6：用额外的大括号划分作用域。
  // 运行前预测传感器的析构时机与顺序，再记录实际输出。

  return 0;
}
