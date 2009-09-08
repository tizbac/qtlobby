include(${QT_USE_FILE})
include_directories(${QTSCRIPTTOOLS_INCLUDE_DIR})
include_directories(..)

# source files
set(qtlobby_SRCS
    #sources
AbstractChannel.cpp
AbstractLobbyTab.cpp
AbstractStateClient.cpp
AgreementWidget.cpp    
AudioBackend.cpp       
BattleChannel.cpp      
BattleCloseFirstWidget.cpp
Battle.cpp                
BattleHost.cpp            
BattleHostingDialog.cpp   
BattleManager.cpp         
BattlePasswordWidget.cpp  
Battles.cpp               
BattleTreeModel.cpp       
BattleUserTreeModel.cpp   
Channel.cpp               
ChannelTextBrowser.cpp    
CommandAssigner.cpp       
Command.cpp               
CompletionListWidget.cpp  
ConnectionWidget.cpp      
Downloader.cpp                   
DownloadsDialog.cpp              
DownloadsModel.cpp               
glextensions.cpp                 
GLHeightMap.cpp                  
GLMaterial.cpp                   
GLObject.cpp                     
GLProgressDialog.cpp             
GLWaterPlane.cpp                 
InfoChannel.cpp                  
InputLine.cpp                    
LobbyTabs.cpp                               
MainWindow.cpp                   
MapElementWidget.cpp             
MapInfoLoader.cpp                
MapOverviewDialog.cpp            
MapRendererWidget.cpp            
MapSelector.cpp                  
MapWidget.cpp                    
ModOption.cpp                    
NetworkInterface.cpp             
Notification.cpp                 
#PathManager.cpp                  
Preference.cpp                   
PreferencePathElement.cpp        
PrivateChannel.cpp               
ProgressBarItemDelegate.cpp      
RawHeightMap.cpp
ScriptingDialog.cpp
ServerConfig.cpp
ServerContextState.cpp
ServerProfilesModel.cpp
Settings.cpp
ShaderSet.cpp
Singleton.cpp
StatusTracker.cpp
StylesheetDialog.cpp
TLDList.cpp
ToolBarWidget.cpp
TrayIconNotificationBackend.cpp
TreeSortFilterProxyModel.cpp
UnitSyncLib.cpp
User.cpp
UserGroup.cpp
UserGroupsDialog.cpp
UserManager.cpp
UserMenuBuilder.cpp
Users.cpp
UsersTreeView.cpp
UserTreeModel.cpp
sqadsprototypes.cpp
)

set(qtlobby_SRCS_H 
AbstractChannel.h
AbstractLobbyTab.h
AbstractNotificationBackend.h
AbstractStateClient.h        
AgreementWidget.h            
AudioBackend.h               
BattleChannel.h              
BattleCloseFirstWidget.h     
Battle.h                     
BattleHost.h                 
BattleHostingDialog.h        
BattleManager.h              
BattlePasswordWidget.h       
Battles.h                    
BattleTreeModel.h            
BattleUserTreeModel.h        
Channel.h                    
ChannelTextBrowser.h         
CommandAssigner.h            
Command.h                    
CompletionListWidget.h       
config.h                     
ConnectionWidget.h           
Downloader.h                   
DownloadsDialog.h              
DownloadsModel.h               
glextensions.h                 
GLHeightMap.h                  
GLMaterial.h                   
GLObject.h                     
GLProgressDialog.h             
GLWaterPlane.h                 
InfoChannel.h                  
InputLine.h                    
LobbyTabs.h                    
MainWindow.h                   
MapElementWidget.h             
MapInfoLoader.h                
MapOverviewDialog.h            
MapRendererWidget.h            
MapSelector.h                  
MapWidget.h                    
ModOption.h                    
NetworkInterface.h             
Notification.h                 
#PathManager.h                  
Preference.h                   
PreferencePathElement.h        
PrivateChannel.h               
ProgressBarItemDelegate.h      
RawHeightMap.h                 
ScriptingDialog.h              
ServerConfig.h                 
ServerContextState.h           
ServerProfilesModel.h          
Settings.h                     
ShaderSet.h                    
Singleton.h                    
StatusTracker.h                
StylesheetDialog.h             
TLDList.h                      
ToolBarWidget.h                
TrayIconNotificationBackend.h  
TreeSortFilterProxyModel.h
UnitSyncLib.h
UserGroup.h
UserGroupsDialog.h
User.h
UserManager.h
UserMenuBuilder.h
Users.h
UsersTreeView.h
UserTreeModel.h
sqadsprototypes.h
)     

if(WIN32 OR BUILDBOT)
set(qtlobby_SRCS ${qtlobby_SRCS} DBusVisualNotificationBackend.cpp)
set(qtlobby_SRCS_H ${qtlobby_SRCS_H} DBusVisualNotificationBackend.h)
endif(WIN32 OR BUILDBOT)

QT4_WRAP_CPP(qtlobby_SRCS_MOC ${qtlobby_SRCS_H})
      
# UI files
set( qtlobby_UIS
../ui/AboutWidget.ui
../ui/AgreementWidget.ui
../ui/BattleCloseFirstWidget.ui
../ui/BattleHostingDialog.ui
../ui/BattlePasswordWidget.ui
../ui/BattleWindowForm.ui
../ui/ConnectionWidget.ui
../ui/DownloadsDialog.ui
../ui/GLProgressDialog.ui
../ui/LoginDialog.ui
../ui/MainWidget.ui
../ui/MapOverviewDialog.ui
../ui/MapSelectorWidget.ui
../ui/PreferencePathElement.ui
../ui/PreferenceWidget.ui
../ui/ScriptingDialog.ui
../ui/StylesheetDialog.ui
../ui/ToolBarWidget.ui
../ui/UserGroupsDialog.ui
)     
QT4_WRAP_UI(qtlobby_UIS_H ${qtlobby_UIS})
include_directories(../ui)
      
QT4_ADD_RESOURCES(qtlobby_RES ../resources.qrc)
      
QT4_ADD_TRANSLATION(qtlobby_TRANS
../i18n/qtlobby_de.ts
../i18n/qtlobby_es.ts
../i18n/qtlobby_fi.ts
../i18n/qtlobby_ru.ts
)     
      
# linking compiling
add_executable(qtlobby ${qtlobby_SRCS} ${qtlobby_SRCS_HH} ${qtlobby_UIS_H} ${qtlobby_SRCS_MOC} ${qtlobby_RES} ${qtlobby_TRANS} main.cpp)

if(BUILDBOT)
SET_TARGET_PROPERTIES(qtlobby PROPERTIES LINK_FLAGS -static-libgcc -Wl,-subsystem,windows)
endif(BUILDBOT)

target_link_libraries(qtlobby ${QT_LIBRARIES} ${QSCINTILLA_LIBRARIES} ${QTSCRIPTTOOLS_LIBRARIES})

#install
install(TARGETS qtlobby DESTINATION bin)

