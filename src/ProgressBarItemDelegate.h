#ifndef PROGRESSBARITEMDELEGATE_H
#define PROGRESSBARITEMDELEGATE_H

#include <QStyledItemDelegate>

class ProgressBarItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ProgressBarItemDelegate(QObject * parent = 0);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // PROGRESSBARITEMDELEGATE_H
