// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include <QApplication>
#include "MainWindow.h"

#ifdef Q_WS_WIN
#ifdef _MSC_VER
#include "MiniDumper.h"
#else
#include <windows.h>
#endif
#endif

int main( int argc, char *argv[] ) {
    #ifdef Q_WS_WIN
    #ifdef _MSC_VER
    MiniDumper(TEXT("QtLobby"));
    #else
    LoadLibrary("exchndl.dll");
    #endif
    QApplication app( argc, argv );
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(":/i18n/qtlobby_") + locale );
    app.installTranslator(&translator);

    if ( !QSystemTrayIcon::isSystemTrayAvailable() ) {
        QMessageBox::critical( 0, QObject::tr( "Systray" ),
                               QObject::tr( "I couldn't detect any system tray "
                                            "on this system." ) );
    }
    MainWindow mw;
    mw.show();
    return app.exec();
}
