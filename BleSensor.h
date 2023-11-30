#ifndef BLE_SENSOR_H
#define BLE_SENSOR_H

#include <sdbus-c++/sdbus-c++.h>

class BleSensor
{
private:
    std::unique_ptr<sdbus::IProxy> bluezProxy;
    bool getBluetoothStatus();

public:
    BleSensor();
    void scanAndConnect();
};

#endif //  BLE_SENSOR_H