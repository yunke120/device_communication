#ifndef UDPCOMM_H
#define UDPCOMM_H

#include "icomminterface.h"
#include <QUdpSocket>

class UdpComm : public ICommInterface {
    Q_OBJECT
public:
    UdpComm(const QString& host, quint16 port, QObject* parent = nullptr);
    ~UdpComm();

    bool open() override;
    void close() override;
    bool isOpen() const override;
    bool write(const QByteArray& data) override;
    QByteArray read(qint64 maxSize = 1024) override;
    QString description() const override;

private:
    QUdpSocket* m_socket;
    QString m_host;
    quint16 m_port;
    bool m_opened;
};

#endif // UDPCOMM_H 