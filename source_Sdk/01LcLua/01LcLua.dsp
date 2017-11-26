# Microsoft Developer Studio Project File - Name="01LcLua" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=01LcLua - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "01LcLua.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "01LcLua.mak" CFG="01LcLua - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "01LcLua - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "01LcLua - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "01LcLua - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\..\lib\Lua.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\include\Lua	copy lua.h ..\..\include\Lua	copy lualib.h ..\..\include\Lua	copy lauxlib.h ..\..\include\Lua
# End Special Build Tool

!ELSEIF  "$(CFG)" == "01LcLua - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\lib\Lua_.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\..\include\Lua	copy lua.h ..\..\include\Lua	copy lualib.h ..\..\include\Lua	copy lauxlib.h ..\..\include\Lua
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "01LcLua - Win32 Release"
# Name "01LcLua - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\lapi.cpp
# End Source File
# Begin Source File

SOURCE=.\lauxlib.cpp
# End Source File
# Begin Source File

SOURCE=.\lbaselib.cpp
# End Source File
# Begin Source File

SOURCE=.\lcode.cpp
# End Source File
# Begin Source File

SOURCE=.\ldbg.cpp
# End Source File
# Begin Source File

SOURCE=.\ldblib.cpp
# End Source File
# Begin Source File

SOURCE=.\ldo.cpp
# End Source File
# Begin Source File

SOURCE=.\ldump.cpp
# End Source File
# Begin Source File

SOURCE=.\lfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\lgc.cpp
# End Source File
# Begin Source File

SOURCE=.\liolib.cpp
# End Source File
# Begin Source File

SOURCE=.\llex.cpp
# End Source File
# Begin Source File

SOURCE=.\lmathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\lmem.cpp
# End Source File
# Begin Source File

SOURCE=.\loadlib.cpp
# End Source File
# Begin Source File

SOURCE=.\lobject.cpp
# End Source File
# Begin Source File

SOURCE=.\lopcodes.cpp
# End Source File
# Begin Source File

SOURCE=.\lparser.cpp
# End Source File
# Begin Source File

SOURCE=.\lstate.cpp
# End Source File
# Begin Source File

SOURCE=.\lstring.cpp
# End Source File
# Begin Source File

SOURCE=.\lstrlib.cpp
# End Source File
# Begin Source File

SOURCE=.\ltable.cpp
# End Source File
# Begin Source File

SOURCE=.\ltablib.cpp
# End Source File
# Begin Source File

SOURCE=.\ltests.cpp
# End Source File
# Begin Source File

SOURCE=.\ltm.cpp
# End Source File
# Begin Source File

SOURCE=.\lundump.cpp
# End Source File
# Begin Source File

SOURCE=.\lvm.cpp
# End Source File
# Begin Source File

SOURCE=.\lzio.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\lapi.h
# End Source File
# Begin Source File

SOURCE=.\lauxlib.h
# End Source File
# Begin Source File

SOURCE=.\lcode.h
# End Source File
# Begin Source File

SOURCE=.\ldbg.h
# End Source File
# Begin Source File

SOURCE=.\ldo.h
# End Source File
# Begin Source File

SOURCE=.\lfunc.h
# End Source File
# Begin Source File

SOURCE=.\lgc.h
# End Source File
# Begin Source File

SOURCE=.\llex.h
# End Source File
# Begin Source File

SOURCE=.\llimits.h
# End Source File
# Begin Source File

SOURCE=.\lmem.h
# End Source File
# Begin Source File

SOURCE=.\lobject.h
# End Source File
# Begin Source File

SOURCE=.\lopcodes.h
# End Source File
# Begin Source File

SOURCE=.\lparser.h
# End Source File
# Begin Source File

SOURCE=.\lstate.h
# End Source File
# Begin Source File

SOURCE=.\lstring.h
# End Source File
# Begin Source File

SOURCE=.\ltable.h
# End Source File
# Begin Source File

SOURCE=.\ltm.h
# End Source File
# Begin Source File

SOURCE=.\lua.h
# End Source File
# Begin Source File

SOURCE=.\lualib.h
# End Source File
# Begin Source File

SOURCE=.\lundump.h
# End Source File
# Begin Source File

SOURCE=.\lvm.h
# End Source File
# Begin Source File

SOURCE=.\lzio.h
# End Source File
# End Group
# End Target
# End Project
