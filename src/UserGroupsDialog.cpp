// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "src/UserGroupsDialog.h"
#include "ui_UserGroupsDialog.h"
#include <QDebug>
#include <QColorDialog>

UserGroupsDialog::UserGroupsDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::UserGroupsDialog) {
    m_ui->setupUi(this);
    UserGroupList* groups = UserGroupList::getInstance();
    groups->load();
    m_ui->groupsListWidget->addItems(groups->getGroupNames());
    connect(m_ui->groupsListWidget, SIGNAL(currentTextChanged(const QString&)), SLOT(onCurrentTextChanged(const QString&)));
}

UserGroupsDialog::~UserGroupsDialog() {
    delete m_ui;
}

void UserGroupsDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UserGroupsDialog::onNewClicked() {
    UserGroupList* groups = UserGroupList::getInstance();
    if (!groups->size()) {
        m_ui->nameLineEdit->setEnabled(true);
        m_ui->descrLineEdit->setEnabled(true);
        m_ui->colorLabel->setEnabled(true);
        m_ui->membersPlainTextEdit->setEnabled(true);
    }
    UserGroup* g = new UserGroup();
    int n = 1;
    while (groups->findGroup("New group"+QString::number(n))) n++;
    g->name = "New group"+QString::number(n);
    g->description = "New Descriptioin";
    g->members << "Player1" << "Player2";
    groups->append(g);
    m_ui->groupsListWidget->clear();
    m_ui->groupsListWidget->addItems(groups->getGroupNames());
}

void UserGroupsDialog::onDeleteClicked() {
    UserGroupList* groups = UserGroupList::getInstance();
    groups->removeGroup(currentName);
    m_ui->groupsListWidget->clear();
    m_ui->groupsListWidget->addItems(groups->getGroupNames());
    if (!groups->size()) {
        m_ui->nameLineEdit->setEnabled(false);
        m_ui->descrLineEdit->setEnabled(false);
        m_ui->colorLabel->setEnabled(false);
        m_ui->membersPlainTextEdit->setEnabled(false);
    }
}

void UserGroupsDialog::onSaveClicked() {
    UserGroupList* groups = UserGroupList::getInstance();
    UserGroup* g = groups->findGroup(currentName);
    if (g) {
        g->name = m_ui->nameLineEdit->text();
        g->description = m_ui->descrLineEdit->text();
        g->color = currentcolor;
        g->members = m_ui->membersPlainTextEdit->toPlainText().split(QRegExp("[ ,;]+"));
    }
    groups->save();
    m_ui->groupsListWidget->clear();
    m_ui->groupsListWidget->addItems(groups->getGroupNames());
    emit groupsChanged();
    groups->updateMappings();
}

void UserGroupsDialog::onColorClicked() {
    QColor newColor = QColorDialog::getColor (currentcolor, this, "Select color for group");
    UserGroup* g = UserGroupList::getInstance()->findGroup(currentName);
    g->color = newColor;
    currentcolor = newColor;
    m_ui->colorLabel->setText(QString("<a href=\"#\"><span style=\" text-decoration: underline; color:%1;\">%1</span></a>").arg(newColor.name()));
}

void UserGroupsDialog::onCurrentTextChanged(const QString & currentText) {
    UserGroupList* groups = UserGroupList::getInstance();
    UserGroup* g = groups->findGroup(currentText);
    if (!g) return;
    m_ui->nameLineEdit->setText(g->name);
    m_ui->descrLineEdit->setText(g->description);
    currentcolor = g->color;
    m_ui->colorLabel->setText(QString("<a href=\"#\"><span style=\" text-decoration: underline; color:%1;\">%1</span></a>").arg(currentcolor.name()));
    m_ui->membersPlainTextEdit->setPlainText(g->members.join(", "));
    currentName = currentText;
}
