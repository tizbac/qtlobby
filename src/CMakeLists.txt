include(${QT_USE_FILE})
include_directories(${QTSCRIPTTOOLS_INCLUDE_DIR})
include_directories(..)
# source files
set(qtlobby_SRCS
    #sources
    MainWindow.cpp 
    ServerContextState.cpp 
    StatusTracker.cpp 
    NetworkInterface.cpp 
    Command.cpp 
    AbstractStateClient.cpp 
    CommandAssigner.cpp 
    ConnectionWidget.cpp 
    Channel.cpp 
    InfoChannel.cpp 
    LobbyTabs.cpp 
    AbstractLobbyTab.cpp 
    User.cpp 
    Users.cpp 
    Settings.cpp 
    AgreementWidget.cpp 
    InputLine.cpp 
    Battle.cpp 
    UserTreeModel.cpp 
    PrivateChannel.cpp 
    AbstractChannel.cpp 
    UserManager.cpp 
    BattleChannel.cpp 
    BattleManager.cpp 
    Battles.cpp 
    BattleTreeModel.cpp 
    ServerConfig.cpp 
    TLDList.cpp 
    UnitSyncLib.cpp 
    AudioBackend.cpp 
    Preference.cpp 
    BattlePasswordWidget.cpp  # MapSelector.cpp  # not used
    BattleUserTreeModel.cpp 
    TreeSortFilterProxyModel.cpp  # MapElementWidget.cpp  # not used
    PreferencePathElement.cpp 
    Singleton.cpp 
    ModOption.cpp 
    StylesheetDialog.cpp 
    ChannelTextBrowser.cpp 
    MapWidget.cpp 
    RawHeightMap.cpp 
    MapInfoLoader.cpp 
    CompletionListWidget.cpp 
    MapOverviewDialog.cpp 
    MapRendererWidget.cpp 
    GLProgressDialog.cpp 
    UserGroup.cpp 
    UserGroupsDialog.cpp 
    glextensions.cpp 
    ScriptingDialog.cpp 
    BattleHost.cpp 
    sqads/sqadsprototypes.cpp 
    BattleHostingDialog.cpp 
    Notification.cpp 
    TrayIconNotificationBackend.cpp 
    ProgressBarItemDelegate.cpp 
    Downloader.cpp 
    DownloadsDialog.cpp 
    DownloadsModel.cpp 
    ToolBarWidget.cpp
    DBusVisualNotificationBackend.cpp
)
set(qtlobby_SRCS_H 
    MainWindow.h 
    ServerContextState.h 
    StatusTracker.h 
    NetworkInterface.h 
    Command.h 
    AbstractStateClient.h 
    CommandAssigner.h 
    ConnectionWidget.h 
    Channel.h 
    InfoChannel.h 
    LobbyTabs.h 
    AbstractLobbyTab.h 
    User.h 
    Users.h 
    Settings.h
    AgreementWidget.h
    InputLine.h
    Battle.h 
    PrivateChannel.h 
    AbstractChannel.h 
    UserTreeModel.h 
    UserManager.h 
    BattleChannel.h 
    BattleManager.h 
    Battles.h 
    BattleTreeModel.h 
    ServerConfig.h 
    TLDList.h 
    UnitSyncLib.h 
    AudioBackend.h 
    Preference.h 
    BattlePasswordWidget.h  # MapSelector.h  # not used
    BattleUserTreeModel.h 
    TreeSortFilterProxyModel.h  # MapElementWidget.h  # not used
    PreferencePathElement.h 
    Singleton.h 
    ModOption.h 
    StylesheetDialog.h 
    ChannelTextBrowser.h 
    MapWidget.h 
    RawHeightMap.h 
    MapInfoLoader.h 
    CompletionListWidget.h 
    MapOverviewDialog.h 
    GLProgressDialog.h 
    UserGroup.h 
    UserGroupsDialog.h 
    glextensions.h 
    ScriptingDialog.h 
    BattleHost.h 
    sqads/sqadsprototypes.h 
    BattleHostingDialog.h 
    Notification.h 
    AbstractNotificationBackend.h 
    TrayIconNotificationBackend.h 
    ProgressBarItemDelegate.h 
    Downloader.h 
    DownloadsDialog.h 
    DownloadsModel.h 
    ToolBarWidget.h    
)
set( qtlobby_SRCS_HH
MapRendererWidget.h 
)
QT4_WRAP_CPP(qtlobby_SRCS_MOC ${qtlobby_SRCS_H})

# UI files
set( qtlobby_UIS
../ui/AboutWidget.ui
../ui/AbstractChannelWidget.ui
../ui/AgreementWidget.ui
../ui/BattleHostingDialog.ui
../ui/BattleWindowForm.ui
../ui/ConnectionWidget.ui
../ui/DownloadsDialog.ui
../ui/GLProgressDialog.ui
../ui/BattlePasswordWidget.ui
../ui/LoginDialog.ui
../ui/MapOverviewDialog.ui
../ui/PreferencePathElement.ui
../ui/ScriptingDialog.ui
../ui/ToolBarWidget.ui
../ui/GLProgressDialog.ui
../ui/MainWidget.ui
../ui/MapSelectorWidget.ui
../ui/PreferenceWidget.ui
../ui/StylesheetDialog.ui
../ui/UserGroupsDialog.ui
)
# TODO: the wrapped ui_*.h files contain src/*.h includes, that could not be found, they should be just ./*.h
QT4_WRAP_UI(qtlobby_UIS_H ${qtlobby_UIS})
include_directories(../ui)

QT4_ADD_RESOURCES(qtlobby_RES ../resources.qrc)

QT4_ADD_TRANSLATION(qtlobby_TRANS ../i18n/qtlobby_de.ts)

# linking compiling
add_executable(qtlobby ${qtlobby_SRCS} ${qtlobby_SRCS_HH} ${qtlobby_UIS_H} ${qtlobby_SRCS_MOC} ${qtlobby_RES} ${qtlobby_TRANS} main.cpp)

# TODO: remove hard coded qt script tools path
target_link_libraries(qtlobby ${QT_LIBRARIES} ${QSCINTILLA_LIBRARIES} ${QTSCRIPTTOOLS_LIBRARIES})

#install
install(TARGETS qtlobby DESTINATION bin)

