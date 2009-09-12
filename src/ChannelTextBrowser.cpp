// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "ChannelTextBrowser.h"
#include "MainWindow.h"
#include <QTextCursor>
#include <QClipboard>
#include <QApplication>

ChannelTextBrowser::ChannelTextBrowser(QWidget * parent) : QTextBrowser(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));
    m_menuEnabled = true;
}

QMimeData *ChannelTextBrowser::createMimeDataFromSelection() const {
    QMimeData *res = QTextEdit::createMimeDataFromSelection();

    if (res->hasHtml()) {
        QString html = res->html();

        int pos = html.indexOf(" <img");
        int lpos = (html.indexOf(">", pos) - pos);
        html.replace(pos,lpos+1,"");

        QTextDocumentFragment frag;
        frag = QTextDocumentFragment::fromHtml(html, document());

        res->setText(frag.toPlainText());
    }
    return res;
}

void ChannelTextBrowser::onCustomContextMenuRequested(QPoint point) {
    QMenu* contextMenu = createStandardContextMenu(point);
    QAction* jl_this = NULL;
    QAction* jl_all = NULL;
    QAction* history = NULL;
    if(m_menuEnabled) {
        contextMenu->addSeparator();
        jl_this = contextMenu->addAction(tr("Enable join/leave messages for this channel"));
        jl_this->setCheckable(true);
        jl_this->setChecked(m_enableJoinLeave);
        jl_all = contextMenu->addAction(tr("Enable join/leave messages by default"));
        jl_all->setCheckable(true);
        jl_all->setChecked(m_enableJoinLeaveDefault);
        contextMenu->addSeparator();
        history = contextMenu->addAction(tr("History"));
    }
    QAction *action = contextMenu->exec( this->viewport()->mapToGlobal( point ) );
    if(action == jl_this) {
        emit enableJoinLeave(jl_this->isChecked());
        m_enableJoinLeave = jl_this->isChecked();
    } else if(action == jl_all) {
        emit enableJoinLeaveDefault(jl_all->isChecked());
        m_enableJoinLeaveDefault = jl_all->isChecked();
    } else if(action == history) {
        MainWindow::getInstance()->getHistoryDialog()->showChannel(objectName().section("_", 1));
    }
    delete contextMenu;
}

void ChannelTextBrowser::setEnableJoinLeave(bool b) {
    m_enableJoinLeave = b;
}

void ChannelTextBrowser::setEnableJoinLeaveDefault(bool b) {
    m_enableJoinLeaveDefault = b;
}

void ChannelTextBrowser::setJoinLeaveMenuEnabled(bool b) {
    m_menuEnabled = b;
}
