#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// 第 06 课：同一段 IMU 数据，从保存、求均值到安全删除和扩容。
// 配套 notes/06_string_array_vector.md；共四个实验，按顺序完成。
// 示例数据和打印函数已提供，核心操作由你实现。
struct ImuSample {
  double timestamp_s{};
  std::array<double, 3> acceleration_mps2{};
};

std::vector<ImuSample> makeTrainingSamples() {
  // 人工教学数据：第三、第四条的 ax 故意异常，用于检查相邻删除。
  return {{0.00, {0.1, -0.2, 9.8}}, {0.01, {0.2, -0.1, 9.7}},
          {0.02, {50.0, 0.0, 9.8}}, {0.03, {-60.0, 0.0, 9.8}},
          {0.04, {0.3, 0.0, 9.9}},  {0.05, {-0.1, 0.1, 9.8}}};
}

void printAxes(const std::array<double, 3> &axes) {
  std::cout << '[' << axes[0] << ", " << axes[1] << ", " << axes[2] << ']';
}

void printSequence(const std::string &label,
                   const std::vector<ImuSample> &samples) {
  std::cout << label << " | size=" << samples.size()
            << " | capacity=" << samples.capacity() << '\n';
  for (const ImuSample &sample : samples) {
    std::cout << "  t=" << sample.timestamp_s << " | acceleration=";
    printAxes(sample.acceleration_mps2);
    std::cout << '\n';
  }
}

bool meanAcceleration(const std::vector<ImuSample> &samples,
                      std::array<double, 3> &mean) {
  mean = {0.0, 0.0, 0.0};
  if (samples.empty()) {
    return false;
  }

  // 实验 2 的核心：用范围 for 累加三轴，再除以样本数量。
  // 写完计算逻辑后将下面的 return false 改为 return true。
  return false;
}

void removeOutliers(std::vector<ImuSample> &samples, double threshold) {
  // 实验 3 的核心：用迭代器遍历，删除 abs(ax) > threshold 的采样。
  // 下面两行只是让未完成的骨架无未使用参数警告，实现后可删除。
  (void)samples;
  (void)threshold;
}

void demoNameAndAxes() {
  std::cout << "\n=== 实验 1：名称与三轴数据 ===\n";
  std::string sensor_name{"imu"};
  std::array<double, 3> axes{0.1, -0.2, 9.8};

  // TODO 1：给名称追加 "_01"，打印名称、长度、是否为空。
  sensor_name += "_01";
  std::cout << "sensor = " << sensor_name <<'\n';
  std::cout << "size = " << sensor_name.size() <<'\n';
  std::cout << "empty = " << sensor_name.empty() <<'\n';
  // 拼出 "sensor=imu_01"，查找 "imu" 并判断是否找到。
  std::string description{"sensor="};
  description += sensor_name;
  std::cout << description <<'\n';

  auto position = sensor_name.find("imu");
  if (position != std::string::npos){
    std::cout << "found imu at " << position <<'\n';
  } else {
    std::cout << "imu not found\n";
  }


  // 读取并修改 axes.at(0)，打印三个轴及 axes.size()。
  std::cout << "axes.at(0)= " << axes.at(0) << '\n';
  axes.at(0) = 0.15;
  
  for (std::size_t i=0; i < axes.size(); ++i){
    std::cout << "修改后：axes.at(" << i << ") = " << axes.at(i) <<'\n';
  }

  std::cout << "axes.size()= " << axes.size();

  // array 的长度固定，但元素不是只读的；先预测改值后的结果。
  std::cout << "name=" << sensor_name << " | axes=";
  printAxes(axes);
  std::cout << '\n';
}

void demoVectorAndMean() {
  std::cout << "\n=== 实验 2：保存序列、大小/容量与均值 ===\n";
  const std::vector<ImuSample> source = makeTrainingSamples();
  std::vector<ImuSample> samples;

  // TODO 2：先打印 samples 的 size/capacity，再 reserve(8) 并重新打印。
  std::cout << "size= " << samples.size() <<'\n';
  std::cout << "capacity= " << samples.capacity() <<'\n';
  samples.reserve(8);
  std::cout << "after reserve size= " << samples.size() <<'\n';
  std::cout << "after reserve capacity= " << samples.capacity() <<'\n';
  std::cout << "source.size()= " << source.size() <<'\n';


  // 用 const ImuSample& 范围 for 读取 source，push_back 到 samples。
  for (const ImuSample& sample : source ){
    samples.push_back(sample);
  }

  std::cout << "after source size= " << samples.size() <<'\n';
  std::cout << "after source capacity= " << samples.capacity() <<'\n';

  // samples 非空后，用 at(0)、front()、back() 读取采样。
  if (!samples.empty()){
    const ImuSample& first = samples.at(0);
    
    std::cout << "at(0): t = " << first.timestamp_s << " | acceleration = ";
    printAxes(first.acceleration_mps2);
    std::cout << "\n";

    const ImuSample& front = samples.front();
    std::cout << "front: t = " << front.timestamp_s << " | acceleration = ";
    printAxes(front.acceleration_mps2);
    std::cout << "\n";

    const ImuSample& back = samples.back();
    std::cout << "back: t = " << back.timestamp_s << " | acceleration = ";
    printAxes(back.acceleration_mps2);
    std::cout << "\n";


  }
  // 另建 probe = samples，在 probe 上依次观察 resize(2)、resize(4)、
  std::vector<ImuSample> probe = samples;
  std::cout << "before: size= " << probe.size() 
            << " | capacity = " << probe.capacity() <<'\n' ;
  
  probe.resize(2);
  std::cout << "after: size= " << probe.size()
       <<" | capacity= " << probe.capacity() << '\n';
  
  const ImuSample& first = probe.front();
  std::cout << "front.timestamp= " << first.timestamp_s << '\n';
  
  const ImuSample& last = probe.back();
  std::cout << "last.timestamp= " << last.timestamp_s << '\n';




  // pop_back()、clear()，每次打印 size/capacity，samples 保持完整。
  // 完成 meanAcceleration()，求 samples 三轴均值；再传入空序列试一次。

  printSequence("samples", samples);
  std::cout << "source count=" << source.size() << '\n';
  std::array<double, 3> mean{};
  if (meanAcceleration(samples, mean)) {
    std::cout << "mean=";
    printAxes(mean);
    std::cout << '\n';
  } else {
    std::cout << "当前没有均值；请先填入采样并完成实验 2 的计算逻辑。\n";
  }
}

void demoIteratorAndErase() {
  std::cout << "\n=== 实验 3：遍历并删除相邻异常采样 ===\n";
  std::vector<ImuSample> samples = makeTrainingSamples();
  printSequence("before erase", samples);

  // TODO 3：先用显式迭代器读取采样，理解 begin/end、*it、it-> 和 ++it。
  // 在 removeOutliers() 中实现安全删除：删除分支接住 erase 返回的迭代器，
  // 未删除分支才 ++it。不要在范围 for 中直接 erase 当前 vector 的元素。
  // 删除后再算均值；还要测试空序列、全部正常、全部异常这三种情况。
  removeOutliers(samples, 10.0);
  printSequence("after erase (删除逻辑由你完成)", samples);
}

void demoReallocation() {
  std::cout << "\n=== 实验 4：扩容后重新获取元素 ===\n";
  std::vector<ImuSample> samples = makeTrainingSamples();
  const std::size_t saved_index = 0;

  // TODO 4：保存当前 capacity，观察 saved_index 对应元素的地址和值。
  // 在一个内部大括号里取得该元素的引用、指针和迭代器，并在扩容前使用。
  // 让这些局部变量离开作用域，再 reserve(old_capacity + 1) 保证扩容。
  // 扩容后按 saved_index 重新取得元素，检查时间戳、数据、size 未变。
  // 只比较预先记录的地址文本；不要读取、比较或解引用旧的失效指针/迭代器。
  // 写出“不扩容的 push_back”和“erase”分别会使哪些位置失效。
  std::cout << "index=" << saved_index << " | initial size=" << samples.size()
            << " | initial capacity=" << samples.capacity() << '\n';
}

int main() {
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "lesson 06: IMU sequence\n";
  demoNameAndAxes();
  demoVectorAndMean();
  demoIteratorAndErase();
  demoReallocation();
  return 0;
}
