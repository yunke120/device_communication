#include "devicewidget.h"
#include "../device/devicefactory.h"
#include <QApplication>
#include <QDebug>

DeviceWidget::DeviceWidget(DeviceBase* device, QWidget* parent)
    : QFrame(parent)
    , m_device(device)
    , m_contextMenu(nullptr)
{
    initUI();
    createContextMenu();
}

DeviceWidget::~DeviceWidget()
{
}

void DeviceWidget::initUI()
{
    setFixedHeight(80);
    setFrameStyle(QFrame::Box);
    setLineWidth(1);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(10);
    
    // 设备信息区域
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(2);

    // 设备型号（左上角）
    QString modelText = m_device->deviceName();
    m_modelLabel = new QLabel(modelText, this);
    m_modelLabel->setStyleSheet("QLabel { color: #2196F3; font-weight: bold; font-size: 11px; }");
    infoLayout->addWidget(m_modelLabel, 0, Qt::AlignLeft);

    // 设备名称
    // m_nameLabel = new QLabel(m_device->modelString(), this);
    // m_nameLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 12px; }");
    // infoLayout->addWidget(m_nameLabel);
    
    // 设备类型
    m_typeLabel = new QLabel(m_device->modelString(), this);
    m_typeLabel->setStyleSheet("QLabel { color: #666666; font-size: 10px; }");
    infoLayout->addWidget(m_typeLabel);
    
    // 设备状态
    m_statusLabel = new QLabel(m_device->statusString(), this);
    m_statusLabel->setStyleSheet("QLabel { color: #666666; font-size: 10px; }");
    infoLayout->addWidget(m_statusLabel);
    
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
    
    // 按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(5);
    
    // 连接/断开按钮
    m_connectButton = new QPushButton(this);
    m_connectButton->setFixedSize(60, 25);
    m_connectButton->setText(tr("连接"));
    m_connectButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 3px;"
        "    font-size: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
    );
    connect(m_connectButton, &QPushButton::clicked, this, &DeviceWidget::onConnectClicked);
    buttonLayout->addWidget(m_connectButton);
    
    // 配置按钮
    m_configButton = new QPushButton(this);
    m_configButton->setFixedSize(60, 25);
    m_configButton->setText(tr("配置"));
    m_configButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 3px;"
        "    font-size: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"
    );
    connect(m_configButton, &QPushButton::clicked, this, &DeviceWidget::onConfigClicked);
    buttonLayout->addWidget(m_configButton);
    
    // 删除按钮
    m_removeButton = new QPushButton(this);
    m_removeButton->setFixedSize(60, 25);
    m_removeButton->setText(tr("删除"));
    m_removeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #f44336;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 3px;"
        "    font-size: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #d32f2f;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #c62828;"
        "}"
    );
    connect(m_removeButton, &QPushButton::clicked, this, &DeviceWidget::onRemoveClicked);
    buttonLayout->addWidget(m_removeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // 设置整体样式
    setStyleSheet(
        "DeviceWidget {"
        "    background-color: white;"
        "    border: 1px solid #cccccc;"
        "    border-radius: 5px;"
        "}"
        "DeviceWidget:hover {"
        "    border-color: #4CAF50;"
        "    background-color: #f9f9f9;"
        "}"
    );
    
    updateStatusDisplay();
}

void DeviceWidget::updateStatus(DeviceStatus status)
{
    Q_UNUSED(status)
    updateStatusDisplay();
}

void DeviceWidget::showError(const QString& error)
{
    m_lastError = error;
    m_statusLabel->setText(tr("错误: %1").arg(error));
    m_statusLabel->setStyleSheet("QLabel { color: #f44336; font-size: 10px; }");
}

void DeviceWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    emit doubleClicked(m_device->name());
    emit configRequested(m_device->name());
}

void DeviceWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (m_contextMenu) {
        m_contextMenu->exec(event->globalPos());
    }
}

void DeviceWidget::onConnectClicked()
{
    if (m_device->isConnected()) {
        emit disconnectRequested(m_device->name());
    } else {
        emit connectRequested(m_device->name());
    }
}

void DeviceWidget::onConfigClicked()
{
    emit configRequested(m_device->name());
}

void DeviceWidget::onRemoveClicked()
{
    emit removeRequested(m_device->name());
}

void DeviceWidget::updateStatusDisplay()
{
    DeviceStatus status = m_device->status();
    QString statusText = m_device->statusString();
    
    m_statusLabel->setText(statusText);
    
    // 根据状态更新按钮和样式
    switch (status) {
    case DeviceStatus::Connected:
        m_connectButton->setText(tr("断开"));
        m_connectButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #f44336;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 3px;"
            "    font-size: 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #d32f2f;"
            "}"
        );
        m_statusLabel->setStyleSheet("QLabel { color: #4CAF50; font-size: 10px; }");
        break;
        
    case DeviceStatus::Connecting:
        m_connectButton->setText(tr("连接中..."));
        m_connectButton->setEnabled(false);
        m_statusLabel->setStyleSheet("QLabel { color: #FF9800; font-size: 10px; }");
        break;
        
    case DeviceStatus::Error:
    case DeviceStatus::Timeout:
        m_connectButton->setText(tr("连接"));
        m_connectButton->setEnabled(true);
        m_statusLabel->setStyleSheet("QLabel { color: #f44336; font-size: 10px; }");
        break;
        
    default: // Disconnected
        m_connectButton->setText(tr("连接"));
        m_connectButton->setEnabled(true);
        m_connectButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #4CAF50;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 3px;"
            "    font-size: 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #45a049;"
            "}"
        );
        m_statusLabel->setStyleSheet("QLabel { color: #666666; font-size: 10px; }");
        break;
    }
}

void DeviceWidget::createContextMenu()
{
    m_contextMenu = new QMenu(this);
    
    QAction* connectAction = new QAction(tr("连接"), this);
    connect(connectAction, &QAction::triggered, this, &DeviceWidget::onConnectClicked);
    m_contextMenu->addAction(connectAction);
    
    QAction* configAction = new QAction(tr("配置"), this);
    connect(configAction, &QAction::triggered, this, &DeviceWidget::onConfigClicked);
    m_contextMenu->addAction(configAction);
    
    m_contextMenu->addSeparator();
    
    QAction* removeAction = new QAction(tr("删除"), this);
    connect(removeAction, &QAction::triggered, this, &DeviceWidget::onRemoveClicked);
    m_contextMenu->addAction(removeAction);
} 
