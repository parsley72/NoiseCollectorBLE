#include "BleSensor.h"
#include <iostream>
#include <chrono>
#include <thread>

BleSensor::BleSensor()
{
    const std::string SERVICE_BLUEZ{"org.bluez"};
    const std::string OBJECT_PATH{"/org/bluez/hci0"};

    bluezProxy = sdbus::createProxy(SERVICE_BLUEZ, OBJECT_PATH);
}

void BleSensor::scanAndConnect()
{
    try
    {
        // Enable Bluetooth if not yet enabled
        if (getBluetoothStatus())
        {
            std::cout << "Bluetooth powered ON\n";
        }
        else
        {
            std::cout << "Powering bluetooth ON\n";
            setBluetoothStatus(true);
        }
        enableScanning(true);
        // Wait to be connected to the sensor
        std::this_thread::sleep_for(std::chrono::seconds(10));
        enableScanning(false);
    }
    catch (sdbus::Error &error)
    {
        std::cerr << "ERR: on scanAndConnect(): " << error.getName() << " with message " << error.getMessage() << std::endl;
    }
}

bool BleSensor::getBluetoothStatus()
{
    const std::string METHOD_GET{"Get"};
    sdbus::Variant variant;
    // Invoke a method that gets a property as a variant
    bluezProxy->callMethod(METHOD_GET)
        .onInterface(INTERFACE_PROPERTIES)
        .withArguments(INTERFACE_ADAPTER, PROPERTY_POWERED)
        .storeResultsTo(variant);
    return (bool)variant;
}

void BleSensor::setBluetoothStatus(bool enable)
{
    const std::string METHOD_SET{"Set"};
    // Invoke a method that sets a property as a variant
    bluezProxy->callMethod(METHOD_SET)
        .onInterface(INTERFACE_PROPERTIES)
        .withArguments(INTERFACE_ADAPTER, PROPERTY_POWERED, sdbus::Variant(enable))
        // .dontExpectReply();
        .storeResultsTo();
}

void BleSensor::enableScanning(bool enable)
{
    const std::string METHOD_START_DISCOVERY{"StartDiscovery"};
    const std::string METHOD_STOP_DISCOVERY{"StopDiscovery"};
    std::cout << (enable ? "Start" : "Stop") << " scanning\n";
    bluezProxy->callMethod(enable ? METHOD_START_DISCOVERY : METHOD_STOP_DISCOVERY)
        .onInterface(INTERFACE_ADAPTER)
        .dontExpectReply();
}