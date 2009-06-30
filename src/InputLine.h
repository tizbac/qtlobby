//
// C++ Interface: InputLine
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INPUTLINE_H
#define INPUTLINE_H

#include "AbstractStateClient.h"
#include "Users.h"
#include <QStringList>
#include <QString>
#include <QLineEdit>
#include <QWidget>
#include <QObject>
#include <QKeyEvent>

/**
the input line provides a message history, user name tab completion etc.

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class InputLine : public QLineEdit {
    Q_OBJECT
public:
    InputLine( QWidget * parent = 0 );
    virtual ~InputLine();
    void setUsers(Users* users);

    public slots:
    void onCompletionSelected(int start, int length, QString text);

signals:
    void sendInput( QString input );
    void requestCompletionList(QString str);

protected:
    void keyPressEvent( QKeyEvent * event );
    bool event(QEvent* event);

private:
    void returnPressed();
    int historyIndex;
    QStringList history;
    Users* users;
};

#endif
