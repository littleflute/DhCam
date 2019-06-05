; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CParametersDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "hvapitest.h"
LastPage=0

ClassCount=4
Class1=CDhTestApp
Class2=CAboutDlg
Class3=CDhTestDlg

ResourceCount=5
Resource1=IDR_MENU_MAIN
Resource2=IDD_DHTEST_DIALOG
Resource3=IDD_ABOUTBOX
Resource4=IDD_DIALOG_PARAMETERS
Class4=CParametersDlg
Resource5=IDR_TOOLBAR1

[CLS:CDhTestApp]
Type=0
BaseClass=CWinApp
HeaderFile=HvApiTest.h
ImplementationFile=HvApiTest.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=HvApiTestDlg.cpp
ImplementationFile=HvApiTestDlg.cpp
LastObject=CAboutDlg

[CLS:CDhTestDlg]
Type=0
BaseClass=CDialog
HeaderFile=HvApiTestDlg.h
ImplementationFile=HvApiTestDlg.cpp
LastObject=CDhTestDlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC_VERSION,static,1342308480
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308480
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_DHTEST_DIALOG]
Type=1
Class=CDhTestDlg
ControlCount=2
Control1=IDC_STATIC_MAIN_UI,static,1342177284
Control2=IDC_BUTTONGROUP,button,1342177287

[MNU:IDR_MENU_MAIN]
Type=1
Class=?
Command1=ID_FILE_EXIT
Command2=ID_TEST_ITEM_1
Command3=ID_TEST_HVGETDEVINFO
Command4=ID_HELP_ABOUT
CommandCount=4

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_BUTTON_1
Command2=ID_BUTTON_2
Command3=ID_BUTTON_3
Command4=ID_BUTTON_4
Command5=ID_HELP_ABOUT
CommandCount=5

[DLG:IDD_DIALOG_PARAMETERS]
Type=1
Class=CParametersDlg
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDC_SLIDER_SLEEP_TIME,msctls_trackbar32,1342242836
Control3=IDC_STATIC_SLEEP_TIME_VALUE,static,1342308353
Control4=IDC_STATIC,button,1342177287
Control5=IDC_COMBO_AOI_TEST_MODE,combobox,1344340226
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_COORDINATE_START,edit,1350631553
Control12=IDC_COORDINATE_END,edit,1350631553
Control13=IDC_EDIT_PRECISION,edit,1350631553
Control14=IDC_COMBO_AOI_TEST_LANGE,combobox,1344340226
Control15=IDC_STATIC,static,1342308352

[CLS:CParametersDlg]
Type=0
HeaderFile=ParametersDlg.h
ImplementationFile=ParametersDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO_AOI_TEST_LANGE
VirtualFilter=dWC

