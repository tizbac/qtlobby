//
// C++ Implementation: InputLine
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "InputLine.h"
#include <QDebug>

InputLine::InputLine( QWidget * parent ) : QLineEdit( parent ), historyIndex( 0 ) {
  history << "" << "";
}
InputLine::~InputLine() {
}

/**
 * keyPressEvent and arrowkeys history behavior
 * @param event the keyEvent from QObject
 */
void InputLine::keyPressEvent( QKeyEvent * event ) {
  if ( Qt::Key_Up == event->key() ) {
    if ( historyIndex < history.count() - 2 ) {
      setText( history[++historyIndex] );
    }
    QLineEdit::keyPressEvent( event );
    return;
  }
  else if ( Qt::Key_Down == event->key() ) {
    if ( historyIndex > 0 && historyIndex < history.count() ) {
      setText( history[--historyIndex] );
    }
    QLineEdit::keyPressEvent( event );
    return;
  }
  else if ( Qt::Key_Return == event->key() || Qt::Key_Return == event->key() ) {
    history[0] = text();
    returnPressed();
    historyIndex = 0;
  }
  else
    historyIndex = 0;
  QLineEdit::keyPressEvent( event );
  history[0] = text();
}

/**
 * sends the input text after short trimming
 * and saves the history
 */
void InputLine::returnPressed() {
  //allow at most one leading space and now trailing spaces
  QString input = text().trimmed();
  if ( input.size() == 0 )
    return;
  input = input.prepend( text().at( 0 ).isSpace() ? " " : "" );
  clear();
  // this disables multiple equal history messages in a series
  if( history[1] != history[0] )
    history.prepend( "" );
  // limit the history to 20 history entries
  if( history.count() > 22 )
    history.removeLast();
  emit sendInput( input );
  emit returnPressed();
}
