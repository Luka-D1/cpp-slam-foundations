#include <iostream>

struct ImuSample {
  double ax{};
  double ay{};
  double az{};
};

void incrementByValue(int value) {

  std::cout << "value is " << value << '\n';
  std::cout << "the address of value is " << &value << '\n';
  value++;
  std::cout << "after ++ value is " << value << '\n';
}

void incrementByReference(int &value) {

  std::cout << "value is " << value << '\n';
  std::cout << "the address of value is " << &value << '\n';
  value++;
  std::cout << "after ++ value is " << value << '\n';
}

void printSample (const ImuSample& sample) {
  std::cout << "ax: " << sample.ax <<'\n';
  std::cout << "ay: " << sample.ay <<'\n';
  std::cout << "az: " << sample.az <<'\n';

}

void resetSample (ImuSample* sample){
  if (sample == nullptr){
    return;
  }

  sample->ax = 0.0;
  sample->ay = 0.0;
  sample->az = 0.0;

}




int main() {

  // TODO 1: 实现 `incrementByValue(int value)`，打印函数内外的数值和地址。
  //         实现 `incrementByReference(int& value)`，与值传递的结果比较。
  int count = 1;
  ImuSample sample{0.1, -0.2, 9.8};

  std::cout << "initial count: " << count << '\n';
  std::cout << "initial ax: " << sample.ax << '\n';
  std::cout << "initial &count: " << &count << '\n';
  incrementByValue(count);
  std::cout << "count after incrementByValue: " << count << '\n';

  count = 1;

  std::cout << "initial count: " << count << '\n';
  std::cout << "initial &count: " << &count << '\n';
  incrementByReference(count);
  std::cout << "count after incrementByReference: " << count << '\n';

  // TODO 2: 使用 const 引用只读访问 ImuSample。

  printSample(sample);


  // TODO 3: 使用指针修改 ImuSample，并处理 nullptr。

  resetSample(&sample);
  printSample(sample);

  // TODO 4: 打印对象与引用/指针所指向对象的地址。
  ImuSample& ref =sample;
  ImuSample* pointer = &sample;
  //std::cout <<"the objective: " << sample << '\n';
  std::cout <<"the address of sample: " << &sample <<'\n';
  std::cout <<"the address of ref: " << &ref <<'\n';
  std::cout <<"the pointer of sample: " << pointer <<'\n';
  std::cout <<"the address of pointer: "<< &pointer <<'\n';


  return 0;
}
