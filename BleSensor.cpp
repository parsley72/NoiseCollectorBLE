#include "BleSensor.h"
#include <iostream>

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
        }
    }
    catch (sdbus::Error &error)
    {
        std::cerr << "ERR: on scanAndConnect(): " << error.getName() << " with message " << error.getMessage() << std::endl;
    }
}

bool BleSensor::getBluetoothStatus()
{
    const std::string METHOD_GET{"Get"};
    const std::string INTERFACE_PROPERTIES{"org.freedesktop.DBus.Properties"};
    const std::string INTERFACE_ADAPTER{"org.bluez.Adapter1"};
    const std::string PROPERTY_POWERED{"Powered"};
    sdbus::Variant variant;
    // Invoke a method that gets a property as a variant
    bluezProxy->callMethod(METHOD_GET)
        .onInterface(INTERFACE_PROPERTIES)
        .withArguments(INTERFACE_ADAPTER, PROPERTY_POWERED)
        .storeResultsTo(variant);
    return (bool)variant;
}
