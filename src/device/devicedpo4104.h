#ifndef DEVICEDPO4104_H
#define DEVICEDPO4104_H

#include "devicebase.h"

class DeviceDPO4104 : public DeviceBase {
    Q_OBJECT
public:
    explicit DeviceDPO4104(QObject* parent = nullptr);
    ~DeviceDPO4104();

    // 可扩展：添加DPO4104特有接口
    QStringList getConfigKeys() const override;
    bool connectDevice() override;
    void disconnect() override;
    bool isConnected() const override;
    bool sendData(const QByteArray& data) override;
    QByteArray receiveData() override;
    void onHeartbeat() override;
    QString defaultCommType() const override { return "TCP"; }
    QString deviceName() const override { return "示波器"; }
    QString modelString() const override {return "DPO4104";}
};

#endif // DEVICEDPO4104_H 
