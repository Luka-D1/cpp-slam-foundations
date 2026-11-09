#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

// 第 08 课：同一批模拟观测，分别练习查找、排序、筛选和统计。
// 配套 notes/08_stl_algorithms_lambda.md。四个实验可以独立完成。
struct Measurement {
  int timestamp_ms{};   //时间戳 单位为ms毫秒
  double residual_m{};  //残差 单位为米
  bool valid{};         //这条观测是否有效
};

std::vector<Measurement> makeTrainingMeasurements() {
  // 故意打乱时间顺序，并包含两条已标记无效的观测。
  return {{300, 9.00, false},
          {400, 2.50, true},
          {100, 1.25, true},
          {500, 7.00, false},
          {200, 0.75, true}};
}

void printMeasurement(const Measurement &sample) {
  std::cout << "  t=" << sample.timestamp_ms
            << " ms | residual=" << sample.residual_m
            << " m | valid=" << sample.valid << '\n';
}

void printMeasurements(const char *label,
                       const std::vector<Measurement> &samples) {
  std::cout << label << " | size=" << samples.size() << '\n';
  for (const Measurement &sample : samples) {
    printMeasurement(sample);
  }
}

void demoFindMeasurement(double threshold_m) {
  std::cout << "\n=== 实验 1：find_if 查找，阈值 " << threshold_m << " m ===\n";
  const std::vector<Measurement> samples = makeTrainingMeasurements();

  // TODO 1：用 lambda 按值捕获 threshold_m，判断观测有效且残差严格大于阈值。
  auto it = std::find_if(
    samples.begin(),
    samples.end(),
    [threshold_m](const Measurement &sample){
      return sample.valid && sample.residual_m > threshold_m;
    }

  );
  // 用 find_if 找到第一条满足条件的观测。先与 end 比较，再决定是
  // 调用 printMeasurement 打印结果，还是提示未找到。
  if ( it != samples.end()){
    printMeasurement( *it );
  }else {std::cout << "Unfound target"<< '\n';}


  // main 会分别传 2.0 和 3.0，核对命中和未命中两种情况。

}

void demoSortMeasurements() {
  std::cout << "\n=== 实验 2：sort 按时间排序 ===\n";
  std::vector<Measurement> samples = makeTrainingMeasurements();
  printMeasurements("排序前", samples);

  // TODO 2：用无捕获 lambda 比较两条观测的 timestamp_ms，
  // 并把它传给 std::sort。只比较时间戳，使用严格小于。

  std::sort(
    samples.begin(),
    samples.end(),
    [](const Measurement& lhs,
       const Measurement& rhs){
       return lhs.timestamp_ms < rhs.timestamp_ms;
       }
  );

  printMeasurements("排序结果", samples);

  std::sort(
    samples.begin(),
    samples.end(),
    [](const Measurement& lhs, const Measurement& rhs) {
        return lhs.residual_m > rhs.residual_m;     //虽然说要严格满足弱序排列 但其实这里只要不带上等于号 都能够进行排列
    }
  );
  printMeasurements("排序结果", samples);
}

struct IsInvalid {
  // TODO 3A：实现 bool operator()(const Measurement &sample) const。
  bool operator() (const Measurement &sample) const{
      return sample.valid == false;
  }

  // 当 sample 无效时返回 true：remove_if 的 true 表示要移除。
};

void demoFilterMeasurements() {
  std::cout << "\n=== 实验 3：函数对象和 erase-remove ===\n";
  std::vector<Measurement> samples = makeTrainingMeasurements();
  printMeasurements("筛选前", samples);

  // TODO 3B：把 IsInvalid{} 交给 std::remove_if，接住返回的逻辑终点。
  auto it = std::remove_if(
    samples.begin(),
    samples.end(),
    IsInvalid{}
  );
  // 先打印 remove_if 后的 size，不读尾段元素。
  std:: cout << "remove_if后size()= " << samples.size() << '\n';

  // 再用 vector::erase 删除逻辑终点至 end 的尾段。
  samples.erase(it, samples.end());

  printMeasurements("筛选结果（完成 TODO 3 后核对）", samples);
}

void demoSummarizeResiduals() {
  std::cout << "\n=== 实验 4：transform 提取与 accumulate 求和 ===\n";
  // 直接给出实验 3 应保留的三条观测，让本实验可以独立运行。
  const std::vector<Measurement> valid_samples{
      {400, 2.50, true}, {100, 1.25, true}, {200, 0.75, true}};
  std::vector<double> residuals;
  printMeasurements("待统计观测", valid_samples);

  // TODO 4：用 transform 和只读 lambda 提取 residual_m，
  // 通过 std::back_inserter(residuals) 写入结果。
  std::transform(
    valid_samples.begin(),
    valid_samples.end(),
    std::back_inserter(residuals),
    [](const Measurement& sample){
      return sample.residual_m;
    }

  );
  
  // 用 accumulate 从 0.0 开始求和；确认非空后再计算均值。




  double sum = std::accumulate(
    residuals.begin(),
    residuals.end(),
    0.0
  );
  std::cout << "残差总和为：" << sum << '\n';

  if ( !residuals.empty()){
  double mean = sum / residuals.size();
  std::cout << "残差的平均值为：" << mean << '\n';
  }else {std::cout << "没有可用于统计的残差值" << '\n' ;}

  std::cout << "残差数量=" << residuals.size() << '\n';
  for (const double residual : residuals) {
    std::cout << "  residual=" << residual << " m\n";
  }
}

int main() {
  std::cout << std::boolalpha << std::fixed << std::setprecision(2);
  std::cout << "lesson 08: measurement filter\n";
  demoFindMeasurement(2.0);
  demoFindMeasurement(3.0);
  demoSortMeasurements();
  demoFilterMeasurements();
  demoSummarizeResiduals();
  return 0;
}
