#include "udpcomm.h"

UdpComm::UdpComm(const QString& host, quint16 port, QObject* parent)
    : ICommInterface(parent), m_socket(new QUdpSocket(this)), m_host(host), m_port(port), m_opened(false)
{
}

UdpComm::~UdpComm() {
    close();
}

bool UdpComm::open() {
    if (m_opened)
        return true;
    m_opened = m_socket->bind(QHostAddress::Any, 0);
    return m_opened;
}

void UdpComm::close() {
    if (m_opened) {
        m_socket->close();
        m_opened = false;
    }
}

bool UdpComm::isOpen() const {
    return m_opened;
}

bool UdpComm::write(const QByteArray& data) {
    if (!m_opened)
        return false;
    return m_socket->writeDatagram(data, QHostAddress(m_host), m_port) == data.size();
}

QByteArray UdpComm::read(qint64 maxSize) {
    if (!m_opened)
        return QByteArray();
    if (!m_socket->hasPendingDatagrams())
        return QByteArray();
    QByteArray datagram;
    datagram.resize(static_cast<int>(qMin(maxSize, m_socket->pendingDatagramSize())));
    m_socket->readDatagram(datagram.data(), datagram.size());
    return datagram;
}

QString UdpComm::description() const {
    return QString("UDP: %1:%2").arg(m_host).arg(m_port);
} 