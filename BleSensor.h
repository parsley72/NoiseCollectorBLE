#ifndef BLE_SENSOR_H
#define BLE_SENSOR_H

#include <sdbus-c++/sdbus-c++.h>

class BleSensor
{
private:
    inline static const std::string INTERFACE_ADAPTER{"org.bluez.Adapter1"};
    inline static const std::string PROPERTY_POWERED{"Powered"};
    inline static const std::string INTERFACE_PROPERTIES{"org.freedesktop.DBus.Properties"};
    std::unique_ptr<sdbus::IProxy> bluezProxy;
    std::unique_ptr<sdbus::IProxy> rootProxy;
    bool getBluetoothStatus();
    void setBluetoothStatus(bool enable);
    void enableScanning(bool enable);
    void subscribeToInterfacesAdded();

public:
    BleSensor();
    void scanAndConnect();
};

#endif //  BLE_SENSOR_H