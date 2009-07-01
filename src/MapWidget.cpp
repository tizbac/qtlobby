#include "MapWidget.h"
#include <QPainter>

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
}

void MapWidget::setImage(QImage image) {
    m_pixmap = QPixmap::fromImage(image);
    update();
    m_scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
}

void MapWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);
    if(!m_error.isNull()) {
        QFontMetrics fm(font());
        int x = (width() - fm.width(m_error)) / 2;
        int y = (height() - fm.height()) / 2;
        p.drawText(x, y, m_error);
        return;
    }
    if(m_scaled.isNull()) return;
    int x = (width() - m_scaled.width()) / 2;
    int y = (height() - m_scaled.height()) / 2;
    p.drawPixmap(x,y,m_scaled.width(), m_scaled.height(), m_scaled);
    event->accept();
}

void MapWidget::resizeEvent(QResizeEvent * event) {
    if(event->oldSize() == size()) return;
    m_scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
}

void MapWidget::setErrorMessage(QString msg) {
    m_error = msg;
}
