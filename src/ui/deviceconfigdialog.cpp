#include "deviceconfigdialog.h"
#include "ui_deviceconfigdialog.h"
#include "../manager/devicemanager.h"
#include <QMessageBox>
#include <QApplication>

DeviceConfigDialog::DeviceConfigDialog(DeviceBase* device, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DeviceConfigDialog)
    , m_device(device)
{
    initUI();
    loadCurrentConfig();
}

DeviceConfigDialog::~DeviceConfigDialog()
{
    delete ui;
}

QMap<QString, QVariant> DeviceConfigDialog::getConfig() const
{
    return m_config;
}

void DeviceConfigDialog::initUI()
{
    ui->setupUi(this);
    setWindowTitle(tr("设备配置 - %1").arg(m_device->name()));
    
    // 设置设备类型标签
    QString typeName = m_device->name();
    ui->label_device_type->setText(tr("设备类型: %1").arg(typeName));
    
    // 根据通信类型创建配置界面
    QString commType = m_device->defaultCommType();
    if (commType == "Serial") {
        createSerialConfig();
    } else if (commType == "TCP") {
        createTcpConfig();
    } else if (commType == "UDP") {
        createUdpConfig();
    } else {
        ui->groupBox_config->setTitle(tr("此设备类型暂不支持配置"));
    }
    
    // 连接按钮信号
    connect(ui->pushButton_test, &QPushButton::clicked, this, &DeviceConfigDialog::onTestConnection);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DeviceConfigDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &DeviceConfigDialog::onRejected);
}

void DeviceConfigDialog::createSerialConfig()
{
    ui->groupBox_config->setTitle(tr("串口配置"));
    
    // 串口端口号
    ui->comboBox_port->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBox_port->addItem(info.portName());
    }
    // 波特率
    ui->comboBox_baudrate->clear();
    QList<int> baudRates = { 9600, 19200, 38400, 57600, 115200 };
    foreach (int baud, baudRates) {
        ui->comboBox_baudrate->addItem(QString::number(baud));
    }
    // 数据位
    ui->comboBox_databits->clear();
    ui->comboBox_databits->addItem("5", QSerialPort::Data5);
    ui->comboBox_databits->addItem("6", QSerialPort::Data6);
    ui->comboBox_databits->addItem("7", QSerialPort::Data7);
    ui->comboBox_databits->addItem("8", QSerialPort::Data8);
    // 停止位
    ui->comboBox_stopbits->clear();
    ui->comboBox_stopbits->addItem("1", QSerialPort::OneStop);
    ui->comboBox_stopbits->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->comboBox_stopbits->addItem("2", QSerialPort::TwoStop);
    // 校验位
    ui->comboBox_parity->clear();
    ui->comboBox_parity->addItem("None", QSerialPort::NoParity);
    ui->comboBox_parity->addItem("Even", QSerialPort::EvenParity);
    ui->comboBox_parity->addItem("Odd", QSerialPort::OddParity);
    ui->comboBox_parity->addItem("Mark", QSerialPort::MarkParity);
    ui->comboBox_parity->addItem("Space", QSerialPort::SpaceParity);
    
    // 流控制
    ui->comboBox_flowcontrol->addItems({"None", "Hardware", "Software"});
}

void DeviceConfigDialog::createTcpConfig()
{
    ui->groupBox_config->setTitle(tr("TCP配置"));
    
    // 隐藏串口相关的控件
    ui->label_port->setVisible(false);
    ui->comboBox_port->setVisible(false);
    ui->label_baudrate->setVisible(false);
    ui->comboBox_baudrate->setVisible(false);
    ui->label_databits->setVisible(false);
    ui->comboBox_databits->setVisible(false);
    ui->label_stopbits->setVisible(false);
    ui->comboBox_stopbits->setVisible(false);
    ui->label_parity->setVisible(false);
    ui->comboBox_parity->setVisible(false);
    ui->label_flowcontrol->setVisible(false);
    ui->comboBox_flowcontrol->setVisible(false);
    
    // 重新设置标签文本
    ui->label_port->setText(tr("主机地址:"));
    ui->comboBox_port->setEditable(true);
    ui->comboBox_port->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBox_port->addItem("127.0.0.1");
    ui->comboBox_port->setVisible(true);
    ui->label_port->setVisible(true);
    
    ui->label_baudrate->setText(tr("端口:"));
    ui->comboBox_baudrate->setEditable(true);
    ui->comboBox_baudrate->addItem("8080");
    ui->comboBox_baudrate->setVisible(true);
    ui->label_baudrate->setVisible(true);
    
    ui->label_databits->setText(tr("超时时间:"));
    ui->comboBox_databits->setEditable(true);
    ui->comboBox_databits->addItem("5000");
    ui->comboBox_databits->setVisible(true);
    ui->label_databits->setVisible(true);
}

void DeviceConfigDialog::createUdpConfig()
{
    ui->groupBox_config->setTitle(tr("UDP配置"));
    
    // 隐藏串口相关的控件
    ui->label_port->setVisible(false);
    ui->comboBox_port->setVisible(false);
    ui->label_baudrate->setVisible(false);
    ui->comboBox_baudrate->setVisible(false);
    ui->label_databits->setVisible(false);
    ui->comboBox_databits->setVisible(false);
    ui->label_stopbits->setVisible(false);
    ui->comboBox_stopbits->setVisible(false);
    ui->label_parity->setVisible(false);
    ui->comboBox_parity->setVisible(false);
    ui->label_flowcontrol->setVisible(false);
    ui->comboBox_flowcontrol->setVisible(false);
    
    // 重新设置标签文本
    ui->label_port->setText(tr("本地端口:"));
    ui->comboBox_port->setEditable(true);
    ui->comboBox_port->addItem("0");
    ui->comboBox_port->setVisible(true);
    ui->label_port->setVisible(true);
    
    ui->label_baudrate->setText(tr("远程主机:"));
    ui->comboBox_baudrate->setEditable(true);
    ui->comboBox_baudrate->addItem("127.0.0.1");
    ui->comboBox_baudrate->setVisible(true);
    ui->label_baudrate->setVisible(true);
    
    ui->label_databits->setText(tr("远程端口:"));
    ui->comboBox_databits->setEditable(true);
    ui->comboBox_databits->addItem("8080");
    ui->comboBox_databits->setVisible(true);
    ui->label_databits->setVisible(true);
}

void DeviceConfigDialog::loadCurrentConfig()
{
    m_config = m_device->getConfig();
    
    if (m_device->name() == "Serial") {
        {
            int index = ui->comboBox_port->findText(m_config["port"].toString());
            if (index >= 0) ui->comboBox_port->setCurrentIndex(index);
            
            index = ui->comboBox_baudrate->findText(m_config["baudRate"].toString());
            if (index >= 0) ui->comboBox_baudrate->setCurrentIndex(index);
            
            index = ui->comboBox_databits->findText(m_config["dataBits"].toString());
            if (index >= 0) ui->comboBox_databits->setCurrentIndex(index);
            
            index = ui->comboBox_stopbits->findText(m_config["stopBits"].toString());
            if (index >= 0) ui->comboBox_stopbits->setCurrentIndex(index);
            
            index = ui->comboBox_parity->findText(m_config["parity"].toString());
            if (index >= 0) ui->comboBox_parity->setCurrentIndex(index);
            
            index = ui->comboBox_flowcontrol->findText(m_config["flowControl"].toString());
            if (index >= 0) ui->comboBox_flowcontrol->setCurrentIndex(index);
        }
    }
        
    else if (m_device->name() == "TCP") {
        ui->comboBox_port->setCurrentText(m_config["host"].toString());
        ui->comboBox_baudrate->setCurrentText(m_config["port"].toString());
        ui->comboBox_databits->setCurrentText(m_config["timeout"].toString());
    }
        
    else if (m_device->name() == "UDP") {
        ui->comboBox_port->setCurrentText(m_config["localPort"].toString());
        ui->comboBox_baudrate->setCurrentText(m_config["remoteHost"].toString());
        ui->comboBox_databits->setCurrentText(m_config["remotePort"].toString());
    }
        
}

void DeviceConfigDialog::saveConfig()
{
    if (m_device->name() == "Serial") {
        m_config["port"] = ui->comboBox_port->currentText();
        m_config["baudRate"] = ui->comboBox_baudrate->currentText();
        m_config["dataBits"] = ui->comboBox_databits->currentText();
        m_config["stopBits"] = ui->comboBox_stopbits->currentText();
        m_config["parity"] = ui->comboBox_parity->currentText();
        m_config["flowControl"] = ui->comboBox_flowcontrol->currentText();
    }
        
    else if (m_device->name() == "TCP") {
        m_config["host"] = ui->comboBox_port->currentText();
        m_config["port"] = ui->comboBox_baudrate->currentText().toInt();
        m_config["timeout"] = ui->comboBox_databits->currentText().toInt();
    }
        
    else if (m_device->name() == "UDP") {
        m_config["localPort"] = ui->comboBox_port->currentText().toInt();
        m_config["remoteHost"] = ui->comboBox_baudrate->currentText();
        m_config["remotePort"] = ui->comboBox_databits->currentText().toInt();
    }
        
}

void DeviceConfigDialog::onAccepted()
{
    saveConfig();
    DeviceManager::instance()->setDeviceConfig(m_device->name(), m_config);
    accept();
}

void DeviceConfigDialog::onRejected()
{
    reject();
}

void DeviceConfigDialog::onTestConnection()
{
    saveConfig();
    
    // 临时设置配置
    DeviceManager::instance()->setDeviceConfig(m_device->name(), m_config);
    
    // 测试连接
    qDebug() << "测试连接：" << m_device->name();
    if (DeviceManager::instance()->connectDevice(m_device->name())) {
        QMessageBox::information(this, tr("连接测试"), tr("连接成功！"));
        DeviceManager::instance()->disconnectDevice(m_device->name());
    } else {
        QString error = DeviceManager::instance()->getDeviceError(m_device->name());
        QMessageBox::warning(this, tr("连接测试"), tr("连接失败: %1").arg(error));
    }
}
