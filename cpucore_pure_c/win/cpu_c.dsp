# Microsoft Developer Studio Project File - Name="CPU_C" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CPU_C - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CPU_C.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CPU_C.mak" CFG="CPU_C - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CPU_C - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CPU_C - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP="E:\Program Files\Codeplay\vectorcl.exe"
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CPU_C - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir  ..\..\romloader\win\plugins\ 	mkdir  ..\..\romloader\win\plugins\cpu	xcopy  /y  Release\CPU_C.dll   ..\..\romloader\win\plugins\cpu
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CPU_C - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /debug /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir  ..\..\romloader\win\plugins\ 	mkdir  ..\..\romloader\win\plugins\cpu	xcopy  /y  Debug\CPU_C.dll   ..\..\romloader\win\plugins\cpu
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CPU_C - Win32 Release"
# Name "CPU_C - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\config.c
# End Source File
# Begin Source File

SOURCE=..\config_options.c
# End Source File
# Begin Source File

SOURCE=..\cop0_scp.c
# End Source File
# Begin Source File

SOURCE=..\cop1_fpu.c
# End Source File
# Begin Source File

SOURCE=.\CPU_C.def
# End Source File
# Begin Source File

SOURCE=..\debugger.c
# End Source File
# Begin Source File

SOURCE=..\disass.c
# End Source File
# Begin Source File

SOURCE=..\exec.c
# End Source File
# Begin Source File

SOURCE=..\interrupts.c
# End Source File
# Begin Source File

SOURCE=..\libcpu.c
# End Source File
# Begin Source File

SOURCE=..\memory.c
# End Source File
# Begin Source File

SOURCE=..\opstruct.c
# End Source File
# Begin Source File

SOURCE=..\patch.c
# End Source File
# Begin Source File

SOURCE=..\reg_names.c
# End Source File
# Begin Source File

SOURCE=..\savestate.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\config_options.h
# End Source File
# Begin Source File

SOURCE=..\debugger.h
# End Source File
# Begin Source File

SOURCE=..\..\include\decode.h
# End Source File
# Begin Source File

SOURCE=..\..\include\general.h
# End Source File
# Begin Source File

SOURCE=..\linking.h
# End Source File
# Begin Source File

SOURCE=..\..\include\linking.h
# End Source File
# Begin Source File

SOURCE=..\..\include\module.h
# End Source File
# Begin Source File

SOURCE=..\opcodes.h
# End Source File
# Begin Source File

SOURCE=..\registers.h
# End Source File
# Begin Source File

SOURCE=..\..\include\romheader.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
