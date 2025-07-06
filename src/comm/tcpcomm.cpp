#include "tcpcomm.h"

TcpComm::TcpComm(const QString& host, quint16 port, QObject* parent)
    : ICommInterface(parent), m_socket(new QTcpSocket(this)), m_host(host), m_port(port)
{
}

TcpComm::~TcpComm() {
    close();
}

bool TcpComm::open() {
    if (m_socket->isOpen())
        return true;
    m_socket->connectToHost(m_host, m_port);
    return m_socket->waitForConnected(3000);
}

void TcpComm::close() {
    if (m_socket->isOpen())
        m_socket->close();
}

bool TcpComm::isOpen() const {
    return m_socket->isOpen();
}

bool TcpComm::write(const QByteArray& data) {
    if (!m_socket->isOpen())
        return false;
    return m_socket->write(data) == data.size();
}

QByteArray TcpComm::read(qint64 maxSize) {
    if (!m_socket->isOpen())
        return QByteArray();
    if (!m_socket->waitForReadyRead(1000))
        return QByteArray();
    return m_socket->read(maxSize);
}

QString TcpComm::description() const {
    return QString("TCP: %1:%2").arg(m_host).arg(m_port);
} 