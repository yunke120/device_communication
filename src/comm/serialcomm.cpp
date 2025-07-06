#include "serialcomm.h"
#include <QSerialPortInfo>

SerialComm::SerialComm(const QString& portName, int baudRate, QObject* parent)
    : ICommInterface(parent), m_serialPort(new QSerialPort(this)), m_portName(portName), m_baudRate(baudRate)
{
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(m_baudRate);
}

SerialComm::~SerialComm() {
    close();
}

bool SerialComm::open() {
    if (m_serialPort->isOpen())
        return true;
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(m_baudRate);
    return m_serialPort->open(QIODevice::ReadWrite);
}

void SerialComm::close() {
    if (m_serialPort->isOpen())
        m_serialPort->close();
}

bool SerialComm::isOpen() const {
    return m_serialPort->isOpen();
}

bool SerialComm::write(const QByteArray& data) {
    if (!m_serialPort->isOpen())
        return false;
    return m_serialPort->write(data) == data.size();
}

QByteArray SerialComm::read(qint64 maxSize) {
    if (!m_serialPort->isOpen())
        return QByteArray();
    return m_serialPort->read(maxSize);
}

QString SerialComm::description() const {
    return QString("Serial: %1 @ %2").arg(m_portName).arg(m_baudRate);
} 