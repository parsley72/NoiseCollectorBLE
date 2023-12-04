#include "BleSensor.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Noise Collector BLE" << std::endl;
    BleSensor bleSensor{"RP2-SENSOR"};
    bleSensor.scanAndConnect();

    return 0;
}
