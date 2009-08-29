# $Id$
TEMPLATE = app
TARGET = qtlobby
OBJECTS_DIR = debug
MOC_DIR = debug
OBJMOC = debug
DESTDIR = debug
DESTDIR_TARGET = qtlobby
SRCMOC = debug
QMAKE_CLEAN = debug/*
DEPENDPATH += .
INCLUDEPATH += . \
    src/sqads
RESOURCES = resources.qrc
UI_HEADERS_DIR = src
SVN_REVISION = $$(SVN_REVISION)
!isEmpty(SVN_REVISION):DEFINES += 'SVN_REV=\\"$$(SVN_REVISION)\\"'
else { 
    unix:DEFINES += 'SVN_REV=\\"$(shell svnversion -n .)\\"'
    else:DEFINES += 'SVN_REV=\\"$$system(svnversion -n .)\\"'
}
DEFINES += RPM_OPT_FLAGS
target.path += $$INSTALL_ROOT/bin
INSTALLS += target

# QtLobby version
DEFINES += 'QTLOBBY_VERSION=\\"1.0alpha\\"'
desktop.files = src/qtlobby.desktop
desktop.path = $$INSTALL_ROOT/share/applications
INSTALLS += desktop
desktopicon.files = icons/qtlobby-logo.svg
desktopicon.path = $$INSTALL_ROOT/share/pixmaps
INSTALLS += desktopicon
contains( CONFIG, buildbot ) { 
    QMAKE_LFLAGS += -static-libgcc \
        -Wl,-subsystem,windows
    QMAKE_CXXFLAGS += -g
    
    # ([]lennart) for testing the bug when trying to load unitsync library with qt.
    # this sets workaround (use of winapi) as default for buildbot.
    CONFIG += unitsync_winapi
}

# SVNDEF := -DSVN_REV=444
# DEFINES += SVN_REV=444
# QMAKE_CFLAGS += $(SVNDEF)
# QMAKE_CXXFLAGS += $(SVNDEF)
# Input
HEADERS += src/MainWindow.h \
    src/ServerContextState.h \
    src/StatusTracker.h \
    src/NetworkInterface.h \
    src/Command.h \
    src/AbstractStateClient.h \
    src/CommandAssigner.h \
    src/ConnectionWidget.h \
    src/Channel.h \
    src/InfoChannel.h \
    src/LobbyTabs.h \
    src/AbstractLobbyTab.h \
    src/User.h \
    src/Users.h \
    src/Settings.h \
    src/AgreementWidget.h \
    src/InputLine.h \
    src/Battle.h \
    src/PrivateChannel.h \
    src/AbstractChannel.h \
    src/UserTreeModel.h \
    src/UserManager.h \
    src/BattleChannel.h \
    src/BattleManager.h \
    src/Battles.h \
    src/BattleTreeModel.h \
    src/ServerConfig.h \
    src/TLDList.h \
    src/UnitSyncLib.h \
    src/AudioBackend.h \
    src/Preference.h \
    src/BattlePasswordWidget.h \
    src/BattleCloseFirstWidget.h \
    src/BattleUserTreeModel.h \
    src/TreeSortFilterProxyModel.h \
    src/PreferencePathElement.h \
    src/Singleton.h \
    src/ModOption.h \
    src/StylesheetDialog.h \
    src/ChannelTextBrowser.h \
    src/MapWidget.h \
    src/RawHeightMap.h \
    src/MapInfoLoader.h \
    src/CompletionListWidget.h \
    src/MapOverviewDialog.h \
    src/MapRendererWidget.h \
    src/GLProgressDialog.h \
    src/UserGroup.h \
    src/UserGroupsDialog.h \
    src/glextensions.h \
    src/ScriptingDialog.h \
    src/BattleHost.h \
    src/sqads/sqadsprototypes.h \
    src/BattleHostingDialog.h \
    src/Notification.h \
    src/AbstractNotificationBackend.h \
    src/TrayIconNotificationBackend.h \
    src/ProgressBarItemDelegate.h \
    src/Downloader.h \
    src/DownloadsDialog.h \
    src/DownloadsModel.h \
    src/ToolBarWidget.h \
    src/ServerProfilesModel.h \
    src/UserMenuBuilder.h \
    src/UsersTreeView.h \
    src/ShaderSet.h \
    src/GLMaterial.h \
    src/GLObject.h \
    src/GLHeightMap.h \
    src/GLWaterPlane.h

# src/MapSelector.h \ # not used
# src/MapElementWidget.h \ # not used
SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/ServerContextState.cpp \
    src/StatusTracker.cpp \
    src/NetworkInterface.cpp \
    src/Command.cpp \
    src/AbstractStateClient.cpp \
    src/CommandAssigner.cpp \
    src/ConnectionWidget.cpp \
    src/Channel.cpp \
    src/InfoChannel.cpp \
    src/LobbyTabs.cpp \
    src/AbstractLobbyTab.cpp \
    src/User.cpp \
    src/Users.cpp \
    src/Settings.cpp \
    src/AgreementWidget.cpp \
    src/InputLine.cpp \
    src/Battle.cpp \
    src/UserTreeModel.cpp \
    src/PrivateChannel.cpp \
    src/AbstractChannel.cpp \
    src/UserManager.cpp \
    src/BattleChannel.cpp \
    src/BattleManager.cpp \
    src/Battles.cpp \
    src/BattleTreeModel.cpp \
    src/ServerConfig.cpp \
    src/TLDList.cpp \
    src/UnitSyncLib.cpp \
    src/AudioBackend.cpp \
    src/Preference.cpp \
    src/BattlePasswordWidget.cpp \
    src/BattleCloseFirstWidget.cpp \
    src/BattleUserTreeModel.cpp \
    src/TreeSortFilterProxyModel.cpp \
    src/PreferencePathElement.cpp \
    src/Singleton.cpp \
    src/ModOption.cpp \
    src/StylesheetDialog.cpp \
    src/ChannelTextBrowser.cpp \
    src/MapWidget.cpp \
    src/RawHeightMap.cpp \
    src/MapInfoLoader.cpp \
    src/CompletionListWidget.cpp \
    src/MapOverviewDialog.cpp \
    src/MapRendererWidget.cpp \
    src/GLProgressDialog.cpp \
    src/UserGroup.cpp \
    src/UserGroupsDialog.cpp \
    src/glextensions.cpp \
    src/ScriptingDialog.cpp \
    src/BattleHost.cpp \
    src/sqads/sqadsprototypes.cpp \
    src/BattleHostingDialog.cpp \
    src/Notification.cpp \
    src/TrayIconNotificationBackend.cpp \
    src/ProgressBarItemDelegate.cpp \
    src/Downloader.cpp \
    src/DownloadsDialog.cpp \
    src/DownloadsModel.cpp \
    src/ToolBarWidget.cpp \
    src/ServerProfilesModel.cpp \
    src/UserMenuBuilder.cpp \
    src/UsersTreeView.cpp \
    src/ShaderSet.cpp \
    src/GLMaterial.cpp \
    src/GLObject.cpp \
    src/GLHeightMap.cpp \
    src/GLWaterPlane.cpp

# src/MapSelector.cpp \ # not used
# src/MapElementWidget.cpp \ # not used
QT += gui \
    network \
    opengl \
    xml \
    script \
    scripttools
FORMS += ui/MainWidget.ui \
    ui/ConnectionWidget.ui \
    ui/BattleWindowForm.ui \
    ui/BattlePasswordWidget.ui \
    ui/BattleCloseFirstWidget.ui \
    ui/AboutWidget.ui \
    ui/AgreementWidget.ui \
    ui/PreferenceWidget.ui \
    ui/PreferencePathElement.ui \
    ui/StylesheetDialog.ui \
    ui/MapOverviewDialog.ui \
    ui/GLProgressDialog.ui \
    ui/LoginDialog.ui \
    ui/UserGroupsDialog.ui \
    ui/ScriptingDialog.ui \
    ui/BattleHostingDialog.ui \
    ui/DownloadsDialog.ui \
    ui/ToolBarWidget.ui

# ui/MapSelectorWidget.ui \ # not used
DISTFILES += doc/ProtocolDescription.xml \
    TODO \
    doc/xml2html.xsl \
    doc/z
contains( CONFIG, vc ) { 
    HEADERS += src/MiniDumper.h
    SOURCES += src/MiniDumper.cpp
    LIBS += QScintilla2.lib
}
else:LIBS += -lqscintilla2

# ([]lennart)sets preprocessor switch to use direct winapi calls instead of qt
# to load unitsync library
contains( CONFIG, unitsync_winapi ): DEFINES += PURE_WINAPI_UNITSYNC_LOADER
unix:!contains( CONFIG, buildbot ) { 
    HEADERS += src/DBusVisualNotificationBackend.h
    SOURCES += src/DBusVisualNotificationBackend.cpp
    QT += dbus
}
RC_FILE += src/qtlobby.rc
OTHER_FILES += src/sqads.js
TRANSLATIONS = i18n/qtlobby_de.ts \
    i18n/qtlobby_es.ts
