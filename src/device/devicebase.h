#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include "../comm/icomminterface.h"

enum class DeviceStatus {
    Disconnected,
    Connecting,
    Connected,
    Error,
    Timeout
};

class DeviceBase : public QObject
{
    Q_OBJECT

public:
    explicit DeviceBase(QObject* parent = nullptr);
    virtual ~DeviceBase();

    // 基本属性
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    DeviceStatus status() const { return m_status; }
    QString statusString() const;
    
    // 配置管理
    void setConfig(const QMap<QString, QVariant>& config);
    QMap<QString, QVariant> getConfig() const { return m_config; }
    virtual QStringList getConfigKeys() const = 0;
    
    // 连接管理
    virtual bool connectDevice() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // 数据收发
    virtual bool sendData(const QByteArray& data) = 0;
    virtual QByteArray receiveData() = 0;
    
    // 状态监控
    void startHeartbeat(int interval = 5000);
    void stopHeartbeat();
    
    // 错误处理
    QString lastError() const { return m_lastError; }

    void setCommInterface(ICommInterface* comm);
    ICommInterface* commInterface() const;

    virtual QString defaultCommType() const = 0;
    virtual QString modelString() const = 0;
    virtual QString deviceName() const = 0;

signals:
    void statusChanged(DeviceStatus status);
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void connectionEstablished();
    void connectionLost();

protected:
    void setStatus(DeviceStatus status);
    void setError(const QString& error);
    virtual void onHeartbeat() = 0;
    
    QString m_name;
    DeviceStatus m_status;
    QMap<QString, QVariant> m_config;
    QString m_lastError;
    QTimer* m_heartbeatTimer;
    QMutex m_mutex;

private slots:
    void heartbeatTimeout();

private:
    ICommInterface* m_comm;
};

#endif // DEVICEBASE_H 
