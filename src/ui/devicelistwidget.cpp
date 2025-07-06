#include "devicelistwidget.h"
#include "devicewidget.h"
#include "../manager/devicemanager.h"
#include <QApplication>
#include <QDebug>
#include <QMimeData>
#include "deviceconfigdialog.h"
#include "../comm/serialcomm.h"
#include "../comm/tcpcomm.h"
#include "../comm/udpcomm.h"

DeviceListWidget::DeviceListWidget(QWidget* parent)
    : QWidget(parent)
{
    initUI();
    
    // 连接设备管理器信号
    DeviceManager* manager = DeviceManager::instance();
    connect(manager, &DeviceManager::deviceAdded, this, &DeviceListWidget::onDeviceAdded);
    connect(manager, &DeviceManager::deviceRemoved, this, &DeviceListWidget::removeDeviceWidget);
    connect(manager, &DeviceManager::deviceStatusChanged, this, &DeviceListWidget::onDeviceStatusChanged);
    connect(manager, &DeviceManager::deviceErrorOccurred, this, &DeviceListWidget::onDeviceErrorOccurred);
}

DeviceListWidget::~DeviceListWidget()
{
}

void DeviceListWidget::initUI()
{
    setAcceptDrops(true);
    
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    
    // 创建标题
    QLabel* titleLabel = new QLabel(tr("设备列表"), this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; padding: 5px; }");
    m_mainLayout->addWidget(titleLabel);
    
    // 创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    m_contentWidget = new QWidget(m_scrollArea);
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(5, 5, 5, 5);
    m_contentLayout->setSpacing(5);
    m_contentLayout->addStretch();
    
    m_scrollArea->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_scrollArea);
    
    // 设置样式
    setStyleSheet(
        "DeviceListWidget {"
        "    background-color: #f5f5f5;"
        "    border: 1px solid #cccccc;"
        "}"
    );
}

void DeviceListWidget::addDeviceWidget(DeviceBase* device)
{
    if (!device) return;
    
    createDeviceWidget(device);
}

void DeviceListWidget::removeDeviceWidget(const QString& name)
{
    QWidget* widget = findDeviceWidget(name);
    if (widget) {
        m_contentLayout->removeWidget(widget);
        widget->deleteLater();
        m_deviceWidgets.remove(name);
    }
}

void DeviceListWidget::updateDeviceStatus(const QString& name, DeviceStatus status)
{
    QWidget* widget = findDeviceWidget(name);
    if (widget) {
        DeviceWidget* deviceWidget = qobject_cast<DeviceWidget*>(widget);
        if (deviceWidget) {
            deviceWidget->updateStatus(status);
        }
    }
}

void DeviceListWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-device-type")) {
        event->acceptProposedAction();
    }
}

void DeviceListWidget::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-device-type")) {
        event->acceptProposedAction();
    }
}

void DeviceListWidget::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    
    if (mimeData->hasFormat("application/x-device-type")) {
        QString deviceType = QString::fromUtf8(mimeData->data("application/x-device-type"));
        QString baseName = QString::fromUtf8(mimeData->data("application/x-device-name"));

        // 自动生成唯一设备名
        DeviceManager* manager = DeviceManager::instance();
        QString name = baseName;
        int index = 1;
        while (manager->getDevice(name)) {
            name = baseName + QString::number(index++);
        }

        // 只添加设备，不弹窗、不创建通信接口
        if (manager->addDevice(deviceType, nullptr, this)) {
            event->acceptProposedAction();
        }
    }
}

void DeviceListWidget::onDeviceStatusChanged(const QString& name, DeviceStatus status)
{
    DeviceManager* manager = DeviceManager::instance();
    DeviceBase* device = manager->getDevice(name);
    
    if (device && !m_deviceWidgets.contains(name)) {
        createDeviceWidget(device);
    }
    
    updateDeviceStatus(name, status);
}

void DeviceListWidget::onDeviceErrorOccurred(const QString& name, const QString& error)
{
    QWidget* widget = findDeviceWidget(name);
    if (widget) {
        DeviceWidget* deviceWidget = qobject_cast<DeviceWidget*>(widget);
        if (deviceWidget) {
            deviceWidget->showError(error);
        }
    }
}

void DeviceListWidget::onDeviceAdded(const QString& name, const QString& deviceType)
{
    Q_UNUSED(deviceType)
    DeviceManager* manager = DeviceManager::instance();
    createDeviceWidget(manager->getDevice(name));
}

void DeviceListWidget::onRemoveDeviceRequested(const QString& name)
{
    DeviceManager::instance()->removeDevice(name);
}

void DeviceListWidget::createDeviceWidget(DeviceBase* device)
{
    if (!device) return;
    
    // 移除占位符
    QLayoutItem* stretchItem = m_contentLayout->itemAt(m_contentLayout->count() - 1);
    if (stretchItem && stretchItem->spacerItem()) {
        m_contentLayout->removeItem(stretchItem);
    }
    
    // 创建设备组件
    DeviceWidget* deviceWidget = new DeviceWidget(device, m_contentWidget);
    m_contentLayout->addWidget(deviceWidget);
    m_deviceWidgets[device->name()] = deviceWidget;
    
    // 连接信号
    connect(deviceWidget, &DeviceWidget::doubleClicked, this, &DeviceListWidget::deviceDoubleClicked);
    connect(deviceWidget, &DeviceWidget::configRequested, this, [this, device](const QString&) {
        DeviceConfigDialog dlg(device, this);
        if (dlg.exec() == QDialog::Accepted) {
            QMap<QString, QVariant> config = dlg.getConfig();
            ICommInterface* comm = nullptr;
            QString commType = device->defaultCommType();
            if (commType == "Serial") {
                comm = new SerialComm(config["port"].toString(), config["baudRate"].toInt());
            } else if (commType == "TCP") {
                comm = new TcpComm(config["host"].toString(), config["port"].toInt());
            } else if (commType == "UDP") {
                comm = new UdpComm(config["remoteHost"].toString(), config["remotePort"].toInt());
            }
            if (comm) {
                device->setCommInterface(comm);
            }
        }
    });
    connect(deviceWidget, &DeviceWidget::removeRequested, this, &DeviceListWidget::onRemoveDeviceRequested);
    connect(deviceWidget, &DeviceWidget::connectRequested, this, [device](const QString& name) {
        bool ret = DeviceManager::instance()->connectDevice(name);
        // qDebug() << "connectDevice" << name << ret;
        if (ret) {
            emit DeviceManager::instance()->deviceStatusChanged(name, DeviceStatus::Connected);
        } else {    
            emit DeviceManager::instance()->deviceErrorOccurred(name, DeviceManager::instance()->getDeviceError(name));
        }
    });
    connect(deviceWidget, &DeviceWidget::disconnectRequested, this, [device](const QString& name) {
        bool ret = DeviceManager::instance()->disconnectDevice(name);
        if (ret) {
            emit DeviceManager::instance()->deviceStatusChanged(name, DeviceStatus::Disconnected);
        } else {
            emit DeviceManager::instance()->deviceErrorOccurred(name, DeviceManager::instance()->getDeviceError(name));
        }
    });
    // 重新添加占位符
    m_contentLayout->addStretch();
}

QWidget* DeviceListWidget::findDeviceWidget(const QString& name)
{
    return m_deviceWidgets.value(name, nullptr);
} 
