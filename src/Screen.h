
// ScreenSpy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

class CScreenApp : public CWinApp
{
public:
	CScreenApp();

public:
	HWND m_hwndDlg;//���������Dialog
	virtual BOOL InitInstance();
	ULONG_PTR m_gdiToken;

	DECLARE_MESSAGE_MAP()
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
};

extern CScreenApp theApp;