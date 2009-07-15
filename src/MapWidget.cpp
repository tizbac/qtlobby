#include "MapWidget.h"
#include <QPainter>
#include <QDebug>

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
}

void MapWidget::setImage(QImage image) {
    m_pixmap = QPixmap::fromImage(image);
    update();
    m_scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
}

void MapWidget::drawStartRecs() {
    if (m_scaled.isNull()) return;
    m_withRects = m_scaled;
    QPainter p(&m_withRects);
    for (QMap<int, QRect>::const_iterator i = startRects.begin(); i != startRects.end(); i++) {
        QRect scaled = i.value();
        scaled.setWidth(scaled.width()/200.*m_scaled.width());
        scaled.setHeight(scaled.height()/200.*m_scaled.height());
        scaled.moveTo(scaled.x()/200.*m_scaled.width(), scaled.y()/200.*m_scaled.height());
        int alpha = 100;
        int width = 1;
        QColor red(Qt::red);
        QColor redFill(Qt::red);
        redFill.setAlpha(alpha);
        QColor green(Qt::green);
        QColor greenFill(Qt::green);
        greenFill.setAlpha(alpha);
        if (i.key() == myAlly) {
            p.setBrush(greenFill);
            p.setPen(QPen(green, width));
        } else {
            p.setBrush(redFill);
            p.setPen(QPen(red, width));
        }
        p.drawRect(scaled);
    }
    p.end();
}

void MapWidget::paintEvent ( QPaintEvent * /*event*/ ) {
    QPainter p(this);
    if (!m_error.isNull()) {
        QFontMetrics fm(font());
        int x = (width() - fm.width(m_error)) / 2;
        int y = (height() - fm.height()) / 2;
        p.drawText(x, y, m_error);
        return;
    }
    if (m_scaled.isNull()) return;
    drawStartRecs();
    int x = (width() - m_scaled.width()) / 2;
    int y = (height() - m_scaled.height()) / 2;
    p.drawPixmap(x,y,m_withRects.width(), m_withRects.height(), m_withRects);
}

void MapWidget::resizeEvent(QResizeEvent * event) {
    if (event->oldSize() == size()) return;
    if (m_pixmap.isNull()) return;
    m_scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
}

void MapWidget::setErrorMessage(QString msg) {
    m_error = msg;
}

void MapWidget::addStartRect(int ally, QRect r) {
    startRects[ally] = r;
    update();
}

void MapWidget::setMyAllyTeam(int n) {
    myAlly = n-1;
    update();
}

void MapWidget::removeStartRect(int ally) {
    startRects.remove(ally);
    update();
}
