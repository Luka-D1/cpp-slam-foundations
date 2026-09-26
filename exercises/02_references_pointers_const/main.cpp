#include <iostream>

struct ImuSample {
  double ax{};
  double ay{};
  double az{};
};

void incrementByValue(int value){

  std::cout <<"value is "<< value <<'\n';
  std::cout <<"the address of value is " << &value <<'\n';
  value++;
  std::cout <<"after ++ value is " << value <<'\n';

}

void incrementByReference(int &value){

  std::cout <<"value is "<< value <<'\n';
  std::cout <<"the address of value is " << &value <<'\n';
  value++;
  std::cout <<"after ++ value is " << value <<'\n';

}









int main() {
  int count = 1;
  ImuSample sample{0.1, -0.2, 9.8};

  std::cout << "initial count: " << count << '\n';
  //std::cout << "initial ax: " << sample.ax << '\n';
  std::cout <<"initial &count: " << &count << '\n';
  incrementByValue(count);


  count = 1;

  std::cout << "initial count: " << count << '\n';
  //std::cout << "initial ax: " << sample.ax << '\n';
  std::cout <<"initial &count: " << &count << '\n';
  incrementByReference(count);

  // TODO 1: 比较值传递和引用传递。
  // TODO 2: 使用 const 引用只读访问 ImuSample。
  // TODO 3: 使用指针修改 ImuSample，并处理 nullptr。
  // TODO 4: 打印对象与引用/指针所指向对象的地址。

  return 0;
}
