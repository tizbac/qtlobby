#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>

class MapWidget : public QWidget {
public:
    MapWidget(QWidget* parent = 0);
    void paintEvent(QPaintEvent * event);
    void setImage(QImage image);
    void setErrorMessage(QString msg);
private:
    QPixmap m_pixmap;
    QString m_error;
};

#endif // MAPWIDGET_H
