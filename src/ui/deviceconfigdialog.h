#ifndef DEVICECONFIGDIALOG_H
#define DEVICECONFIGDIALOG_H

#include "../device/devicebase.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QMap>
#include <QVariant>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class DeviceConfigDialog;
}
QT_END_NAMESPACE

class DeviceConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceConfigDialog(DeviceBase* device, QWidget* parent = nullptr);
    ~DeviceConfigDialog() override;

    QMap<QString, QVariant> getConfig() const;

private slots:
    void onAccepted();
    void onRejected();
    void onTestConnection();

private:
    void initUI();
    void createSerialConfig();
    void createTcpConfig();
    void createUdpConfig();
    void loadCurrentConfig();
    void saveConfig();

    Ui::DeviceConfigDialog* ui;
    DeviceBase* m_device;
    QMap<QString, QVariant> m_config;
};

#endif // DEVICECONFIGDIALOG_H 
