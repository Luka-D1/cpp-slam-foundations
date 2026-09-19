#include <iostream>
#include <string>

class Sensor {
public:
  virtual ~Sensor() { std::cout << "Sensor destructor\n"; }

  virtual std::string name()
      const = 0; // virtual 允许动态多态；=0 这是纯虚函数； const 调用name()
                 // 不应该修改对象状态
                 //表示所有具体传感器必须提供读取方法，不然Sensor不知道具体如何读取
  virtual void read() = 0;
};

class ImuSensor : public Sensor {
public:
  ~ImuSensor() override { std::cout << "ImuSensor destructor\n"; }

  std::string name() const override { return "IMU"; }

  void read() override {
    std::cout << "IMUDATA,2320,268857.283,"
              << "-0.00597353690,     0.00835359731,    -0.00161346283,"
              << "-0.10190863674,     0.06805694516,   -10.04755315296, "
              << '\n';
  }
};

class GnssSensor : public Sensor {
public:
  ~GnssSensor() override { std::cout << "GnssSensor destructor\n"; }

  std::string name() const override { return "GNSS"; }

  void read() override {
    std::cout
        << "GNSSDATA 268626.00000000	40.07008828	116.27570961	"
           "36.36397054	1.89060000	1.57320000	1.69100000"
        << '\n';
  }
};

void printSensorData(Sensor &sensor) {
  std::cout << sensor.name() << '\n';
  sensor.read();
}

int main() {
  ImuSensor imu;
  GnssSensor gnss;
  printSensorData(imu);
  printSensorData(gnss);
  return 0;
}
