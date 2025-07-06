#ifndef DEVICEN5750A_H
#define DEVICEN5750A_H

#include "devicebase.h"

class DeviceN5750A : public DeviceBase {
    Q_OBJECT
public:
    explicit DeviceN5750A(QObject* parent = nullptr);
    ~DeviceN5750A();

    // 可扩展：添加N5750A特有接口
    QStringList getConfigKeys() const override;
    bool connectDevice() override;
    void disconnect() override;
    bool isConnected() const override;
    bool sendData(const QByteArray& data) override;
    QByteArray receiveData() override;
    void onHeartbeat() override;
    QString defaultCommType() const override { return "TCP"; }
    QString deviceName() const override { return "高压源"; }
    QString modelString() const override {return "N5750";}
};

#endif // DEVICEN5750A_H 
