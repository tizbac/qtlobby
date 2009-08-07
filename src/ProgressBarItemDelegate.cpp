// $Id: InfoChannel.cpp 229 2009-07-23 02:18:19Z mirko.windhoff $
// QtLobby released under the GPLv3, see COPYING for details.
#include "ProgressBarItemDelegate.h"
#include <QStyleOptionProgressBar>
#include <QApplication>

ProgressBarItemDelegate::ProgressBarItemDelegate(QObject * parent) : QStyledItemDelegate(parent) {
}

void ProgressBarItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    int progress = index.data().toInt();

    QStyleOptionProgressBar progressBarOption;
    progressBarOption.rect = option.rect;
    if(progress == -1) {
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 1;
        progressBarOption.progress = 0;
        progressBarOption.text = tr("File size unknown");
    } else {
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
    }
    progressBarOption.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                       &progressBarOption, painter);
}
