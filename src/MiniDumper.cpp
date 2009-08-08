#include "MiniDumper.h"

TCHAR* MiniDumper::m_szAppName;

MiniDumper::MiniDumper( TCHAR* szAppName )
{
    if( m_szAppName!=NULL )
        return;

    m_szAppName = szAppName ? _wcsdup(szAppName) : TEXT("Application");

    ::SetUnhandledExceptionFilter( TopLevelFilter );
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
    LONG retval = EXCEPTION_CONTINUE_SEARCH;

    // firstly see if dbghelp.dll is around and has the function we need
    // look next to the EXE first, as the one in System32 might be old 
    // (e.g. Windows 2000)
    HMODULE hDll = NULL;
    hDll = ::LoadLibrary( TEXT("DBGHELP.DLL") );
    LPCTSTR szResult = NULL;

    if (hDll){
        MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
        if (pDump){
            TCHAR szDumpPath[_MAX_PATH];
            TCHAR szScratch [_MAX_PATH];
            wcscpy( szDumpPath, TEXT(".\\") );
            wcscat( szDumpPath, m_szAppName );
            wcscat( szDumpPath, TEXT(".dmp") );

            ::MessageBox( NULL, TEXT("Application has crashed.\n Press 'OK' to generate dump file(this may take a while).\nPlease report .dmg file to our tracker(http://code.google.com/p/qtlobby/issues/entry) or at #qtlobby"), m_szAppName, MB_OK );
            
            // create the file
            HANDLE hFile = ::CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                                            FILE_ATTRIBUTE_NORMAL, NULL );

            if (hFile!=INVALID_HANDLE_VALUE){
                _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

                ExInfo.ThreadId = ::GetCurrentThreadId();
                ExInfo.ExceptionPointers = pExceptionInfo;
                ExInfo.ClientPointers = NULL;

                // write the dump
                MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | MiniDumpScanMemory);
                BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, &ExInfo, NULL, NULL );
                if (bOK) {
                    swprintf( szScratch, TEXT("Saved dump file to '%s'"), szDumpPath );
                    szResult = szScratch;
                    retval = EXCEPTION_EXECUTE_HANDLER;
                } else {
                    swprintf( szScratch, TEXT("Failed to save dump file to '%s' (error %d)"), szDumpPath, GetLastError() );
                    szResult = szScratch;
                }
                ::CloseHandle(hFile);
            } else {
                swprintf( szScratch, TEXT("Failed to create dump file '%s' (error %d)"), szDumpPath, GetLastError() );
                szResult = szScratch;
            }
        }
    } else {
            szResult = TEXT("DBGHELP.DLL not found");
    }
    if (szResult)
        ::MessageBox( NULL, szResult, m_szAppName, MB_OK );
    return retval;
}
