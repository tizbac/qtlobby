#include "MapWidget.h"
#include <QPainter>

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
}

void MapWidget::setImage(QImage image) {
    m_pixmap = QPixmap::fromImage(image);
    //m_pixmap = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
}

void MapWidget::paintEvent ( QPaintEvent * event ) {
    QPixmap scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
    QPainter p(this);
    if(!m_error.isNull()) {
        QFontMetrics fm(font());
        int x = (width() - fm.width(m_error)) / 2;
        int y = (height() - fm.height()) / 2;
        p.drawText(x, y, m_error);
        return;
    }
    if(scaled.isNull()) return;
    int x = (width() - scaled.width()) / 2;
    int y = (height() - scaled.height()) / 2;
    p.drawPixmap(x,y,scaled.width(), scaled.height(), scaled);
    event->accept();
}

void MapWidget::setErrorMessage(QString msg) {
    m_error = msg;
}
