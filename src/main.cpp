#include <QApplication>
#include "MainWindow.h"

int main( int argc, char *argv[] )
{
  QApplication app( argc, argv );
  if ( !QSystemTrayIcon::isSystemTrayAvailable() ) {
    QMessageBox::critical( 0, QObject::tr( "Systray" ),
                           QObject::tr( "I couldn't detect any system tray "
                                        "on this system." ) );
  }
  MainWindow mw;
  mw.show();
  return app.exec();
}
