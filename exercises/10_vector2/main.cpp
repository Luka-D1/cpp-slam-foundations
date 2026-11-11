#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

struct Vector2 {
  double x{};
  double y{};

  // TODO 1A 在此定义成员函数 Vector2 operator+(const Vector2 &rhs) const。
  // 结果的两个分量分别相加，返回一个新的 Vector2。
};

void printComponents(const char *label, const Vector2 &v) {
  std::cout << label << " x=" << v.x << ", y=" << v.y << '\n';
}

// TODO 2A 在此定义非成员 operator*，支持 Vector2 * double 和 double * Vector2。
// 让其中一个方向复用另一个方向。

// TODO 3A 在此定义非成员 operator<<，接收 std::ostream & 和 const Vector2 &。
// 将分量写到传入的流，采用 (x, y) 格式，再返回这个流的引用。

// TODO 4A 在此定义非成员 operator== 和普通函数 isApprox。
// operator== 对两个分量做精确比较。isApprox 接收额外的绝对容差。

void demoAddition() {
  std::cout << "\n=== 实验 1 向量加法 ===\n";
  const Vector2 a{3.0, -2.0};
  const Vector2 delta{-1.0, 4.0};
  printComponents("a", a);
  printComponents("delta", delta);

  // TODO 1B 计算 a + delta，输出新向量并再次输出 a 和 delta。
}

void demoScalarMultiplication() {
  std::cout << "\n=== 实验 2 标量乘法 ===\n";
  const Vector2 a{3.0, -2.0};
  const double scale = 2.0;
  printComponents("a", a);
  std::cout << "scale=" << scale << '\n';

  // TODO 2B 分别计算 a * scale 和 scale * a，用 printComponents 输出结果。
}

void demoStreamOutput() {
  std::cout << "\n=== 实验 3 向流输出 ===\n";
  const Vector2 a{3.0, -2.0};
  printComponents("a", a);

  // TODO 3B 用 std::cout << a << '\n' 验证链式输出。
  // 再写入一个 std::ostringstream，读取它的 str() 核对内容。
}

void demoEquality() {
  std::cout << "\n=== 实验 4 精确比较和近似比较 ===\n";
  const Vector2 a{3.0, -2.0};
  const Vector2 same = a;
  const Vector2 near{3.005, -2.0};
  const Vector2 far{3.05, -2.0};
  const double abs_tol = 0.01;

  printComponents("a", a);
  printComponents("same", same);
  printComponents("near", near);
  printComponents("far", far);
  std::cout << "abs_tol=" << abs_tol << '\n';

  // TODO 4B 输出 a == same、a == near、isApprox(a, near, abs_tol) 和
  // isApprox(a, far, abs_tol) 的结果。
}

int main() {
  std::cout << std::boolalpha << std::fixed << std::setprecision(3);
  std::cout << "lesson 10: Vector2\n";
  demoAddition();
  demoScalarMultiplication();
  demoStreamOutput();
  demoEquality();
  return 0;
}
