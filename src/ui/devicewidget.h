#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include "../device/devicebase.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(DeviceBase* device, QWidget* parent = nullptr);
    ~DeviceWidget() override;

    void updateStatus(DeviceStatus status);
    void showError(const QString& error);

signals:
    void doubleClicked(const QString& name);
    void configRequested(const QString& name);
    void connectRequested(const QString& name);
    void disconnectRequested(const QString& name);
    void removeRequested(const QString& name);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void onConnectClicked();
    void onConfigClicked();
    void onRemoveClicked();

private:
    void initUI();
    void updateStatusDisplay();
    void createContextMenu();

    DeviceBase* m_device;
    QLabel* m_nameLabel;
    QLabel* m_typeLabel;
    QLabel* m_statusLabel;
    QLabel* m_modelLabel;
    QPushButton* m_connectButton;
    QPushButton* m_configButton;
    QPushButton* m_removeButton;
    QMenu* m_contextMenu;
    QString m_lastError;
};

#endif // DEVICEWIDGET_H 