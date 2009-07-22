# define name of installer
outFile "qtlobby_installer.exe"
 
# define installation directory
installDir $PROGRAMFILES\qtlobby

LicenseData LICENSE.rtf

SetCompressor /SOLID lzma
 
Page license
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles


# start default section
section
 
    # set the installation directory as the destination for the following actions
    setOutPath $INSTDIR
 
    # create the uninstaller
    writeUninstaller "$INSTDIR\uninstall.exe"
 
    # create a shortcut named "new shortcut" in the start menu programs directory
    # point the new shortcut at the program uninstaller
    createShortCut "$DESKTOP\qtlobby.lnk" "$INSTDIR\qtlobby.exe" "" "$INSTDIR\logo.ico"
    file release\qtlobby.exe
    file icons\logo.ico
    file C:\build\msvcp90.dll
    file C:\build\msvcr90.dll
    file C:\build\QtCore4.dll
    file C:\build\QtGui4.dll
    file C:\build\QtNetwork4.dll
    file C:\build\QtOpenGL4.dll
    file C:\build\QtXml4.dll
	file C:\build\QtScript4.dll
    file C:\build\qscintilla2.dll
sectionEnd
 
# uninstaller section start
section "uninstall"
 
    # first, delete the uninstaller
    delete "$INSTDIR\uninstall.exe"
 
    # second, remove the link from the start menu
    delete "$DESKTOP\qtlobby.lnk"
    delete $INSTDIR\qtlobby.exe
    delete $INSTDIR\logo.ico
    delete $INSTDIR\msvcp90.dll
    delete $INSTDIR\msvcr90.dll
    delete $INSTDIR\QtCore4.dll
    delete $INSTDIR\QtGui4.dll
    delete $INSTDIR\QtNetwork4.dll
    delete $INSTDIR\QtOpenGL4.dll
    delete $INSTDIR\QtXml4.dll
    delete $INSTDIR
 
# uninstaller section end
sectionEnd