#ifndef DEVICE8808A_H
#define DEVICE8808A_H

#include "devicebase.h"

class Device8808A : public DeviceBase {
    Q_OBJECT
public:
    explicit Device8808A(QObject* parent = nullptr);
    ~Device8808A();

    // 可扩展：添加8808A特有接口
    QStringList getConfigKeys() const override;
    bool connectDevice() override;
    void disconnect() override;
    bool isConnected() const override;
    bool sendData(const QByteArray& data) override;
    QByteArray receiveData() override;
    void onHeartbeat() override;
    QString defaultCommType() const override { return "Serial"; }
    QString deviceName() const override { return "万用表"; }
    QString modelString() const override {return "Fluke 8808A";}
};

#endif // DEVICE8808A_H 
