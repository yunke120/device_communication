#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "device/devicefactory.h"
#include <QApplication>
#include <QMessageBox>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainSplitter(nullptr)
    , m_devicePalette(nullptr)
    , m_deviceList(nullptr)
    , m_logWidget(nullptr)
    , m_statusLabel(nullptr)
    , m_deviceCountLabel(nullptr)
    , m_deviceManager(nullptr)
{
    ui->setupUi(this);
    m_deviceManager = DeviceManager::instance();
    
    createActions();
    createStatusBar();
    createCentralWidget();
    setupConnections();
    
    setWindowTitle(tr("设备通信管理器"));
    setMinimumSize(800, 600);
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    // 连接UI中的动作
    connect(ui->action_connect_all, &QAction::triggered, this, &MainWindow::onConnectAllDevices);
    connect(ui->action_disconnect_all, &QAction::triggered, this, &MainWindow::onDisconnectAllDevices);
    connect(ui->action_clear_log, &QAction::triggered, this, &MainWindow::onClearLog);
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::onAbout);
}



void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel(tr("就绪"), this);
    ui->statusbar->addWidget(m_statusLabel);
    
    m_deviceCountLabel = new QLabel(tr("设备数量: 0"), this);
    ui->statusbar->addPermanentWidget(m_deviceCountLabel);
}

void MainWindow::createCentralWidget()
{
    // 确保 centralwidget 有布局
    if (!ui->centralwidget->layout()) {
        QHBoxLayout* layout = new QHBoxLayout(ui->centralwidget);
        layout->setContentsMargins(0, 0, 0, 0);
        ui->centralwidget->setLayout(layout);
    }

    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    ui->centralwidget->layout()->addWidget(m_mainSplitter);
    
    // 创建设备面板
    createDevicePalette();
    
    // 创建设备列表
    m_deviceList = new DeviceListWidget(m_mainSplitter);
    m_mainSplitter->addWidget(m_deviceList);
    
    // 创建日志窗口
    m_logWidget = new QTextEdit(m_mainSplitter);
    m_logWidget->setReadOnly(true);
    m_logWidget->setMaximumWidth(300);
    m_logWidget->setStyleSheet(
        "QTextEdit {"
        "    background-color: #f8f8f8;"
        "    border: 1px solid #cccccc;"
        "    font-family: 'Consolas', 'Monaco', monospace;"
        "    font-size: 10px;"
        "}"
    );
    m_mainSplitter->addWidget(m_logWidget);
    
    // 设置分割器比例
    m_mainSplitter->setSizes({200, 500, 300});
}

void MainWindow::createDevicePalette()
{
    m_devicePalette = new QWidget(m_mainSplitter);
    m_devicePalette->setMaximumWidth(200);
    m_devicePalette->setMinimumWidth(150);
    
    QVBoxLayout* layout = new QVBoxLayout(m_devicePalette);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    
    // 标题
    QLabel* titleLabel = new QLabel(tr("设备类型"), m_devicePalette);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 12px; }");
    layout->addWidget(titleLabel);
    
    // 创建设备类型拖拽组件
    QStringList deviceTypes = DeviceFactory::getSupportedDeviceTypes();
    QMap<QString, QString> deviceTypeToModel = {
        {"8808A", tr("万用表")},
        {"N5750A", tr("高压源")},
        {"DPO4104", tr("示波器")}
    };
    for (const QString& typeName : deviceTypes) {
        QString deviceType = typeName;
        QString modelName = deviceTypeToModel.value(typeName, typeName);
        QString displayName = QString("%1\n(%2)").arg(typeName).arg(modelName);
        DeviceDragWidget* dragWidget = new DeviceDragWidget(deviceType, displayName, m_devicePalette);
        layout->addWidget(dragWidget);
    }
    
    layout->addStretch();
    
    // 设置样式
    m_devicePalette->setStyleSheet(
        "QWidget {"
        "    background-color: #f0f0f0;"
        "    border: 1px solid #cccccc;"
        "}"
    );
    
    m_mainSplitter->addWidget(m_devicePalette);
}

void MainWindow::setupConnections()
{
    // 连接设备列表信号
    connect(m_deviceList, &DeviceListWidget::deviceDoubleClicked, this, &MainWindow::onDeviceDoubleClicked);
    connect(m_deviceList, &DeviceListWidget::deviceConfigRequested, this, &MainWindow::onDeviceConfigRequested);
    
    // 连接设备管理器信号
    connect(m_deviceManager, &DeviceManager::deviceAdded, this, [this](const QString& name, const QString& type) {
        logMessage(tr("设备已添加: %1 (%2)").arg(name).arg(type));
        updateStatusBar();
    });
    
    connect(m_deviceManager, &DeviceManager::deviceRemoved, this, [this](const QString& name) {
        logMessage(tr("设备已移除: %1").arg(name));
        updateStatusBar();
    });
    
    connect(m_deviceManager, &DeviceManager::deviceStatusChanged, this, [this](const QString& name, DeviceStatus status) {
        QString statusStr;
        switch (status) {
        case DeviceStatus::Disconnected:
            statusStr = tr("断开连接");
            break;
        case DeviceStatus::Connecting:
            statusStr = tr("连接中");
            break;
        case DeviceStatus::Connected:
            statusStr = tr("已连接");
            break;
        case DeviceStatus::Error:
            statusStr = tr("错误");
            break;
        case DeviceStatus::Timeout:
            statusStr = tr("超时");
            break;
        default:
            statusStr = tr("未知");
            break;
        }
        logMessage(tr("设备状态变化: %1 -> %2").arg(name).arg(statusStr));
        updateStatusBar();
    });
    
    connect(m_deviceManager, &DeviceManager::deviceDataReceived, this, &MainWindow::onDeviceDataReceived);
    connect(m_deviceManager, &DeviceManager::deviceErrorOccurred, this, &MainWindow::onDeviceErrorOccurred);
}

void MainWindow::onDeviceDoubleClicked(const QString& name)
{
    onDeviceConfigRequested(name);
}

void MainWindow::onDeviceConfigRequested(const QString& name)
{
    DeviceBase* device = m_deviceManager->getDevice(name);
    if (!device) {
        QMessageBox::warning(this, tr("错误"), tr("设备不存在: %1").arg(name));
        return;
    }
    
    DeviceConfigDialog dialog(device, this);
    if (dialog.exec() == QDialog::Accepted) {
        logMessage(tr("设备配置已更新: %1").arg(name));
    }
}

void MainWindow::onDeviceConnectRequested(const QString& name)
{
    if (m_deviceManager->connectDevice(name)) {
        logMessage(tr("设备连接成功: %1").arg(name));
    } else {
        QString error = m_deviceManager->getDeviceError(name);
        logMessage(tr("设备连接失败: %1 - %2").arg(name).arg(error));
    }
}

void MainWindow::onDeviceDisconnectRequested(const QString& name)
{
    m_deviceManager->disconnectDevice(name);
    logMessage(tr("设备已断开: %1").arg(name));
}

void MainWindow::onDeviceRemoveRequested(const QString& name)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("确认删除"), 
        tr("确定要删除设备 '%1' 吗？").arg(name),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        m_deviceManager->removeDevice(name);
    }
}

void MainWindow::onDeviceDataReceived(const QString& name, const QByteArray& data)
{
    QString hexData = data.toHex(' ').toUpper();
    logMessage(tr("收到数据 [%1]: %2").arg(name).arg(hexData));
}

void MainWindow::onDeviceErrorOccurred(const QString& name, const QString& error)
{
    logMessage(tr("设备错误 [%1]: %2").arg(name).arg(error));
}

void MainWindow::onConnectAllDevices()
{
    if (m_deviceManager->connectAllDevices()) {
        logMessage(tr("所有设备连接成功"));
        m_statusLabel->setText(tr("所有设备已连接"));
    } else {
        logMessage(tr("部分设备连接失败"));
        m_statusLabel->setText(tr("部分设备连接失败"));
    }
}

void MainWindow::onDisconnectAllDevices()
{
    m_deviceManager->disconnectAllDevices();
    logMessage(tr("所有设备已断开"));
    m_statusLabel->setText(tr("所有设备已断开"));
}

void MainWindow::onClearLog()
{
    m_logWidget->clear();
    logMessage(tr("日志已清除"));
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("关于设备通信管理器"),
        tr("设备通信管理器 v1.0\n\n"
           "支持多种设备通信协议:\n"
           "- 串口通信 (RS232/RS485)\n"
           "- TCP客户端\n"
           "- UDP通信\n\n"
           "功能特性:\n"
           "- 拖拽式设备添加\n"
           "- 可视化设备配置\n"
           "- 实时状态监控\n"
           "- 数据收发日志"));
}

void MainWindow::logMessage(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logEntry = QString("[%1] %2").arg(timestamp).arg(message);
    m_logWidget->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = m_logWidget->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logWidget->setTextCursor(cursor);
}

void MainWindow::updateStatusBar()
{
    int deviceCount = m_deviceManager->getDeviceCount();
    m_deviceCountLabel->setText(tr("设备数量: %1").arg(deviceCount));
} 
