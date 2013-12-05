; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPppoeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "pppoe.h"

ClassCount=4
Class1=CPppoeApp
Class2=CPppoeDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PPPOE_DIALOG

[CLS:CPppoeApp]
Type=0
HeaderFile=pppoe.h
ImplementationFile=pppoe.cpp
Filter=N

[CLS:CPppoeDlg]
Type=0
HeaderFile=pppoeDlg.h
ImplementationFile=pppoeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=pppoeDlg.h
ImplementationFile=pppoeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PPPOE_DIALOG]
Type=1
Class=CPppoeDlg
ControlCount=6
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_USERNAME,edit,1350631552
Control3=IDC_PASSWORD,edit,1350631584
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

