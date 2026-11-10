#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

// 第 09 课练习把处理规则作为参数。四个实验可以独立完成。
struct Measurement {
  int timestamp_ms{};
  double residual_m{};
};

double correctResidual( const double residual_m, const double bias_m){
  return residual_m - bias_m;
}

double correctResidual( const Measurement& sample, const double bias_m){
  return correctResidual(sample.residual_m , bias_m);
}

void demoOverloads() {
  std::cout << "\n=== 实验 1 函数重载 ===\n";
  const Measurement sample{400, 2.50};
  const double bias_m = 0.50;

  // TODO 1：在本函数上方定义两个返回 double 的 correctResidual 重载。
  // 一个接收原始残差和偏差，另一个接收 const Measurement & 和偏差。
  const double residual_m = 2.50;


  const double corrected1_m =
    correctResidual(residual_m, bias_m);

  const double corrected2_m = 
    correctResidual(sample, bias_m);

  // 第二个重载取出 residual_m，再调用第一个。分别调用并打印校正值。


  std::cout << "原始残差=" << sample.residual_m << " m | 偏差=" << bias_m
            << " m\n";
    std::cout << "纠正后残差1= " << corrected1_m << "m\n" << "纠正后残差2= " << corrected2_m
            << " m\n";
}


inline bool exceedsLimit(
  double residual_m,
  double limit_m = 2.0
){
  return (std::abs(residual_m) > limit_m);
}



void demoDefaultArgumentAndInline() {
  std::cout << "\n=== 实验 2 默认实参与 inline ===\n";
  const double residual_m = -2.50;

  // TODO 2：在本函数上方定义 inline bool exceedsLimit。
  // 第二个参数 limit_m 默认为 2.0，判断残差幅值是否严格超过门限。
  // 对 -2.50 m 分别使用默认门限和 3.0 m。再测恰好在门限上的 -2.0 m。
  bool correct1 = exceedsLimit(residual_m);
  bool correct2 = exceedsLimit(residual_m, 3.0);
  bool correct3 = exceedsLimit(-2.0);


  std::cout << "待检查残差=" << residual_m << " m\n";

  std::cout << "默认门限下resisual_m是否超过限差: " << correct1 << '\n' ;
  std::cout << "3.0m门限下resisual_m是否超过限差: " << correct2 << '\n' ;
  std::cout << "默认门限下 -2.0 m 是否超过限差：" << correct3 << '\n' ;

}

double keepResidual(double residual_m) { return residual_m; }

double absoluteResidual(double residual_m) { return std::abs(residual_m); }

using ResidualFn = double (*)(double);

void printWithFunctionPointer(double x, ResidualFn step){
  if (step != nullptr){
    std::cout << step(x) << '\n' ;
  } else {std::cout << "传入的step是空指针" << '\n' ;}
}



void demoFunctionPointer() {
  std::cout << "\n=== 实验 3 通过函数指针替换处理规则 ===\n";
  const double raw_m = -2.50;

  // TODO 3：在本函数上方定义 printWithFunctionPointer。
  // 它接收 double raw_m 和 ResidualFn step，先检查 step 是否为空，

  printWithFunctionPointer(raw_m, keepResidual);
  printWithFunctionPointer(raw_m, absoluteResidual);
  printWithFunctionPointer(raw_m, nullptr);
  printWithFunctionPointer(raw_m, 0);
  //printWithFunctionPointer(raw_m); 这里显示too few arguments to fundtion 由此可见 不传入step函数的时候会报错
  // 再调用 step(raw_m) 并打印结果。
  
  // 分别传入 keepResidual、absoluteResidual、nullptr 进行核对。

  std::cout << "待处理残差=" << raw_m << " m\n";
}

using ResidualStep = std::function<double(double)>;

void printPipelineResult(
  double raw_m,
  const std::vector<ResidualStep>& steps){
  
double current_m = raw_m;

for (const auto& step : steps){
  if (!step){
    std::cout << "空步骤，跳过\n" ;
    continue;
  }

  current_m = step(current_m);
  std::cout << "本部结果= " << current_m << " m\n";
}
  }



void demoPipeline() {
  std::cout << "\n=== 实验 4 用 std::function 串联处理步骤 ===\n";
  const double raw_m = -2.50;

  // TODO 4：在本函数上方定义 printPipelineResult。
  // 输入原始残差和 const std::vector<ResidualStep> &steps。
  // 逐个调用非空步骤，将结果作为下一步输入，并打印每步结果。


  // 在本函数内组装两步：先取绝对值，再用捕获门限的 lambda 截断幅值。
  const double limit_m = 1.50;

  for (double limit_m : {1.50, 0.75}) {
  std::vector<ResidualStep> steps{
    absoluteResidual,
    [limit_m](double x) {
      return std::min(x, limit_m);
    }
  };

  std::cout << "门限=" << limit_m << " m\n";
  printPipelineResult(raw_m, steps);
  // 分别用 1.50 m 和 0.75 m 的门限运行同一个处理函数。


  std::cout << "管线输入=" << raw_m << " m\n";
}
}

int main() {
  std::cout << std::boolalpha << std::fixed << std::setprecision(2);
  std::cout << "lesson 09: processing pipeline\n";
  demoOverloads();
  demoDefaultArgumentAndInline();
  demoFunctionPointer();
  demoPipeline();
  return 0;
}
