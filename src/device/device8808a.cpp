#include "device8808a.h"

Device8808A::Device8808A(QObject* parent)
    : DeviceBase(parent)
{
}

Device8808A::~Device8808A()
{
}

QStringList Device8808A::getConfigKeys() const { return {}; }
bool Device8808A::connectDevice() {
    qDebug() << "connectDevice";
    // 实际连接逻辑...
    setStatus(DeviceStatus::Connected);
    return true;
}
void Device8808A::disconnect() {
    // 实际断开逻辑...
    setStatus(DeviceStatus::Disconnected);
}
bool Device8808A::isConnected() const {
    return status() == DeviceStatus::Connected;
}
bool Device8808A::sendData(const QByteArray&) { return false; }
QByteArray Device8808A::receiveData() { return {}; }
void Device8808A::onHeartbeat() {} 
