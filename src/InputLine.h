// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef INPUTLINE_H
#define INPUTLINE_H

#include "AbstractStateClient.h"
#include "Users.h"
#include <QStringList>
#include <QString>
#include <QMap>
#include <QLineEdit>
#include <QWidget>
#include <QObject>
#include <QKeyEvent>

/**
 * The input line provides a message history, user name tab completion etc.
 */
class InputLine : public QLineEdit {
    Q_OBJECT
public:
    InputLine( QWidget * parent = 0 );
    virtual ~InputLine();
    void setUsers(Users* users);

public slots:
    void onCompletionSelected(int start, int length, QString text);
    void emulateKeyEvent(QKeyEvent* event);
    void onChannelActivated(QModelIndex index);

signals:
    void sendInput( QString input );
    void requestCompletionList(QString str);

protected:
    void keyPressEvent( QKeyEvent * event );
    bool event(QEvent* event);

private:
    QMap <int, QString> buffers;
    int currentIndex;
    bool previousWasTab;
    void returnPressed();
    int historyIndex;
    QStringList history;
    Users* users;
};

#endif
