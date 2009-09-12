#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QtGui/QDialog>
#include "AbstractChannel.h"

namespace Ui {
    class HistoryDialog;
}

class HistoryDialog : public QDialog {
    Q_OBJECT
public:
    explicit HistoryDialog(QWidget *parent = 0);
    virtual ~HistoryDialog();
    void showChannel(QString channel);
    void showPrivate(QString nick);
signals:
    void needReplay(QDate from, QDate to);

protected:
    virtual void changeEvent(QEvent *e);

private:
    void replayHistory();
    Ui::HistoryDialog *m_ui;
    AbstractChannel* m_channel;

private slots:
    void on_toDateEdit_editingFinished();
    void on_fromDateEdit_editingFinished();
    void on_nameLineEdit_editingFinished();
    void on_typeComboBox_activated(QString );
};

#endif // HISTORYDIALOG_H
