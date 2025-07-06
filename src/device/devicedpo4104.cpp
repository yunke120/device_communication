#include "devicedpo4104.h"

DeviceDPO4104::DeviceDPO4104(QObject* parent)
    : DeviceBase(parent)
{
}

DeviceDPO4104::~DeviceDPO4104()
{
}

QStringList DeviceDPO4104::getConfigKeys() const { return {}; }
bool DeviceDPO4104::connectDevice() { return false; }
void DeviceDPO4104::disconnect() {}
bool DeviceDPO4104::isConnected() const { return false; }
bool DeviceDPO4104::sendData(const QByteArray&) { return false; }
QByteArray DeviceDPO4104::receiveData() { return {}; }
void DeviceDPO4104::onHeartbeat() {} 