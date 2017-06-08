#include <iostream>
#if __cplusplus > 199711L
// For C++11, it's greater than 199711L.
    #include <chrono>
#else
    #include <ctime>
#endif
// For sleep:
#include <thread>

using namespace std;

class Simple_PID {
    /*
     * Based on
     * https://en.wikipedia.org/wiki/PID_controller
    */
public:
    Simple_PID() {}
    Simple_PID(double Kp, double Ki, double Kd):
        Kp(Kp), Ki(Ki), Kd(Kd)
    {}
    virtual ~Simple_PID() {}

    void initTime()
    {
#if __cplusplus > 199711L
        prevT = std::chrono::system_clock::now();
#else
        prevT = std::time(0);
#endif
    }

    double update(double setpoint, double current_value, double dt)
    {
        error = setpoint - current_value;
        integral = integral + error*dt;
        derivative = (error - previous_error)/dt;
        output = Kp*error + Ki*integral + Kd*derivative;
        previous_error = error;
        return output;
    }

    double update(double setpoint, double current_value)
    {
#if __cplusplus > 199711L
        curT = std::chrono::system_clock::now();
//        curT = std::chrono::high_resolution_clock::now();
        elapsed_seconds = curT - prevT;
        prevT = curT;
        dt = elapsed_seconds.count();
#else
        curT = std::time(0);
        dt = difftime(curT, prevT);
        prevT = curT;
#endif
        error = setpoint - current_value;
        integral = integral + error*dt;
        derivative = (error - previous_error)/dt;
        output = Kp*error + Ki*integral + Kd*derivative;
        previous_error = error;
        return output;
    }

    double get_Kp() const {return Kp;}
    void set_Kp(double value) {this->Kp = value;}
    double get_Ki() const {return Ki;}
    void set_Ki(double value) {this->Ki = value;}
    double get_Kd() const {return Kd;}
    void set_Kd(double value) {this->Kd = value;}

private:
    double previous_error = 0.;
    double integral = 0.;
    double derivative = 0.;
    double error = 0.;
    double dt = 0.;

    double Kp = 0.;
    double Ki = 0.;
    double Kd = 0.;
    double output = 0.;

#if __cplusplus > 199711L
    // For C++11
    std::chrono::time_point<std::chrono::system_clock> prevT, curT;
    std::chrono::duration<double> elapsed_seconds;
#else
    time_t prevT, curT;
#endif
};

int main()
{
    cout << "Simple PID example:" << endl;
    Simple_PID pid(0.02, 0.01, 0.01);
    pid.initTime();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    cout << pid.update(0.9, 0.3) << endl;
//    using namespace std::chrono_literals;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    cout << pid.update(0.9, 0.3) << endl;
    // Output may use as delta between current value and new set (output) value.
    return 0;
}
