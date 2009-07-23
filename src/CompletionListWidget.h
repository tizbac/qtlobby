// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef COMPLETIONLISTWIDGET_H
#define COMPLETIONLISTWIDGET_H

#include <QListWidget>
#include <QKeyEvent>

class CompletionListWidget : public QListWidget {
    Q_OBJECT
public:
    CompletionListWidget(QWidget* parent = 0);
    void setInitianSubstitutions(QStringList list);
    void setInitianBuffer(QString buffer);
    void setCompletionParams(int start, int length);
private slots:
    void selectionCompleted(bool addSpace = false);
protected:
    void keyPressEvent (QKeyEvent * event);
    void updateItems();
signals:
    void completionSelected(int start, int length, QString text);
    void simulateKeyEvent(QKeyEvent* event);
private:
    int m_start;
    int m_length;
    QStringList m_items;
    QString m_buffer;
};

#endif // COMPLETIONLISTWIDGET_H
