# Microsoft Developer Studio Project File - Name="MDBL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MDBL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MDBL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MDBL.mak" CFG="MDBL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MDBL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MDBL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Win32/MDBL", OAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MDBL - Win32 Release"

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
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\wcl" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX"MBDL.hpp" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "MDBL - Win32 Debug"

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
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /I "..\wcl" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX"MBDL.hpp" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "MDBL - Win32 Release"
# Name "MDBL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Column.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Field.cpp
# End Source File
# Begin Source File

SOURCE=.\IntMapIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\JoinedSet.cpp
# End Source File
# Begin Source File

SOURCE=.\MDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBCCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBCException.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBCParams.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBCSource.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Row.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLException.cpp
# End Source File
# Begin Source File

SOURCE=.\StrMapIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\Table.cpp
# End Source File
# Begin Source File

SOURCE=.\TableSet.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeStamp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoTrans.hpp
# End Source File
# Begin Source File

SOURCE=.\Column.hpp
# End Source File
# Begin Source File

SOURCE=.\ColumnSet.hpp
# End Source File
# Begin Source File

SOURCE=.\Compare.hpp
# End Source File
# Begin Source File

SOURCE=.\Field.hpp
# End Source File
# Begin Source File

SOURCE=.\Index.hpp
# End Source File
# Begin Source File

SOURCE=.\IntMapIndex.hpp
# End Source File
# Begin Source File

SOURCE=.\Join.hpp
# End Source File
# Begin Source File

SOURCE=.\JoinedSet.hpp
# End Source File
# Begin Source File

SOURCE=.\MDB.hpp
# End Source File
# Begin Source File

SOURCE=.\MDBL.hpp
# End Source File
# Begin Source File

SOURCE=.\MDBLTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\ODBCCursor.hpp
# End Source File
# Begin Source File

SOURCE=.\ODBCException.hpp
# End Source File
# Begin Source File

SOURCE=.\ODBCParams.hpp
# End Source File
# Begin Source File

SOURCE=.\ODBCSource.hpp
# End Source File
# Begin Source File

SOURCE=.\ResultSet.hpp
# End Source File
# Begin Source File

SOURCE=.\Row.hpp
# End Source File
# Begin Source File

SOURCE=.\RowSet.hpp
# End Source File
# Begin Source File

SOURCE=.\SortColumns.hpp
# End Source File
# Begin Source File

SOURCE=.\SQLCursor.hpp
# End Source File
# Begin Source File

SOURCE=.\SQLException.hpp
# End Source File
# Begin Source File

SOURCE=.\SQLParams.hpp
# End Source File
# Begin Source File

SOURCE=.\SQLSource.hpp
# End Source File
# Begin Source File

SOURCE=.\StrMapIndex.hpp
# End Source File
# Begin Source File

SOURCE=.\Table.hpp
# End Source File
# Begin Source File

SOURCE=.\TableSet.hpp
# End Source File
# Begin Source File

SOURCE=.\TimeStamp.hpp
# End Source File
# Begin Source File

SOURCE=.\UniqIndex.hpp
# End Source File
# Begin Source File

SOURCE=.\Value.hpp
# End Source File
# Begin Source File

SOURCE=.\ValueSet.hpp
# End Source File
# Begin Source File

SOURCE=.\Where.hpp
# End Source File
# Begin Source File

SOURCE=.\WhereExp.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# End Target
# End Project
