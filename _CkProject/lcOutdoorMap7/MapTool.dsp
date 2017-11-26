# Microsoft Developer Studio Project File - Name="MapTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MapTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapTool.mak" CFG="MapTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MapTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapTool - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"_Exec/MapTool.exe"

!ELSEIF  "$(CFG)" == "MapTool - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"_Exec/MapTool_.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MapTool - Win32 Release"
# Name "MapTool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MapTool.cpp
# End Source File
# Begin Source File

SOURCE=.\MapTool.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\_res\DirectX.ico
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_01.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_02.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_02.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_03.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_03.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_04.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_04.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_21.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_21.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_22.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_22.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_23.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_23.bmp
# End Source File
# Begin Source File

SOURCE=.\_D3D\res\ICON_24.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_25.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_26.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_27.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_28.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_29.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_30.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_31.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_32.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_33.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_34.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_35.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_36.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_37.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\ICON_38.bmp
# End Source File
# Begin Source File

SOURCE=.\_res\IDBWrk.bmp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\Main\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Main.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndFld.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndFld.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndFog.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndFog.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndLcl.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndLcl.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndLght.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndLght.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndLyr.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndLyr.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndMtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndMtrl.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndObj.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndTool.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndTool.h
# End Source File
# Begin Source File

SOURCE=.\Main\WndWrk.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WndWrk.h
# End Source File
# End Group
# Begin Group "LcxMap"

# PROP Default_Filter "cpp;h;"
# Begin Source File

SOURCE=.\LcxMap\ILcxmFld.cpp
# End Source File
# Begin Source File

SOURCE=.\LcxMap\ILcxmFld.h
# End Source File
# Begin Source File

SOURCE=.\LcxMap\ILcxmFldX.cpp
# End Source File
# Begin Source File

SOURCE=.\LcxMap\ILcxmFldX.h
# End Source File
# Begin Source File

SOURCE=.\LcxMap\LcxmBlc.cpp
# End Source File
# Begin Source File

SOURCE=.\LcxMap\LcxmBlc.h
# End Source File
# Begin Source File

SOURCE=.\LcxMap\LcxmDst.cpp
# End Source File
# Begin Source File

SOURCE=.\LcxMap\LcxmDst.h
# End Source File
# End Group
# End Target
# End Project
