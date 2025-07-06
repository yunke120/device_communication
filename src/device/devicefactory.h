#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include "devicebase.h"
#include <QObject>
#include <QMap>
#include "../comm/icomminterface.h"

class DeviceFactory : public QObject
{
    Q_OBJECT

public:
    static DeviceFactory* instance();
    
    // 创建设备
    static DeviceBase* createDevice(const QString& deviceType, ICommInterface* comm, QObject* parent = nullptr);
    
    // 获取支持的设备类型
    static QStringList getSupportedDeviceTypes();
    

private:
    explicit DeviceFactory(QObject* parent = nullptr);
    ~DeviceFactory() = default;
    DeviceFactory(const DeviceFactory&) = delete;
    DeviceFactory& operator=(const DeviceFactory&) = delete;
    
    static DeviceFactory* m_instance;
};

#endif // DEVICEFACTORY_H 