#include <cstddef>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// 第 07 课：按名称管理传感器配置。
// 配套 notes/07_map_set_unordered_map.md；保持四个独立实验。
// 示例数据、打印工具已提供，容器操作由你填写。
struct SensorInfo {
  int id{};
  double frequency_hz{};
  bool enabled{};
};

struct SensorRecord {
  std::string name;
  SensorInfo info;
};

std::vector<SensorRecord> makeTrainingRecords() {
  // 故意不按名称顺序提供，观察 map/set 的顺序。
  return {{"ODO", {3, 20.0, false}},
          {"IMU", {1, 200.0, true}},
          {"GNSS", {2, 10.0, true}}};
}

std::vector<std::string> makeReportedNames() {
  return {"IMU", "GNSS", "IMU", "ODO", "GNSS"};
}

void printInfo(const SensorInfo &info) {
  std::cout << "id=" << info.id << " | frequency_hz=" << info.frequency_hz
            << " | enabled=" << info.enabled;
}

void printOrderedRegistry(const std::string &label,
                          const std::map<std::string, SensorInfo> &registry) {
  std::cout << label << " | size=" << registry.size() << '\n';
  for (const auto &entry : registry) {
    std::cout << "  " << entry.first << " | ";
    printInfo(entry.second);
    std::cout << '\n';
  }
}

void printNames(const std::set<std::string> &names) {
  std::cout << "unique names | size=" << names.size() << '\n';
  for (const std::string &name : names) {
    std::cout << "  " << name << '\n';
  }
}

void printHashedRegistry(
    const std::string &label,
    const std::unordered_map<std::string, SensorInfo> &registry) {
  std::cout << label << " | size=" << registry.size() << '\n';
  for (const auto &entry : registry) {
    std::cout << "  " << entry.first << " | ";
    printInfo(entry.second);
    std::cout << '\n';
  }
}

void printHashStats(
    const std::string &label,
    const std::unordered_map<std::string, SensorInfo> &registry) {
  std::cout << label << " | size=" << registry.size()
            << " | buckets=" << registry.bucket_count()
            << " | load_factor=" << registry.load_factor()
            << " | max_load_factor=" << registry.max_load_factor() << '\n';
}

void demoOrderedRegistry() {
  std::cout << "\n=== 实验 1：map 注册、查询和更新 ===\n";
  const std::vector<SensorRecord> source = makeTrainingRecords();
  std::map<std::string, SensorInfo> registry;

  // TODO 1：用 insert 将 source 注册进 map，打印按键排列的结果。
  for (const auto& record: source){
    registry.insert({record.name,record.info});
  }
  printOrderedRegistry("注册后",registry);

  // 重复插入 IMU 的 {99, 999.0, false}，查看返回 pair 的 second，
  const auto result = registry.insert({"IMU",SensorInfo{99, 999.0, false}});
  std::cout << "duplicate IMU inserted= " << result.second << '\n';
  std::cout << "IMU after duplicate insert | ";
  printInfo(result.first -> second);
  std::cout << '\n';


  
  // 验证原 IMU 配置未覆盖。find IMU 后将频率更新为 250.0。
  auto it = registry.find("IMU");

  if (it != registry.end()){
    it -> second.frequency_hz = 250.0;
    std::cout << "IMU after update | ";
    printInfo(it -> second);
    std::cout << '\n';
  } else {
    std::cout << "IMU not found\n";
  }

  // find LIDAR，先判断 end，确认查询没有插入新元素。
  auto it2 = registry.find("LIDAR");

  if (it2 == registry.end()){
      std::cout << "Lidar unfound." << "\n" ;
  } 




  // erase ODO 两次，记录两次返回的删除数量；最终剩 GNSS、IMU。
  auto erased1 = registry.erase("ODO");
  auto erased2 = registry.erase("ODO");

  std:: cout << "first erase= " << erased1 << "\n";
  std:: cout << "second erase= " << erased2 << "\n";

  printOrderedRegistry("final registry", registry);


  std::cout << "source records=" << source.size() << '\n';
  printOrderedRegistry("registry", registry);
}





void demoUniqueNames() {
  std::cout << "\n=== 实验 2：set 去重与有序查找 ===\n";
  const std::vector<std::string> reported = makeReportedNames();
  std::set<std::string> names;

  // TODO 2：插入 reported 中的名称，记录每次插入的 second。
  for (const auto& report: reported){

      auto it = names.insert(report);
      std::cout << "it.second= " << it.second << '\n';

  }
  printNames(names);


  // 观察有序且不重复的结果；find/count 查询 IMU 与 LIDAR。
  auto find1 = names.find("IMU");
  if (find1 != names.end()){
  std::cout << "IMU find1(obj): " << *find1 << '\n';
  }
  else{std::cout << "IMU unfound" << '\n' ;}

  auto count1 = names.count("IMU");
  std::cout << "IMU count1(number): " << count1 << '\n';


  auto find2 = names.find("LIDAR");
  if (find2 != names.end()){
  std::cout << "LIDAR find2(obj): " << *find2 << '\n';
  }
  else { std::cout << "LIDAR unfound" << '\n' ;}

  auto count2 = names.count("LIDAR");
  std::cout << "LIDAR count2(number): " << count2 << '\n';

  // 查看 lower_bound(\"LIDAR\")、upper_bound(\"IMU\")、
  // lower_bound(\"ZZZ\")；每次读取前都先检查是否为 end。
  auto bound1 = names.lower_bound("LIDAR");

  if (bound1 != names.end()){
    std::cout << "bound1 (an element >= LIDAR) = " << *bound1 << '\n';
  } else { std::cout << "unfound an element >= LIDAR" ;}


  auto bound2 = names.upper_bound("IMU");

  if (bound2 != names.end()){
    std::cout << "bound2 (an element > IMU) = " << *bound2 << '\n';
  } else { std::cout << "unfound an element > IMU" ;}


  auto bound3 = names.lower_bound("ZZZ");

  if (bound3 != names.end()){
    std::cout << "bound3 (an element >= ZZZ) = " << *bound3 << '\n';
  } else { std::cout << "unfound an element >= ZZZ" << '\n' ;}

  // 最后 erase GNSS，解释为什么不能直接修改 set 中的名称。

  auto erased = names.erase("GNSS");
  std::cout << "erased GNSS count= " << erased <<'\n';

  std::cout << "reported names=" << reported.size() << '\n';
  printNames(names);
}

void demoHashedRegistry() {
  std::cout << "\n=== 实验 3：unordered_map 与桶 ===\n";
  const std::vector<SensorRecord> source = makeTrainingRecords();
  std::unordered_map<std::string, SensorInfo> registry;

  // TODO 3：用与实验 1 相同的 source 填充哈希注册表并按名称查询。
  // 不要求遍历顺序与 map 相同；记录 size、bucket_count、load_factor。
  // reserve(32) 后观察统计，不能把 bucket_count 当作 vector 的 capacity。
  // 要确保一次桶数增长，保存当前桶数再 rehash(old_bucket_count + 1)。
  // rehash 前让旧迭代器离开局部作用域，之后重新 find IMU。
  // 进阶：保留指向 IMU 的 SensorInfo 的指针，验证未删除元素时
  // rehash 不使该指针失效；与第 06 课 vector 扩容作对比。
  std::cout << "source records=" << source.size() << '\n';
  printHashStats("initial stats", registry);
  printHashedRegistry("hashed registry", registry);
}

void demoAccessAndChoice() {
  std::cout << "\n=== 实验 4：查询陷阱、更新策略与容器选择 ===\n";
  std::map<std::string, SensorInfo> registry{{"IMU", {1, 200.0, true}},
                                             {"GNSS", {2, 10.0, true}}};

  // TODO 4：对比 find(\"LIDAR\") 与 registry[\"LIDAR\"] 后的 size。
  // 后者会插入默认 SensorInfo；打印它，再 erase 掉误插入项。
  // insert_or_assign 更新 IMU 为 {1, 400.0, true}，解释 second=false。
  // try_emplace 再插入同键 IMU {99, 999.0, false}，验证没有覆盖。
  // try_emplace 新键 ODO，值为 SensorInfo{3, 20.0, false}。
  // 用迭代器循环删除 enabled=false 的项，接住 erase 返回值。
  // 书面回答：按名称排序输出、只做快速查找、只做去重，分别选谁？
  printOrderedRegistry("initial registry", registry);
}

int main() {
  std::cout << std::boolalpha << std::fixed << std::setprecision(1);
  std::cout << "lesson 07: sensor registry\n";
  demoOrderedRegistry();
  demoUniqueNames();
  demoHashedRegistry();
  demoAccessAndChoice();
  return 0;
}
