cd %cd%

@rem copy Debug_Public\YWSQLite.dll Debug\YWSQLite.dll
copy Debug_Public\pthreadVC2.dll Debug\pthreadVC2.dll
copy Debug_Public\LibWRISFramework.dll Debug\LibWRISFramework.dll
copy Debug_Public\BugReport.exe Debug\BugReport.exe
copy Debug_Public\WRISFramework.exe Debug\WRISFramework.exe

@rem copy Release_Public\YWSQLite.dll Release\YWSQLite.dll
copy Release_Public\pthreadVC2.dll Release\pthreadVC2.dll
copy Release_Public\LibWRISFramework.dll Release\LibWRISFramework.dll
copy Release_Public\BugReport.exe Release\BugReport.exe
copy Release_Public\WRISFramework.exe Release\WRISFramework.exe

pause