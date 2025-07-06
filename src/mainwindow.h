#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QTextEdit>
#include <QTimer>
#include <QLabel>
#include "ui/devicelistwidget.h"
#include "ui/devicedragwidget.h"
#include "ui/deviceconfigdialog.h"
#include "manager/devicemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
class QStatusBar;
class QToolBar;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDeviceDoubleClicked(const QString& name);
    void onDeviceConfigRequested(const QString& name);
    void onDeviceConnectRequested(const QString& name);
    void onDeviceDisconnectRequested(const QString& name);
    void onDeviceRemoveRequested(const QString& name);
    void onDeviceDataReceived(const QString& name, const QByteArray& data);
    void onDeviceErrorOccurred(const QString& name, const QString& error);
    
    void onConnectAllDevices();
    void onDisconnectAllDevices();
    void onClearLog();
    void onAbout();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createCentralWidget();
    void createDevicePalette();
    void setupConnections();
    void logMessage(const QString& message);
    void updateStatusBar();

    // UI组件
    Ui::MainWindow* ui;
    QSplitter* m_mainSplitter;
    QWidget* m_devicePalette;
    DeviceListWidget* m_deviceList;
    QTextEdit* m_logWidget;
    
    // 状态栏
    QLabel* m_statusLabel;
    QLabel* m_deviceCountLabel;
    
    // 设备管理器
    DeviceManager* m_deviceManager;
};

#endif // MAINWINDOW_H 