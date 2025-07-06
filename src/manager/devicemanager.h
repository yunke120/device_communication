#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "../device/devicebase.h"
#include "../device/devicefactory.h"
#include <QObject>
#include <QList>
#include <QMap>
#include <QMutex>

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    static DeviceManager* instance();
    
    // 设备管理
    bool addDevice(const QString& deviceType, ICommInterface* comm, QObject* parent = nullptr);
    bool removeDevice(const QString& name);
    DeviceBase* getDevice(const QString& name);
    QList<DeviceBase*> getAllDevices() const;
    
    // 连接管理
    bool connectDevice(const QString& name);
    bool disconnectDevice(const QString& name);
    bool connectAllDevices();
    bool disconnectAllDevices();
    
    // 数据收发
    bool sendDataToDevice(const QString& name, const QByteArray& data);
    QByteArray receiveDataFromDevice(const QString& name);
    
    // 状态查询
    bool isDeviceConnected(const QString& name);
    DeviceStatus getDeviceStatus(const QString& name);
    QString getDeviceError(const QString& name);
    
    // 配置管理
    void setDeviceConfig(const QString& name, const QMap<QString, QVariant>& config);
    QMap<QString, QVariant> getDeviceConfig(const QString& name);
    
    // 设备列表
    QStringList getDeviceNames() const;
    int getDeviceCount() const;

signals:
    void deviceAdded(const QString& name, const QString& deviceType);
    void deviceRemoved(const QString& name);
    void deviceStatusChanged(const QString& name, DeviceStatus status);
    void deviceErrorOccurred(const QString& name, const QString& error);
    void deviceDataReceived(const QString& name, const QByteArray& data);

private:
    explicit DeviceManager(QObject* parent = nullptr);
    ~DeviceManager();
    DeviceManager(const DeviceManager&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;
    
    void connectDeviceSignals(DeviceBase* device);
    void disconnectDeviceSignals(DeviceBase* device);
    
    static DeviceManager* m_instance;
    QMap<QString, DeviceBase*> m_devices;
    mutable QMutex m_mutex;
};

#endif // DEVICEMANAGER_H 