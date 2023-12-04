#ifndef BLE_SENSOR_H
#define BLE_SENSOR_H

#include <sdbus-c++/sdbus-c++.h>
#include <condition_variable>

class BleSensor
{
private:
    inline static const std::string SERVICE_BLUEZ{"org.bluez"};
    inline static const std::string INTERFACE_ADAPTER{"org.bluez.Adapter1"};
    inline static const std::string PROPERTY_POWERED{"Powered"};
    inline static const std::string INTERFACE_PROPERTIES{"org.freedesktop.DBus.Properties"};
    std::unique_ptr<sdbus::IProxy> bluezProxy;
    std::unique_ptr<sdbus::IProxy> rootProxy;
    std::unique_ptr<sdbus::IProxy> deviceProxy;
    std::mutex mtx;
    std::condition_variable cv;
    bool connected;
    const std::string deviceName;
    bool getBluetoothStatus();
    void setBluetoothStatus(bool enable);
    void enableScanning(bool enable);
    void subscribeToInterfacesAdded();
    void connectToDevice(sdbus::ObjectPath path);

public:
    BleSensor(const std::string &sensor_name);
    void scanAndConnect();
};

#endif //  BLE_SENSOR_H