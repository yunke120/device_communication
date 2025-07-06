#include "devicen5750a.h"

DeviceN5750A::DeviceN5750A(QObject* parent)
    : DeviceBase(parent)
{
}

DeviceN5750A::~DeviceN5750A()
{
}

QStringList DeviceN5750A::getConfigKeys() const { return {}; }
bool DeviceN5750A::connectDevice() { return false; }
void DeviceN5750A::disconnect() {}
bool DeviceN5750A::isConnected() const { return false; }
bool DeviceN5750A::sendData(const QByteArray&) { return false; }
QByteArray DeviceN5750A::receiveData() { return {}; }
void DeviceN5750A::onHeartbeat() {} 