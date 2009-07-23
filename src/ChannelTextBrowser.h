// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef CHANNELTEXTBROWSER_H
#define CHANNELTEXTBROWSER_H

#include <QTextBrowser>
#include <QMimeData>
#include <QDebug>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QMenu>

class ChannelTextBrowser : public QTextBrowser {
    Q_OBJECT
public:
    ChannelTextBrowser(QWidget * parent = 0);
signals:
    void enableJoinLeave(bool b);
    void enableJoinLeaveDefault(bool b);
public slots:
    void setJoinLeaveMenuEnabled(bool b);
    void setEnableJoinLeave(bool b);
    void setEnableJoinLeaveDefault(bool b);
private slots:
    void onCustomContextMenuRequested(QPoint p);
protected:
    QMimeData *createMimeDataFromSelection () const;
    bool m_enableJoinLeave;
    bool m_enableJoinLeaveDefault;
    bool m_menuEnabled;
};

#endif // CHANNELTEXTBROWSER_H
