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
	////更新窗口,消除TRACK时的虚线框
	//((CScreenDlg *)pWnd)->PaintWindow();
	//((CScreenDlg *)pWnd)->SendMessage(WM_MOUSEMOVE);		
	__super::DrawTrackerRect(lpRect,pWndClipTo,pDC,pWnd);
}

