#include "devicebase.h"
#include <QDebug>

DeviceBase::DeviceBase(QObject* parent)
    : QObject(parent), m_comm(nullptr)
{
}

DeviceBase::~DeviceBase()
{
    if (m_comm) {
        delete m_comm;
        m_comm = nullptr;
    }
}

QString DeviceBase::statusString() const
{
    switch (m_status) {
    case DeviceStatus::Disconnected:
        return tr("断开连接");
    case DeviceStatus::Connecting:
        return tr("连接中...");
    case DeviceStatus::Connected:
        return tr("已连接");
    case DeviceStatus::Error:
        return tr("错误");
    case DeviceStatus::Timeout:
        return tr("超时");
    default:
        return tr("未知");
    }
}

void DeviceBase::setConfig(const QMap<QString, QVariant>& config)
{
    QMutexLocker locker(&m_mutex);
    m_config = config;
}

void DeviceBase::setStatus(DeviceStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
        
        if (status == DeviceStatus::Connected) {
            emit connectionEstablished();
        } else if (status == DeviceStatus::Disconnected || 
                   status == DeviceStatus::Error || 
                   status == DeviceStatus::Timeout) {
            emit connectionLost();
        }
    }
}

void DeviceBase::setError(const QString& error)
{
    m_lastError = error;
    setStatus(DeviceStatus::Error);
    emit errorOccurred(error);
}

void DeviceBase::startHeartbeat(int interval)
{
    if (m_heartbeatTimer) {
        m_heartbeatTimer->start(interval);
    }
}

void DeviceBase::stopHeartbeat()
{
    if (m_heartbeatTimer) {
        m_heartbeatTimer->stop();
    }
}

void DeviceBase::heartbeatTimeout()
{
    onHeartbeat();
} 

void DeviceBase::setCommInterface(ICommInterface* comm)
{
    if (m_comm) {
        delete m_comm;
    }
    m_comm = comm;
}

ICommInterface* DeviceBase::commInterface() const
{
    return m_comm;
} 