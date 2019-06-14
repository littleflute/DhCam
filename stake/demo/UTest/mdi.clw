; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBounceWnd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mdi.h"
LastPage=0

ClassCount=9
Class1=CBounceWnd
Class2=CDlgPrp
Class3=CMainFrame
Class4=CMdiApp
Class5=CCoolBar
Class6=CSizingControlBar
Class7=CSCBMiniDockFrameWnd
Class8=CXdTreeCtrl

ResourceCount=7
<<<<<<< .mine
Resource1=IDD_DLG_PRP
Resource2=IDD_ABOUTBOX (English (U.S.))
Resource3=IDR_BOUNCE (English (U.S.))
Resource4=IDR_MAINFRAME
Resource5=IDD_DLG_WB
Resource6=IDR_HELLO (English (U.S.))
=======
Resource1=IDD_DLG_PRP
Resource2=IDD_ABOUTBOX (English (U.S.))
Resource3=IDR_BOUNCE (English (U.S.))
Resource4=IDR_MAINFRAME
Resource5=IDD_DLG_WB
Resource6=IDR_HELLO (English (U.S.))
>>>>>>> .r200
Class9=CWBDlg
Resource7=IDR_MAINFRAME (English (U.S.))

[CLS:CBounceWnd]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=bounce.h
ImplementationFile=bounce.cpp
LastObject=CBounceWnd
Filter=M
VirtualFilter=mfWC

[CLS:CDlgPrp]
Type=0
BaseClass=CDialog
HeaderFile=DlgPrp.h
ImplementationFile=DlgPrp.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=mainfrm.h
ImplementationFile=mainfrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CMdiApp]
Type=0
BaseClass=CWinApp
HeaderFile=mdi.h
ImplementationFile=mdi.cpp

[CLS:CCoolBar]
Type=0
BaseClass=baseCCoolBar
HeaderFile=scbarg.h
ImplementationFile=scbarg.cpp

[CLS:CSizingControlBar]
Type=0
BaseClass=baseCSizingControlBar
HeaderFile=sizecbar.h
ImplementationFile=sizecbar.cpp

[CLS:CSCBMiniDockFrameWnd]
Type=0
BaseClass=baseCSCBMiniDockFrameWnd
HeaderFile=sizecbar.h
ImplementationFile=sizecbar.cpp

[CLS:CXdTreeCtrl]
Type=0
BaseClass=CTreeCtrl
HeaderFile=XdTreeCtrl.h
ImplementationFile=XdTreeCtrl.cpp

[DLG:IDD_DLG_PRP]
Type=1
Class=CDlgPrp
ControlCount=1
Control1=IDOK,button,1342242817

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDOK,button,1342373889
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_AOI_MACHING_BEST
Command2=ID_GRAB_CONTINUOUS
Command3=ID_GRAB_CANCEL
Command4=ID_GRAB_SINGLE
Command5=ID_BUTTON_ENUMERATE_DEVICES
CommandCount=5

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_APP_ABOUT
CommandCount=1

[MNU:IDR_BOUNCE (English (U.S.))]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_VIEW_TOOLBAR
Command3=ID_VIEW_STATUS_BAR
Command4=ID_VIEW_TYPE_BAR
Command5=IDM_CUSTOM
Command6=ID_OUTPUTWIN_SCALE50
Command7=ID_OUTPUTWIN_SCALE100
Command8=ID_OUTPUTWIN_SCALE150
Command9=ID_OUTPUTWIN_SCALE200
Command10=ID_OUTPUTWIN_SCALE_USER
Command11=ID_IMAGEMODE
Command12=ID_WHITE_BALANCE
Command13=ID_GRAB_SINGLE
Command14=ID_GRAB_CONTINUOUS
Command15=ID_GRAB_CANCEL
Command16=ID_WINDOW_CASCADE
Command17=ID_WINDOW_TILE_VERT
Command18=ID_WINDOW_ARRANGE
Command19=ID_APP_ABOUT
CommandCount=19

[MNU:IDR_HELLO (English (U.S.))]
Type=1
Class=?
Command1=IDM_BOUNCE
Command2=IDM_HELLO
Command3=ID_APP_EXIT
Command4=IDM_BLACK
Command5=IDM_RED
Command6=IDM_GREEN
Command7=IDM_BLUE
Command8=IDM_WHITE
Command9=IDM_CUSTOM
Command10=ID_WINDOW_CASCADE
Command11=ID_WINDOW_TILE_VERT
Command12=ID_WINDOW_ARRANGE
Command13=ID_APP_ABOUT
CommandCount=13

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_VIEW_TOOLBAR
Command3=ID_VIEW_STATUS_BAR
Command4=ID_VIEW_TYPE_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[DLG:IDD_DLG_WB]
Type=1
Class=CWBDlg
ControlCount=11
Control1=IDC_EDIT_RED_RATIO,edit,1350631552
Control2=IDC_EDIT_GREEN_RATIO,edit,1350633600
Control3=IDC_EDIT_BLUE_RATIO,edit,1350631552
Control4=IDOK,button,1342242816
Control5=IDCANCEL,button,1342242816
Control6=ID_WB_RATIO,button,1342242816
Control7=IDC_STATIC,static,1342308866
Control8=IDC_STATIC,static,1342308866
Control9=IDC_STATIC,static,1342308866
Control10=IDC_STATIC,button,1342177287
Control11=ID_WB_SET,button,1342242816

[CLS:CWBDlg]
Type=0
HeaderFile=WBDlg.h
ImplementationFile=WBDlg.cpp
BaseClass=CDialog
LastObject=IDM_CUSTOM
Filter=D
VirtualFilter=dWC

