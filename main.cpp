#include "BleSensor.h"
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    std::cout << "Noise Collector BLE" << std::endl;
    BleSensor bleSensor{"RP2-SENSOR"};
    bleSensor.scanAndConnect();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    bleSensor.getValue();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
