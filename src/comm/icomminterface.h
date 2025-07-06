#ifndef ICOMMINTERFACE_H
#define ICOMMINTERFACE_H

#include <QObject>
#include <QByteArray>
#include <QString>

class ICommInterface : public QObject {
    Q_OBJECT
public:
    explicit ICommInterface(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ICommInterface() {}

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual bool write(const QByteArray& data) = 0;
    virtual QByteArray read(qint64 maxSize = 1024) = 0;
    virtual QString description() const = 0;
};

#endif // ICOMMINTERFACE_H 