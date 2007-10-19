# Microsoft Developer Studio Project File - Name="mdsc_main" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=mdsc_main - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mdsc_main.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mdsc_main.mak" CFG="mdsc_main - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mdsc_main - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mdsc_main - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mdsc_main - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Gr /Zp4 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ./libs/tc_4.8/CaliperMTX.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "mdsc_main - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mdsc_main - Win32 Release"
# Name "mdsc_main - Win32 Debug"
# Begin Source File

SOURCE=.\Attractions.c
# End Source File
# Begin Source File

SOURCE=.\atwork_freq_util.c
# End Source File
# Begin Source File

SOURCE=.\atwork_linkages.c
# End Source File
# Begin Source File

SOURCE=.\atwork_mc_logsum.c
# End Source File
# Begin Source File

SOURCE=.\atwork_mc_props.c
# End Source File
# Begin Source File

SOURCE=.\atwork_motorized_choice.c
# End Source File
# Begin Source File

SOURCE=.\atwork_od_utilities.c
# End Source File
# Begin Source File

SOURCE=.\atwork_se_utilities.c
# End Source File
# Begin Source File

SOURCE=.\avail_ram.c
# End Source File
# Begin Source File

SOURCE=.\BalanceAttractions.c
# End Source File
# Begin Source File

SOURCE=.\calibration_report.c
# End Source File
# Begin Source File

SOURCE=.\check_logsum.c
# End Source File
# Begin Source File

SOURCE=.\ClusterJourneys.c
# End Source File
# Begin Source File

SOURCE=.\CompetitionAreas.c
# End Source File
# Begin Source File

SOURCE=.\CountKPMGJourneys.c
# End Source File
# Begin Source File

SOURCE=.\DebugProps.c
# End Source File
# Begin Source File

SOURCE=.\discr_freq_util.c
# End Source File
# Begin Source File

SOURCE=.\discr_journey_freqs.c
# End Source File
# Begin Source File

SOURCE=.\discr_mc_logsum.c
# End Source File
# Begin Source File

SOURCE=.\discr_mc_props.c
# End Source File
# Begin Source File

SOURCE=.\discr_motorized_choice.c
# End Source File
# Begin Source File

SOURCE=.\discr_od_utilities.c
# End Source File
# Begin Source File

SOURCE=.\discr_se_utilities.c
# End Source File
# Begin Source File

SOURCE=.\echo_ini.c
# End Source File
# Begin Source File

SOURCE=.\final_reports.c
# End Source File
# Begin Source File

SOURCE=.\get_input_data.c
# End Source File
# Begin Source File

SOURCE=.\get_journey_data.c
# End Source File
# Begin Source File

SOURCE=.\get_motorized_MSC_index.c
# End Source File
# Begin Source File

SOURCE=.\get_nm_MSC_index.c
# End Source File
# Begin Source File

SOURCE=.\get_skims.c
# End Source File
# Begin Source File

SOURCE=.\get_skims_by_row.c
# End Source File
# Begin Source File

SOURCE=.\GetStopSize.c
# End Source File
# Begin Source File

SOURCE=.\isort.c
# End Source File
# Begin Source File

SOURCE=.\link_atwork.c
# End Source File
# Begin Source File

SOURCE=.\LogDensity.c
# End Source File
# Begin Source File

SOURCE=.\maint_freq_util.c
# End Source File
# Begin Source File

SOURCE=.\maint_journey_freqs.c
# End Source File
# Begin Source File

SOURCE=.\maint_mc_logsum.c
# End Source File
# Begin Source File

SOURCE=.\maint_mc_props.c
# End Source File
# Begin Source File

SOURCE=.\maint_motorized_choice.c
# End Source File
# Begin Source File

SOURCE=.\maint_od_utilities.c
# End Source File
# Begin Source File

SOURCE=.\maint_se_utilities.c
# End Source File
# Begin Source File

SOURCE=.\mdsc_main.c
# End Source File
# Begin Source File

SOURCE=.\Misc.c
# End Source File
# Begin Source File

SOURCE=.\motor_dc_coeffs.c
# End Source File
# Begin Source File

SOURCE=.\motor_dc_props.c
# End Source File
# Begin Source File

SOURCE=.\nm_dest_choice.c
# End Source File
# Begin Source File

SOURCE=.\nm_dest_gamma.c
# End Source File
# Begin Source File

SOURCE=.\nm_dest_props.c
# End Source File
# Begin Source File

SOURCE=.\OD_Utilities.c
# End Source File
# Begin Source File

SOURCE=.\ParkingCost.c
# End Source File
# Begin Source File

SOURCE=.\PreModeChoice.c
# End Source File
# Begin Source File

SOURCE=.\Read_nm_MSCIndices.c
# End Source File
# Begin Source File

SOURCE=.\Read_nm_MSCs.c
# End Source File
# Begin Source File

SOURCE=.\Read_nm_Targets.c
# End Source File
# Begin Source File

SOURCE=.\ReadAttrCorrections.c
# End Source File
# Begin Source File

SOURCE=.\ReadBPMDist1Coeffs.c
# End Source File
# Begin Source File

SOURCE=.\ReadControlFile.c
# End Source File
# Begin Source File

SOURCE=.\ReadDistFactors.c
# End Source File
# Begin Source File

SOURCE=.\ReadINI.c
# End Source File
# Begin Source File

SOURCE=.\ReadKPMGJourneys.c
# End Source File
# Begin Source File

SOURCE=.\ReadMotorizedMSCIndices.c
# End Source File
# Begin Source File

SOURCE=.\ReadMotorizedMSCs.c
# End Source File
# Begin Source File

SOURCE=.\ReadMotorizedTargets.c
# End Source File
# Begin Source File

SOURCE=.\ReadODSkims.c
# End Source File
# Begin Source File

SOURCE=.\ReadRingDistData.c
# End Source File
# Begin Source File

SOURCE=.\ReadRiverData.c
# End Source File
# Begin Source File

SOURCE=.\ReadSchoolDists.c
# End Source File
# Begin Source File

SOURCE=.\ReadSubAreaData.c
# End Source File
# Begin Source File

SOURCE=.\ReadTaxiData.c
# End Source File
# Begin Source File

SOURCE=.\ReadWalkZoneData.c
# End Source File
# Begin Source File

SOURCE=.\ReadZonalData.c
# End Source File
# Begin Source File

SOURCE=.\run_mdc.c
# End Source File
# Begin Source File

SOURCE=.\school_freq_util.c
# End Source File
# Begin Source File

SOURCE=.\school_journey_freqs.c
# End Source File
# Begin Source File

SOURCE=.\school_mc_logsum.c
# End Source File
# Begin Source File

SOURCE=.\school_mc_props.c
# End Source File
# Begin Source File

SOURCE=.\school_motorized_choice.c
# End Source File
# Begin Source File

SOURCE=.\school_od_utilities.c
# End Source File
# Begin Source File

SOURCE=.\school_se_utilities.c
# End Source File
# Begin Source File

SOURCE=.\SE_Utilities.c
# End Source File
# Begin Source File

SOURCE=.\set_function_pointers.c
# End Source File
# Begin Source File

SOURCE=.\SetCluster.c
# End Source File
# Begin Source File

SOURCE=.\SetStopsCluster.c
# End Source File
# Begin Source File

SOURCE=.\StopLocations.c
# End Source File
# Begin Source File

SOURCE=.\stops.c
# End Source File
# Begin Source File

SOURCE=.\StopsReports.c
# End Source File
# Begin Source File

SOURCE=.\univ_freq_util.c
# End Source File
# Begin Source File

SOURCE=.\univ_journey_freqs.c
# End Source File
# Begin Source File

SOURCE=.\univ_mc_logsum.c
# End Source File
# Begin Source File

SOURCE=.\univ_mc_props.c
# End Source File
# Begin Source File

SOURCE=.\univ_motorized_choice.c
# End Source File
# Begin Source File

SOURCE=.\univ_od_utilities.c
# End Source File
# Begin Source File

SOURCE=.\univ_se_utilities.c
# End Source File
# Begin Source File

SOURCE=.\ValidStop.c
# End Source File
# Begin Source File

SOURCE=.\work_freq_util.c
# End Source File
# Begin Source File

SOURCE=.\work_journey_freqs.c
# End Source File
# Begin Source File

SOURCE=.\work_mc_logsum.c
# End Source File
# Begin Source File

SOURCE=.\work_mc_props.c
# End Source File
# Begin Source File

SOURCE=.\work_motorized_choice.c
# End Source File
# Begin Source File

SOURCE=.\work_od_utilities.c
# End Source File
# Begin Source File

SOURCE=.\work_se_utilities.c
# End Source File
# End Target
# End Project
