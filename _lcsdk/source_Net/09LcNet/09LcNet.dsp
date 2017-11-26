# Microsoft Developer Studio Project File - Name="09LcNet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=09LcNet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "09LcNet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "09LcNet.mak" CFG="09LcNet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "09LcNet - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "09LcNet - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "09LcNet - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\LcNet.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\include\LcNet	copy ILcNet.h ..\..\include\LcNet	copy LcNetUtil.h ..\..\include\LcNet
# End Special Build Tool

!ELSEIF  "$(CFG)" == "09LcNet - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\LcNet_.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\include\LcNet	copy ILcNet.h ..\..\include\LcNet	copy LcNetUtil.h ..\..\include\LcNet
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "09LcNet - Win32 Release"
# Name "09LcNet - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ILcNet.cpp
# End Source File
# Begin Source File

SOURCE=.\LcNet.cpp
# End Source File
# Begin Source File

SOURCE=.\LcNetIocp.cpp
# End Source File
# Begin Source File

SOURCE=.\LcNetSlct.cpp
# End Source File
# Begin Source File

SOURCE=.\LcNetSlctA.cpp
# End Source File
# Begin Source File

SOURCE=.\LcNetSlctE.cpp
# End Source File
# Begin Source File

SOURCE=.\LcNetUtil.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ILcNet.h
# End Source File
# Begin Source File

SOURCE=.\LcNet.h
# End Source File
# Begin Source File

SOURCE=.\LcNetIocp.h
# End Source File
# Begin Source File

SOURCE=.\LcNetSlct.h
# End Source File
# Begin Source File

SOURCE=.\LcNetSlctA.h
# End Source File
# Begin Source File

SOURCE=.\LcNetSlctE.h
# End Source File
# Begin Source File

SOURCE=.\LcNetUtil.h
# End Source File
# End Group
# End Target
# End Project
