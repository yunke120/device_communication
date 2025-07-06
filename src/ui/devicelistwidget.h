#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include "../device/devicebase.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QDragEnterEvent>
#include <QDropEvent>

class DeviceListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceListWidget(QWidget* parent = nullptr);
    ~DeviceListWidget() override;

    void addDeviceWidget(DeviceBase* device);
    void updateDeviceStatus(const QString& name, DeviceStatus status);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

signals:
    void deviceDoubleClicked(const QString& name);
    void deviceConfigRequested(const QString& name);

private slots:
    void onDeviceAdded(const QString& name, const QString& deviceType);
    void onDeviceStatusChanged(const QString& name, DeviceStatus status);
    void onDeviceErrorOccurred(const QString& name, const QString& error);
    void onRemoveDeviceRequested(const QString& name);
    void removeDeviceWidget(const QString& name);

private:
    void initUI();
    void createDeviceWidget(DeviceBase* device);
    QWidget* findDeviceWidget(const QString& name);

    QVBoxLayout* m_mainLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_contentWidget;
    QVBoxLayout* m_contentLayout;
    QMap<QString, QWidget*> m_deviceWidgets;
};

#endif // DEVICELISTWIDGET_H 