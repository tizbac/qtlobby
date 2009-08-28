// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include <QApplication>
#include "MainWindow.h"
#include "MapInfoLoader.h"
#include "MapOverviewDialog.h"

#ifdef Q_WS_WIN
#ifdef _MSC_VER
#include "MiniDumper.h"
#else
#include <windows.h>
#endif
#endif

bool mainLoop = true;

void cmd_3dpreview(QApplication& app, QString mapname) {
    mainLoop = false;
    MapInfoLoader loader;
    loader.setMap(mapname);
    loader.start();
    loader.wait();
    MapOverviewDialog d;
    d.setSource(mapname, loader.mapinfo.description, loader.minimap, loader.metalmap, loader.rawHeightmap);
    d.setWindowState(Qt::WindowMaximized);
    d.show();
    app.exec();
}


int main( int argc, char *argv[] ) {
    #ifdef Q_WS_WIN
    #ifdef _MSC_VER
    MiniDumper(TEXT("QtLobby"));
    #else
    LoadLibrary(TEXT("exchndl.dll"));
    #endif
    #endif
    QApplication app( argc, argv );
    QStringList args = app.arguments();
    for(int i = 1; i < args.size(); i++) {
        if(args.at(i) == "--3dpreview" || args.at(i) == "-3d") {
            i++;
            cmd_3dpreview(app, args.at(i));
        }
    }
    if(!mainLoop) return 0;
    if ( !QSystemTrayIcon::isSystemTrayAvailable() ) {
        QMessageBox::critical( 0, QObject::tr( "Systray" ),
                               QObject::tr( "I couldn't detect any system tray "
                                            "on this system." ) );
    }
    MainWindow mw;
    mw.show();
    return app.exec();
}
