#include "stdafx.h"
#include "CRectTrackerInheritance.h"
#include "ScreenDlg.h"

CRectTrackerInheritance::CRectTrackerInheritance()
{

}

CRectTrackerInheritance::~CRectTrackerInheritance()
{
}


void CRectTrackerInheritance::DrawTrackerRect( LPCRECT lpRect, CWnd* pWndClipTo,CDC* pDC, CWnd* pWnd )
{	    
	////���´���,����TRACKʱ�����߿�
	//((CScreenDlg *)pWnd)->PaintWindow();
	//((CScreenDlg *)pWnd)->SendMessage(WM_MOUSEMOVE);		
	__super::DrawTrackerRect(lpRect,pWndClipTo,pDC,pWnd);
}

