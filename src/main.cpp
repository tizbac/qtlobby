// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include "MapInfoLoader.h"
#include "MapOverviewDialog.h"
#include "PathManager.h"

#ifdef Q_WS_WIN
#include <windows.h>
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
    LoadLibrary(TEXT("exchndl.dll"));
#endif
    QApplication app( argc, argv );
    QStringList args = app.arguments();
    QStringList list = args.at(0).split(QDir::separator());
    list.removeLast();
#ifndef Q_WS_WIN
    list.removeLast();//windows does not have bin folder, so no need to get one level higher
#else
    QCoreApplication::addLibraryPath(list.join(QDir::separator())+QDir::separator()+"qt");
#endif
    list.append("share");
    list.append("qtlobby");
    PathManager::getInstance()->setResourceRoot(list.join(QDir::separator())+QDir::separator());
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
    MainWindow::getInstance()->show();
    return app.exec();
}
