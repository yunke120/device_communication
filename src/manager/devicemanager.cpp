#include "devicemanager.h"
#include <QDebug>

DeviceManager* DeviceManager::m_instance = nullptr;

DeviceManager::DeviceManager(QObject* parent)
    : QObject(parent)
{
}

DeviceManager::~DeviceManager()
{
    disconnectAllDevices();
    
    QMutexLocker locker(&m_mutex);
    for (auto device : m_devices) {
        disconnectDeviceSignals(device);
        device->deleteLater();
    }
    m_devices.clear();
}

DeviceManager* DeviceManager::instance()
{
    if (!m_instance) {
        m_instance = new DeviceManager();
    }
    return m_instance;
}

bool DeviceManager::addDevice(const QString& deviceType, ICommInterface* comm, QObject* parent)
{
    // 这里可根据实际需求生成设备名
    QString name = deviceType + QString::number(m_devices.size() + 1);
    DeviceBase* device = DeviceFactory::createDevice(deviceType, comm, parent);
    if (!device) return false;
    device->setName(name); // 设置设备名
    m_devices[name] = device;
    qDebug() << name<<deviceType;
    emit deviceAdded(name, deviceType);
    return true;
}

bool DeviceManager::removeDevice(const QString& name)
{
    DeviceBase* device = nullptr;
    {
        QMutexLocker locker(&m_mutex);
        if (!m_devices.contains(name)) {
            qDebug() << "Device" << name << "not found";
            return false;
        }
        device = m_devices[name];
        disconnectDeviceSignals(device);
        device->disconnect();
        device->deleteLater();
        m_devices.remove(name);
    } // 锁作用域结束，自动释放

    emit deviceRemoved(name); // 信号在锁外发射，避免死锁
    qDebug() << "Device" << name << "removed successfully";
    return true;
}

DeviceBase* DeviceManager::getDevice(const QString& name)
{
    QMutexLocker locker(&m_mutex);
    return m_devices.value(name, nullptr);
}

QList<DeviceBase*> DeviceManager::getAllDevices() const
{
    QMutexLocker locker(&m_mutex);
    return m_devices.values();
}

bool DeviceManager::connectDevice(const QString& name)
{
    DeviceBase* device = getDevice(name);
    if (!device) {
        qDebug() << "Device" << name << "not found";
        return false;
    }
    emit deviceStatusChanged(name, DeviceStatus::Connecting);
    return device->connectDevice();
}

bool DeviceManager::disconnectDevice(const QString& name)
{
    DeviceBase* device = getDevice(name);
    if (!device) {
        qDebug() << "Device" << name << "not found";
        return false;
    }
    
    device->disconnect();
    return true;
}

bool DeviceManager::connectAllDevices()
{
    bool allConnected = true;
    QMutexLocker locker(&m_mutex);
    
    for (auto device : m_devices) {
        if (!device->connectDevice()) {
            allConnected = false;
        }
    }
    
    return allConnected;
}

bool DeviceManager::disconnectAllDevices()
{
    QMutexLocker locker(&m_mutex);
    
    for (auto device : m_devices) {
        device->disconnect();
    }
    
    return true;
}

bool DeviceManager::sendDataToDevice(const QString& name, const QByteArray& data)
{
    DeviceBase* device = getDevice(name);
    if (!device) {
        qDebug() << "Device" << name << "not found";
        return false;
    }
    
    return device->sendData(data);
}

QByteArray DeviceManager::receiveDataFromDevice(const QString& name)
{
    DeviceBase* device = getDevice(name);
    if (!device) {
        qDebug() << "Device" << name << "not found";
        return QByteArray();
    }
    
    return device->receiveData();
}

bool DeviceManager::isDeviceConnected(const QString& name)
{
    DeviceBase* device = getDevice(name);
    return device ? device->isConnected() : false;
}

DeviceStatus DeviceManager::getDeviceStatus(const QString& name)
{
    DeviceBase* device = getDevice(name);
    return device ? device->status() : DeviceStatus::Disconnected;
}

QString DeviceManager::getDeviceError(const QString& name)
{
    DeviceBase* device = getDevice(name);
    return device ? device->lastError() : QString();
}

void DeviceManager::setDeviceConfig(const QString& name, const QMap<QString, QVariant>& config)
{
    DeviceBase* device = getDevice(name);
    if (device) {
        device->setConfig(config);
    }
}

QMap<QString, QVariant> DeviceManager::getDeviceConfig(const QString& name)
{
    DeviceBase* device = getDevice(name);
    return device ? device->getConfig() : QMap<QString, QVariant>();
}

QStringList DeviceManager::getDeviceNames() const
{
    QMutexLocker locker(&m_mutex);
    return m_devices.keys();
}

int DeviceManager::getDeviceCount() const
{
    QMutexLocker locker(&m_mutex);
    return m_devices.size();
}

void DeviceManager::connectDeviceSignals(DeviceBase* device)
{
    if (!device) return;
    
    connect(device, &DeviceBase::statusChanged, this, [this, device](DeviceStatus status) {
        emit deviceStatusChanged(device->modelString(), status);
    });
    
    connect(device, &DeviceBase::errorOccurred, this, [this, device](const QString& error) {
        emit deviceErrorOccurred(device->modelString(), error);
    });
    
    connect(device, &DeviceBase::dataReceived, this, [this, device](const QByteArray& data) {
        emit deviceDataReceived(device->modelString(), data);
    });
}

void DeviceManager::disconnectDeviceSignals(DeviceBase* device)
{
    if (!device) return;
    
    disconnect(device, nullptr, this, nullptr);
} 
