# Microsoft Developer Studio Project File - Name="MaskTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MaskTools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MaskTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MaskTools.mak" CFG="MaskTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MaskTools - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MaskTools - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MaskTools - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MaskTools___Win32_Release"
# PROP BASE Intermediate_Dir "MaskTools___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASKTOOLS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Zp16 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASKTOOLS_EXPORTS" /D "USE_INFL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"F:\Plugins\MaskTools.dll"

!ELSEIF  "$(CFG)" == "MaskTools - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MaskTools___Win32_Debug"
# PROP BASE Intermediate_Dir "MaskTools___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASKTOOLS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASKTOOLS_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"F:\Plugins\MaskTools.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MaskTools - Win32 Release"
# Name "MaskTools - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "MaskedMerge"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MaskedMerge.cpp
# End Source File
# Begin Source File

SOURCE=.\MaskedMerge.h
# End Source File
# Begin Source File

SOURCE=.\MM_func.cpp
# End Source File
# Begin Source File

SOURCE=.\MM_func.h
# End Source File
# End Group
# Begin Group "Invert"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inv_func.cpp
# End Source File
# Begin Source File

SOURCE=.\Inv_func.h
# End Source File
# Begin Source File

SOURCE=.\Invert.cpp
# End Source File
# Begin Source File

SOURCE=.\Invert.h
# End Source File
# End Group
# Begin Group "EdgeMask"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EdgeMask.cpp
# End Source File
# Begin Source File

SOURCE=.\EdgeMask.h
# End Source File
# Begin Source File

SOURCE=.\EM_func.cpp
# End Source File
# Begin Source File

SOURCE=.\EM_func.h
# End Source File
# End Group
# Begin Group "Binarize"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bin_func.cpp
# End Source File
# Begin Source File

SOURCE=.\Bin_func.h
# End Source File
# Begin Source File

SOURCE=.\Binarize.cpp
# End Source File
# Begin Source File

SOURCE=.\Binarize.h
# End Source File
# End Group
# Begin Group "Expand"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EXP_func.cpp
# End Source File
# Begin Source File

SOURCE=.\EXP_func.h
# End Source File
# Begin Source File

SOURCE=.\Expand.cpp
# End Source File
# Begin Source File

SOURCE=.\Expand.h
# End Source File
# End Group
# Begin Group "Logic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LGC_Func.cpp
# End Source File
# Begin Source File

SOURCE=.\LGC_Func.h
# End Source File
# Begin Source File

SOURCE=.\Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic.h
# End Source File
# End Group
# Begin Group "YV12Layer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\YV12L_Func.cpp
# End Source File
# Begin Source File

SOURCE=.\YV12L_Func.h
# End Source File
# Begin Source File

SOURCE=.\YV12Layer.cpp
# End Source File
# Begin Source File

SOURCE=.\YV12Layer.h
# End Source File
# Begin Source File

SOURCE=.\YV12S_Func.cpp
# End Source File
# Begin Source File

SOURCE=.\YV12S_Func.h
# End Source File
# Begin Source File

SOURCE=.\YV12Substract.cpp
# End Source File
# Begin Source File

SOURCE=.\YV12Substract.h
# End Source File
# End Group
# Begin Group "Overlay"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Overlay.cpp
# End Source File
# Begin Source File

SOURCE=.\Overlay.h
# End Source File
# End Group
# Begin Group "FitPlane"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FastFitPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\FastFitPlane.h
# End Source File
# Begin Source File

SOURCE=.\FFP_Func.cpp
# End Source File
# Begin Source File

SOURCE=.\FFP_Func.h
# End Source File
# Begin Source File

SOURCE=.\FitPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\FitPlane.h
# End Source File
# End Group
# Begin Group "Readme"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Copying.txt
# End Source File
# Begin Source File

SOURCE=.\MaskTools.txt
# End Source File
# End Group
# Begin Group "LUT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LUT.cpp
# End Source File
# Begin Source File

SOURCE=.\LUT.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GenericTools.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\avisynth.h
# End Source File
# Begin Source File

SOURCE=.\GenericTools.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\MaskTools.res
# End Source File
# End Group
# End Target
# End Project
