#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QRect>
#include <QMap>

class MapWidget : public QWidget {
    Q_OBJECT
public:
    MapWidget(QWidget* parent = 0);
    void paintEvent(QPaintEvent * event);
    void setImage(QImage image);
    void setErrorMessage(QString msg);
public slots:
    void addStartRect(int ally, QRect r);
    void removeStartRect(int ally);
    void setMyAllyTeam(int n);
protected:
    void resizeEvent(QResizeEvent * event);
    void drawStartRecs();

    QMap<int, QRect> startRects;
    QPixmap m_pixmap;
    QPixmap m_scaled;
    QPixmap m_withRects;
    QString m_error;
    int myAlly;
};

#endif // MAPWIDGET_H
