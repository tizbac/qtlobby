#ifndef COMPLETIONLISTWIDGET_H
#define COMPLETIONLISTWIDGET_H

#include <QListWidget>

class CompletionListWidget : public QListWidget {
    Q_OBJECT
public:
    CompletionListWidget(QWidget* parent = 0);
    void setCompletionParams(int start, int length);
private slots:
    void selectionCompleted();
signals:
    void completionSelected(int start, int length, QString text);
private:
    int m_start;
    int m_length;
};

#endif // COMPLETIONLISTWIDGET_H
