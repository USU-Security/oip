# Microsoft Developer Studio Project File - Name="organicips" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=organicips - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "organicips.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "organicips.mak" CFG="organicips - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "organicips - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "organicips - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "organicips - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 freetype.lib SDL.lib SDL_image.lib cryptlib.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/oip.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "organicips - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "organicips___Win32_Release"
# PROP BASE Intermediate_Dir "organicips___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MT /w /W0 /GX /O2 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SDL.lib SDL_image.lib cryptlib.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/oip.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SDL.lib SDL_image.lib cryptlib.lib ws2_32.lib freetype.lib /nologo /subsystem:windows /machine:I386 /out:"Release/oip.exe"
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib

!ENDIF 

# Begin Target

# Name "organicips - Win32 Debug"
# Name "organicips - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\gui\button.cpp
# End Source File
# Begin Source File

SOURCE=..\chart.cpp
# End Source File
# Begin Source File

SOURCE=..\clientpm.cpp
# End Source File
# Begin Source File

SOURCE=..\config.cpp
# End Source File
# Begin Source File

SOURCE=..\encrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\entity.cpp
# End Source File
# Begin Source File

SOURCE=..\entityset.cpp
# End Source File
# Begin Source File

SOURCE=..\entitytest.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\font.cpp
# End Source File
# Begin Source File

SOURCE=..\image.cpp
# End Source File
# Begin Source File

SOURCE=..\kdtree.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\label.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\layout.cpp
# End Source File
# Begin Source File

SOURCE=..\messages.cpp
# End Source File
# Begin Source File

SOURCE=..\namecache.cpp
# End Source File
# Begin Source File

SOURCE=..\oipgui.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\option.cpp
# End Source File
# Begin Source File

SOURCE=..\packetmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\particle.cpp
# End Source File
# Begin Source File

SOURCE=..\particlemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\text.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\textbox.cpp
# End Source File
# Begin Source File

SOURCE=..\gui\widget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\gui\base.h
# End Source File
# Begin Source File

SOURCE=..\gui\button.h
# End Source File
# Begin Source File

SOURCE=..\chart.h
# End Source File
# Begin Source File

SOURCE=..\clientpm.h
# End Source File
# Begin Source File

SOURCE=..\config.h
# End Source File
# Begin Source File

SOURCE=..\gui\draw.h
# End Source File
# Begin Source File

SOURCE=..\encrypt.h
# End Source File
# Begin Source File

SOURCE=..\entity.h
# End Source File
# Begin Source File

SOURCE=..\entityset.h
# End Source File
# Begin Source File

SOURCE=..\gui\font.h
# End Source File
# Begin Source File

SOURCE=..\hexprint.h
# End Source File
# Begin Source File

SOURCE=..\image.h
# End Source File
# Begin Source File

SOURCE=..\ips.h
# End Source File
# Begin Source File

SOURCE=..\kdtree.h
# End Source File
# Begin Source File

SOURCE=..\gui\label.h
# End Source File
# Begin Source File

SOURCE=..\gui\layout.h
# End Source File
# Begin Source File

SOURCE=..\messages.h
# End Source File
# Begin Source File

SOURCE=..\namecache.h
# End Source File
# Begin Source File

SOURCE=..\gui\option.h
# End Source File
# Begin Source File

SOURCE=..\packetmanager.h
# End Source File
# Begin Source File

SOURCE=..\packetsink.h
# End Source File
# Begin Source File

SOURCE=..\particle.h
# End Source File
# Begin Source File

SOURCE=..\particlemanager.h
# End Source File
# Begin Source File

SOURCE=..\text.h
# End Source File
# Begin Source File

SOURCE=..\gui\textbox.h
# End Source File
# Begin Source File

SOURCE=..\gui\widget.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
