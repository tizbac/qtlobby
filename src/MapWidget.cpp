// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "MapWidget.h"
#include <QPainter>
#include <QDebug>
#include "Settings.h"

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
}

void MapWidget::setImage(QImage image) {
    m_pixmap = QPixmap::fromImage(image);
    switch(Settings::Instance()->value("MapViewing/startPos/startRect/brushNumber").toInt()) {
        case 1:
        m_brushStyle = Qt::BDiagPattern;
        break;
        case 2:
        m_brushStyle = Qt::FDiagPattern;
        break;
        case 3:
        m_brushStyle = Qt::DiagCrossPattern;
        break;
        case 0:
        default:
        m_brushStyle = Qt::SolidPattern;
        break;
    }
    m_borderWidth = Settings::Instance()->value("MapViewing/startPos/startRect/borderWidth").toInt();
    m_alpha = Settings::Instance()->value("MapViewing/startPos/startRect/alpha").toInt();
    update();
    m_scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
}

void MapWidget::drawStartRecs() {
    if (m_scaled.isNull()) return;
    m_withRects = m_scaled;
    if(!m_drawStartPositions) return;
    QPainter p(&m_withRects);
    for (QMap<int, QRect>::const_iterator i = startRects.begin(); i != startRects.end(); i++) {
        QRect scaled = i.value();
        scaled.setWidth(scaled.width()/200.*m_scaled.width());
        scaled.setHeight(scaled.height()/200.*m_scaled.height());
        scaled.moveTo(scaled.x()/200.*m_scaled.width(), scaled.y()/200.*m_scaled.height());
        QColor red(Qt::red);
        QColor redFill(Qt::red);
        redFill.setAlpha(m_alpha);
        QColor green(Qt::green);
        QColor greenFill(Qt::green);
        greenFill.setAlpha(m_alpha);
        if (i.key() == myAlly) {
            p.setBrush(QBrush(greenFill, m_brushStyle));
            p.setPen(QPen(green, m_borderWidth));
        } else {
            p.setBrush(QBrush(redFill, m_brushStyle));
            p.setPen(QPen(red, m_borderWidth));
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
    p.setBrush(Qt::black);
    p.setPen(Qt::black);
    p.drawRect(x,y,m_withRects.width(), m_withRects.height());
    p.drawPixmap(x,y,m_withRects.width(), m_withRects.height(), m_withRects);
}

void MapWidget::resizeEvent(QResizeEvent * event) {
    if (event->oldSize() == size()) return;
    if (m_pixmap.isNull()) return;
    m_scaled = m_pixmap.scaled(size(), Qt::KeepAspectRatio);
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
