#ifndef TCPCOMM_H
#define TCPCOMM_H

#include "icomminterface.h"
#include <QTcpSocket>

class TcpComm : public ICommInterface {
    Q_OBJECT
public:
    TcpComm(const QString& host, quint16 port, QObject* parent = nullptr);
    ~TcpComm();

    bool open() override;
    void close() override;
    bool isOpen() const override;
    bool write(const QByteArray& data) override;
    QByteArray read(qint64 maxSize = 1024) override;
    QString description() const override;

private:
    QTcpSocket* m_socket;
    QString m_host;
    quint16 m_port;
};

#endif // TCPCOMM_H 