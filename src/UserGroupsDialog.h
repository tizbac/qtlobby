#ifndef USERGROUPSDIALOG_H
#define USERGROUPSDIALOG_H

#include <QtGui/QDialog>
#include <QList>
#include "Settings.h"
#include "UserGroup.h"

namespace Ui {
    class UserGroupsDialog;
}

class UserGroupsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(UserGroupsDialog)
        public:
            explicit UserGroupsDialog(QWidget *parent = 0);
    virtual ~UserGroupsDialog();

protected:
    virtual void changeEvent(QEvent *e);
private slots:
    void onNewClicked();
    void onDeleteClicked();
    void onSaveClicked();
    void onColorClicked();
    void onCurrentTextChanged(const QString & currentText);
signals:
    void groupsChanged();

private:
    Ui::UserGroupsDialog *m_ui;
    QColor currentcolor;
    QString currentName;
};

#endif // USERGROUPSDIALOG_H
