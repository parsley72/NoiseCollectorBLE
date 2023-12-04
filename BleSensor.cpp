#include "BleSensor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <regex>

BleSensor::BleSensor(const std::string &sensor_name)
    : deviceProxy{nullptr}, deviceName{sensor_name},
      cv{}, mtx{}, connected{false}
{
    const std::string OBJECT_PATH{"/org/bluez/hci0"};

    bluezProxy = sdbus::createProxy(SERVICE_BLUEZ, OBJECT_PATH);
    rootProxy = sdbus::createProxy(SERVICE_BLUEZ, "/");
}

void BleSensor::scanAndConnect()
{
    std::cout << "(TID: " << std::this_thread::get_id() << ") ";
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
        std::unique_lock<std::mutex> lock(mtx);
        subscribeToInterfacesAdded();
        enableScanning(true);
        // Wait to be connected to the sensor
        cv.wait(lock, [this]()
                { return connected; });
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

void BleSensor::subscribeToInterfacesAdded()
{
    const std::string INTERFACE_OBJ_MGR{"org.freedesktop.DBus.ObjectManager"};
    const std::string MEMBER_IFACE_ADDED{"InterfacesAdded"};
    auto interfaceAddedCallback = [this](sdbus::ObjectPath path,
                                         std::map<std::string,
                                                  std::map<std::string, sdbus::Variant>>
                                             dictionary)
    {
        const std::regex DEVICE_INSTANCE_RE{"^/org/bluez/hci[0-9]/dev(_[0-9A-F]{2}){6}$"};
        std::smatch match;
        std::cout << "(TID: " << std::this_thread::get_id() << ") ";
        if (std::regex_match(path, match, DEVICE_INSTANCE_RE))
        {
            std::cout << "Device iface ";

            if (dictionary["org.bluez.Device1"].count("Name") == 1)
            {
                auto name = (std::string)(dictionary["org.bluez.Device1"].at("Name"));
                std::cout << name << " @ " << path << std::endl;
                if (name == deviceName)
                {
                    std::cout << "Connecting to " << name << std::endl;
                    connectToDevice(path);
                }
            }
            else
            {
                std::cout << "<NAMELESS> @ " << path << std::endl;
            }
        }
        else
        {
            std::cout << "*** UNEXPECTED SIGNAL ***";
        }
    };
    // Let's subscribe for the interfaces added signals (AddMatch)
    rootProxy->uponSignal(MEMBER_IFACE_ADDED).onInterface(INTERFACE_OBJ_MGR).call(interfaceAddedCallback);
    rootProxy->finishRegistration();
}

void BleSensor::connectToDevice(sdbus::ObjectPath path)
{
    const std::string INTERFACE_DEVICE{"org.bluez.Device1"};
    const std::string METHOD_CONNECT{"Connect"};
    auto connectionCallback = [this](const sdbus::Error *error)
    {
        if (error != nullptr)
        {
            std::cerr << "Got connection error "
                      << error->getName() << " with message "
                      << error->getMessage() << std::endl;
            return;
        }
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Connected!!!" << std::endl;
        connected = true;
        lock.unlock();
        cv.notify_one();
        std::cout << "Finished connection method call" << std::endl;
    };
    deviceProxy = sdbus::createProxy(SERVICE_BLUEZ, path);
    deviceProxy->callMethodAsync(METHOD_CONNECT).onInterface(INTERFACE_DEVICE).uponReplyInvoke(connectionCallback);
    std::cout << "Connection method started" << std::endl;
}
