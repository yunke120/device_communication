#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("DeviceCommunicationManager");
    app.setApplicationDisplayName("设备通信管理器");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("YourCompany");
    app.setOrganizationDomain("yourcompany.com");
    
    // 设置应用程序样式
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 设置应用程序图标
    // app.setWindowIcon(QIcon(":/icons/app.png"));
    
    // 创建并显示主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
} 