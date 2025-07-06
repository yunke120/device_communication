#include "devicedragwidget.h"
#include "../device/devicefactory.h"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QStyleOption>

DeviceDragWidget::DeviceDragWidget(const QString& deviceType, const QString& name, QWidget* parent)
    : QLabel(parent), m_deviceType(deviceType), m_deviceName(name), m_isDragging(false)
{
    initUI();
    setToolTip(m_deviceType);
}

DeviceDragWidget::~DeviceDragWidget()
{
}

void DeviceDragWidget::initUI()
{
    setFixedSize(80, 80);
    setAlignment(Qt::AlignCenter);
    setText(m_deviceName);
    setToolTip(m_deviceType);
    
    // 设置样式
    updateStyle();
    
    // 设置接受鼠标事件
    setMouseTracking(true);
}

void DeviceDragWidget::updateStyle()
{
    QString styleSheet = QString(
        "DeviceDragWidget {"
        "    border: 2px solid #cccccc;"
        "    border-radius: 8px;"
        "    background-color: #f0f0f0;"
        "    color: #333333;"
        "    font-weight: bold;"
        "    font-size: 10px;"
        "}"
        "DeviceDragWidget:hover {"
        "    border-color: #4CAF50;"
        "    background-color: #e8f5e8;"
        "}"
        "DeviceDragWidget:pressed {"
        "    border-color: #2196F3;"
        "    background-color: #e3f2fd;"
        "}"
    );
    
    setStyleSheet(styleSheet);
}

void DeviceDragWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
        m_isDragging = false;
    }
    QLabel::mousePressEvent(event);
}

void DeviceDragWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    
    if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }
    
    if (!m_isDragging) {
        m_isDragging = true;
        
        QDrag* drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData;
        
        // 设置拖拽数据
        mimeData->setData("application/x-device-type", m_deviceType.toUtf8());
        mimeData->setData("application/x-device-name", m_deviceName.toUtf8());
        mimeData->setText(m_deviceName);
        
        drag->setMimeData(mimeData);
        
        // 设置拖拽时的预览
        QPixmap pixmap = this->grab();
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
        
        // 执行拖拽
        Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
        Q_UNUSED(dropAction)
        
        m_isDragging = false;
    }
}

void DeviceDragWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_isDragging = false;
    QLabel::mouseReleaseEvent(event);
}

void DeviceDragWidget::enterEvent(QEnterEvent* event)
{
    setCursor(Qt::OpenHandCursor);
    QLabel::enterEvent(event);
}

void DeviceDragWidget::leaveEvent(QEvent* event)
{
    setCursor(Qt::ArrowCursor);
    QLabel::leaveEvent(event);
} 