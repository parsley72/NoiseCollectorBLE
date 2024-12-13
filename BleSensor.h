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
    std::unique_ptr<sdbus::IProxy> _bluezProxy;
    std::unique_ptr<sdbus::IProxy> _rootProxy;
    std::unique_ptr<sdbus::IProxy> _deviceProxy;
    std::unique_ptr<sdbus::IProxy> _tempAttrProxy;
    std::mutex _mtx;
    std::condition_variable _cv;
    bool _connected;
    const std::string _deviceName;

    bool getBluetoothStatus();
    void setBluetoothStatus(bool enable);
    void enableScanning(bool enable);
    void subscribeToInterfacesAdded();
    void connectToDevice(sdbus::ObjectPath path);
    void readTemperature();
    float valueFromIeee11073(std::vector<std::uint8_t> binary);
    void disconnectFromDevice();

public:
    BleSensor(const std::string &sensor_name);
    void scanAndConnect();
    void getValue();
    void disconnect();
};

#endif //  BLE_SENSOR_H
