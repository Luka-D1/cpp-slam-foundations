#include <iostream>
#include <string>

// 第 03 课：类、构造函数、初始化列表、析构函数与组合。
// 按照 notes/03_classes_lifecycle.md 的顺序逐个完成 TODO。

// TODO 1：定义 SensorConfig 类。
class SensorConfig
{
    private:
        std ::string name_;
        double frequency_hz_;

    public:
        SensorConfig(const std::string& name, double frequency_hz)
            : name_(name), frequency_hz_(frequency_hz) //到此实现初始化列表功能 ()中的内容实际上是形参
        {
           std::cout<<"SensorConfig constructed:"<<name_<<" frequency_hz:"<<frequency_hz_<<std::endl;
        }

        ~SensorConfig()
        {
            std::cout<<"SensorConfig::~SC()"<<name_<<std::endl;
        }
        const std::string& name() const  //此处实现name()与frequencyHz()的只读功能
        {
            return name_;
        }

        double frequency_Hz() const
        //为什么引用需要只读限制？
        //因为引用连接着原成员；副本则是独立的一份，修改副本不会影响原成员。
        {
            return frequency_hz_;
        }
};
// - 私有成员：name_（std::string）和 frequency_hz_（double）。
// - 构造函数接收 name 和 frequency_hz，并使用初始化列表初始化成员。
// - 提供只读成员函数 name() 和 frequencyHz()。
// - 在构造函数体和析构函数体中打印信息，便于观察生命周期。

// TODO 2：定义 ImuSensor 类。
class ImuSensor
{
    private:
        SensorConfig config_;
        bool running_;
    
    public:
        ImuSensor(const std::string& name,double frequency_hz,bool running)
        :config_(name,frequency_hz),running_(running)
        {
            std::cout<< "ImuSensor constructed " << name<<", "<<frequency_hz<<", "<<running<<"\n";
        }
        
        ~ImuSensor()
        {std::cout<<"ImuSensor destructed"<<"\n";}

        void start(){ running_ = true; };

        void stop(){ running_ = false; };

        void printStatus() const{ std::cout<<"name:"<<config_.name()
                            <<", frequency: "<<config_.frequency_Hz()
                            <<", running: "<<running_<<'\n';
                        };
};
// - 使用一个 SensorConfig 类型的私有成员 config_，体现“组合”。
// - 再添加一个 bool 类型的成员 running_。
// - 构造函数使用初始化列表初始化 config_ 和 running_。
// - 提供 start()、stop() 和 printStatus() 成员函数。
// - 在构造函数体和析构函数体中打印信息。

int main()
{
    std::cout << "class lifecycle exercise\n";
    SensorConfig a("imu",100.0);
    std::cout << a.name() << std::endl;
    std::cout << a.frequency_Hz() << std::endl;

    // TODO 3：创建一个额外的大括号作用域，在其中构造 ImuSensor 对象。
    // 在作用域内调用 start()、printStatus() 和 stop()。
    std::cout << " === before scope ===\n";
    {
        std ::cout << " === inside scope ===\n";

        ImuSensor imu ("x", 100.0, false);
        imu.printStatus();
        imu.start();
        imu.printStatus();
        imu.stop();
        imu.printStatus();

        std::cout << " === about to leave scope ===\n";

    }
    // TODO 4：在进入作用域前、作用域内和离开作用域后分别打印标记，
    // 记录并解释 SensorConfig 与 ImuSensor 的构造、析构顺序。
    std::cout << " === after scope ===\n";
    return 0;
}
