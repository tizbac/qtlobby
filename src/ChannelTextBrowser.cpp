// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "ChannelTextBrowser.h"
#include <QTextCursor>
#include <QClipboard>
#include <QApplication>

ChannelTextBrowser::ChannelTextBrowser(QWidget * parent) : QTextBrowser(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));
    m_contextMenu = new QMenu(this);
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
    m_contextMenu->clear();
    QAction* copy = m_contextMenu->addAction("Copy");
    QAction* select = m_contextMenu->addAction("Select all");
    QAction* jl_this = NULL;
    QAction* jl_all = NULL;
    if(m_menuEnabled) {
        m_contextMenu->addSeparator();
        jl_this = m_contextMenu->addAction("Enable join/leave messages for this channel");
        jl_this->setCheckable(true);
        jl_this->setChecked(m_enableJoinLeave);
        jl_all = m_contextMenu->addAction("Enable join/leave messages by default");
        jl_all->setCheckable(true);
        jl_all->setChecked(m_enableJoinLeaveDefault);
    }
    QAction *action = m_contextMenu->exec( this->viewport()->mapToGlobal( point ) );
    if(action == copy) {
        QApplication::clipboard()->setText(textCursor().selectedText());
    } else if(action == select) {
        selectAll();
    } else if(action == jl_this) {
        emit enableJoinLeave(jl_this->isChecked());
        m_enableJoinLeave = jl_this->isChecked();
    } else if(action == jl_all) {
        emit enableJoinLeaveDefault(jl_all->isChecked());
        m_enableJoinLeaveDefault = jl_all->isChecked();
    }
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
