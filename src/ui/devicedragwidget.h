#ifndef DEVICEDRAGWIDGET_H
#define DEVICEDRAGWIDGET_H

#include "../device/devicebase.h"
#include <QLabel>
#include <QWidget>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

class DeviceDragWidget : public QLabel
{
    Q_OBJECT

public:
    explicit DeviceDragWidget(const QString& deviceType, const QString& name, QWidget* parent = nullptr);
    ~DeviceDragWidget() override;

    QString deviceType() const { return m_deviceType; }
    QString deviceName() const { return m_deviceName; }

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void initUI();
    void updateStyle();

    QString m_deviceType;
    QString m_deviceName;
    bool m_isDragging;
    QPoint m_dragStartPos;
};

#endif // DEVICEDRAGWIDGET_H 