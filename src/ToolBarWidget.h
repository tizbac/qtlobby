#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QtGui/QWidget>
#include <QResizeEvent>
#include "ui_ToolBarWidget.h"

class ToolBarWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ToolBarWidget)
public:
    explicit ToolBarWidget(QWidget *parent = 0);
    virtual ~ToolBarWidget();

    Ui::ToolBarWidget *ui;

public slots:
    void onNextTab();
    void onPreviousTab();
    void onCloseTab();
protected:
    virtual void changeEvent(QEvent *e);
    virtual void resizeEvent(QResizeEvent * event);
};

#endif // TOOLBARWIDGET_H
