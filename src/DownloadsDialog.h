#ifndef DOWNLOADSDIALOG_H
#define DOWNLOADSDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class DownloadsDialog;
}

class DownloadsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DownloadsDialog)
public:
    explicit DownloadsDialog(QWidget *parent = 0);
    virtual ~DownloadsDialog();

protected:
    virtual void changeEvent(QEvent *e);
    virtual void showEvent(QShowEvent* event);

private:
    Ui::DownloadsDialog *m_ui;

private slots:
    void on_clearPushButton_clicked();
};

#endif // DOWNLOADSDIALOG_H
