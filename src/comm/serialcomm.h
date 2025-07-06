#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include "icomminterface.h"
#include <QSerialPort>

class SerialComm : public ICommInterface {
    Q_OBJECT
public:
    SerialComm(const QString& portName, int baudRate, QObject* parent = nullptr);
    ~SerialComm();

    bool open() override;
    void close() override;
    bool isOpen() const override;
    bool write(const QByteArray& data) override;
    QByteArray read(qint64 maxSize = 1024) override;
    QString description() const override;

private:
    QSerialPort* m_serialPort;
    QString m_portName;
    int m_baudRate;
};

#endif // SERIALCOMM_H 