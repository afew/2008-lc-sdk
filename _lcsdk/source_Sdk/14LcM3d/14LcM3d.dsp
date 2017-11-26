# Microsoft Developer Studio Project File - Name="14LcM3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=14LcM3d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "14LcM3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "14LcM3d.mak" CFG="14LcM3d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "14LcM3d - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "14LcM3d - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "14LcM3d - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\..\lib\LcM3d.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\include\Lc	copy ILcM3d.h ..\..\include\Lc	copy ILcXmsh.h ..\..\include\Lc
# End Special Build Tool

!ELSEIF  "$(CFG)" == "14LcM3d - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\lib\LcM3d_.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\include\Lc	copy ILcM3d.h ..\..\include\Lc	copy ILcXmsh.h ..\..\include\Lc
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "14LcM3d - Win32 Release"
# Name "14LcM3d - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ILcM3d.cpp
# End Source File
# Begin Source File

SOURCE=.\ILcXmsh.cpp
# End Source File
# Begin Source File

SOURCE=.\LcIdx.cpp
# End Source File
# Begin Source File

SOURCE=.\LcLgt.cpp
# End Source File
# Begin Source File

SOURCE=.\LcMtl.cpp
# End Source File
# Begin Source File

SOURCE=.\LcVtx.cpp
# End Source File
# Begin Source File

SOURCE=.\LcXmsh.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ILcM3d.h
# End Source File
# Begin Source File

SOURCE=.\ILcXmsh.h
# End Source File
# Begin Source File

SOURCE=.\LcIdx.h
# End Source File
# Begin Source File

SOURCE=.\LcLgt.h
# End Source File
# Begin Source File

SOURCE=.\LcMtl.h
# End Source File
# Begin Source File

SOURCE=.\LcVtx.h
# End Source File
# Begin Source File

SOURCE=.\LcXmsh.h
# End Source File
# End Group
# End Target
# End Project
