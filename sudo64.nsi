Name "sudo"
OutFile "sudoInstaller64.exe"
InstallDir $PROGRAMFILES\sudo

!include MUI.nsh

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "SimpChinese"

Section "MainSection" SEC01
    SetOutPath $INSTDIR
    
    File "sudo.c"
    File "sudo64.exe"
    File "NSudo64.exe"

    ReadRegStr $0 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH"
    
    StrCpy $1 "$0;$INSTDIR"
    
    WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH" "$1"
    
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\sudo" "DisplayName" "sudo"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\sudo" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\sudo" "InstallLocation" "$INSTDIR"
SectionEnd

Section "Uninstall"
    Delete $INSTDIR\*.*
    
    RMDir $INSTDIR

    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\sudo"
SectionEnd