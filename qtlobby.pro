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
DEFINES += 'SVN_REV=\\"$$system(svnversion -n .)\\"'
DEFINES += RPM_OPT_FLAGS
target.path += $$INSTALL_ROOT/bin
INSTALLS += target
desktop.files = src/qtlobby.desktop
desktop.path = $$INSTALL_ROOT/share/applications
INSTALLS += desktop

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
    src/GamePasswordWidget.h \
#    src/MapSelector.h \               # not used
    src/BattleUserTreeModel.h \
    src/TreeSortFilterProxyModel.h \
#    src/MapElementWidget.h \          # not used
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
    src/ToolBarWidget.h
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
    src/GamePasswordWidget.cpp \
#    src/MapSelector.cpp \             # not used
    src/BattleUserTreeModel.cpp \
    src/TreeSortFilterProxyModel.cpp \
#    src/MapElementWidget.cpp \        # not used
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
    src/ToolBarWidget.cpp
QT += gui \
    network \
    opengl \
    xml \
    script \
    scripttools
FORMS += ui/MainWidget.ui \
    ui/ConnectionWidget.ui \
#    ui/MapSelectorWidget.ui \         # not used
    ui/BattleWindowForm.ui \
    ui/GamePasswordWidget.ui \
    ui/AboutWidget.ui \
    ui/AgreementWidget.ui \
#    ui/AbstractChannelWidget.ui \     # not used
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
DISTFILES += doc/ProtocolDescription.xml \
    TODO \
    doc/xml2html.xsl \
    doc/z
win32:LIBS += QScintilla2.lib
unix { 
    LIBS += -lqscintilla2
    HEADERS += src/DBusVisualNotificationBackend.h
    SOURCES += src/DBusVisualNotificationBackend.cpp
    QT += dbus
}
OTHER_FILES += src/sqads.js
TRANSLATIONS = qtlobby.ts
