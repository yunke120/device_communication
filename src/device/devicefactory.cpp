#include "devicefactory.h"
#include <QDebug>
#include "devicebase.h"
#include "../comm/icomminterface.h"
#include "device8808a.h"
#include "devicedpo4104.h"
#include "devicen5750a.h"

DeviceFactory* DeviceFactory::m_instance = nullptr;

DeviceFactory::DeviceFactory(QObject* parent)
    : QObject(parent)
{
}

DeviceFactory* DeviceFactory::instance()
{
    if (!m_instance) {
        m_instance = new DeviceFactory();
    }
    return m_instance;
}

DeviceBase* DeviceFactory::createDevice(const QString& deviceType, ICommInterface* comm, QObject* parent)
{
    DeviceBase* device = nullptr;
    if (deviceType == "8808A") {
        device = new Device8808A(parent);
    } else if (deviceType == "DPO4104") {
        device = new DeviceDPO4104(parent);
    } else if (deviceType == "N5750A") {
        device = new DeviceN5750A(parent);
    } else {
        return nullptr;
    }
    device->setCommInterface(comm);
    return device;
}

QStringList DeviceFactory::getSupportedDeviceTypes()
{
    return {"8808A", "DPO4104", "N5750A"};
} 