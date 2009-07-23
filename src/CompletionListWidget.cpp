// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "CompletionListWidget.h"
#include <QDebug>

CompletionListWidget::CompletionListWidget(QWidget* parent) : QListWidget(parent) {
    connect(this, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(selectionCompleted()));
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectionCompleted()));
    m_start = 0;
    m_length = 0;
}

void CompletionListWidget::setCompletionParams(int start, int length) {
    m_start = start;
    m_length = length;
}

void CompletionListWidget::selectionCompleted(bool addSpace) {
    hide();
    QString result = selectedItems()[0]->text();
    if (addSpace)
        result += " ";
    emit completionSelected(m_start, m_length, result);
    deleteLater();
}

void CompletionListWidget::keyPressEvent (QKeyEvent * event) {
    if ((event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z)
        || (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
        || event->key() == Qt::Key_BracketLeft
                || event->key() == Qt::Key_BracketRight
                || event->key() == Qt::Key_Underscore) {
        m_buffer += event->text();
        emit simulateKeyEvent(event);
        //m_start++;
        m_length++;
        updateItems();
        return;
    } else if (event->key() == Qt::Key_Backspace) {
        if (m_buffer.length()) {
            m_buffer.chop(1);
            emit simulateKeyEvent(event);
            //m_start--;
            m_length--;
            updateItems();
        }
        return;
    } else if (event->key() == Qt::Key_Escape) {
        hide();
    } else if (event->key() == Qt::Key_Space) {
        selectionCompleted(true);
    }
    QListWidget::keyPressEvent(event);
}

void CompletionListWidget::updateItems() {
    QStringList list;
    for (int i = 0; i < m_items.size(); i++) {
        if (m_items.at(i).contains(m_buffer, Qt::CaseInsensitive))
            list << m_items.at(i);
    }
    if (!list.size()) hide();
    list.sort();
    clear();
    addItems(list);
    setCurrentRow(0);
    resize(sizeHint());
}

void CompletionListWidget::setInitianSubstitutions(QStringList list) {
    m_items = list;
    updateItems();
}

void CompletionListWidget::setInitianBuffer(QString buffer) {
    m_buffer = buffer;
}
